#include "Mc1Sound.h"
#include "../../remc2/portability/port_outputs.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#include "Mc1Blocks.h"
#include "Mc1Entity.h"
#include "Mc1Game.h"
#include "Mc1Menu.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Spells.h"
#include "Mc1Wizard.h"

uint8_t soundInitialized_90AD4_90AD4 = 0;
uint8_t hmiTimer_90AD5_90AD5 = 0;
uint16_t timerHandle_80010_80010 = 0xFFFF;
volatile uint32_t timerTicks_AC5D4_AC5C4 = 0;

char musicDriverName_CBF74_CBF64[32];
uint32_t musicPort_CBF70_CBF60 = 0;
char soundDriverName_CC158_CC148[32];
uint32_t soundPort_CC14C_CC13C = 0;
uint32_t soundIrq_CC150_CC140 = 0;
uint32_t soundDma_CC1BC_CC1AC = 0;

uint16_t midiDriver_CBFDC_CBFCC = 0;
uint16_t song_CBFD8_CBFC8 = 0;
uint16_t digiDriver_CC140_CC130 = 0;
uint8_t musicFile_CBFEE_CBFDE = 0;
uint8_t fmBanks_CBFEF_CBFDF = 0;
uint8_t secondMidiDriver_9AF9C_9AF9C = 0;
// 9AF18: tracks 0-15 take the map of the song file (_MIDI_MAP_TRACK), 16-31 the driver 0
const uint32_t trackMap_9AF18_9AF18[32] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
Type_SosMidiInitSong initSong_12F030_12F020 = {};
Type_SosStartSample startSample_939F0_939F0 = {};

uint8_t* musicBankTab_CBF60_CBF50 = nullptr;
uint8_t* musicBankTabEnd_CBF64_CBF54 = nullptr;
uint8_t* musicBankData_CBF68_CBF58 = nullptr;
uint8_t* soundBankData_CC130_CC120 = nullptr;
uint8_t* soundBankTab_CC154_CC144 = nullptr;
uint8_t* soundBankTabEnd_CC1C0_CC1B0 = nullptr;
char musicDatPath_9AE00_9AE00[28] = "data/music0-0.dat";
char musicTabPath_9AE2C_9AE2C[28] = "data/music0-0.tab";
char soundDatPath_9AE84_9AE84[28] = "data/snds0-0.dat";
char soundTabPath_9AEB0_9AEB0[28] = "data/snds0-0.tab";

// 9AE00, 9AE84: the paths are rewritten for the bank to load
Type_Mc1FileEntry musicFiles_9AE00_9AE00[3] = {
	{ { musicDatPath_9AE00_9AE00, musicDatPath_9AE00_9AE00 }, &musicBankData_CBF68_CBF58, nullptr, 0, 0 },
	{ { musicTabPath_9AE2C_9AE2C, musicTabPath_9AE2C_9AE2C }, &musicBankTab_CBF60_CBF50, &musicBankTabEnd_CBF64_CBF54, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};
Type_Mc1FileEntry soundFiles_9AE84_9AE84[3] = {
	{ { soundDatPath_9AE84_9AE84, soundDatPath_9AE84_9AE84 }, &soundBankData_CC130_CC120, nullptr, 0, 0 },
	{ { soundTabPath_9AEB0_9AEB0, soundTabPath_9AEB0_9AEB0 }, &soundBankTab_CC154_CC144, &soundBankTabEnd_CC1C0_CC1B0, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};
uint16_t musicVolume_939D0_939D0 = 0;
uint16_t currentMusic_939D2_939D2 = 0;
uint16_t musicCount_939D6_939D6 = 0;
uint16_t soundCount_CC1C6_CC1B6 = 0;
uint16_t sampleOwners_CBFF0_CBFE0[32][2];
uint16_t sampleVolumes_CC070_CC060[32];


// the 32-byte records of the bank tabs: name, data offset (+12h), size (+1Ah); port: the offsets stay offsets
static uint8_t* BankData(uint8_t* tab, uint8_t* data, int16_t index)
{
	uint32_t offset;
	memcpy(&offset, tab + index * 32 + 0x12, 4);
	return data + offset;
}

static uint32_t BankSize(uint8_t* tab, int16_t index)
{
	uint32_t size;
	memcpy(&size, tab + index * 32 + 0x1A, 4);
	return size;
}

void StopMusic_20E60_20E60()
{
	if (!byte_939CC_939CC || !byte_939CD_939CD || !currentMusic_939D2_939D2)
		return;
	if (!sosMIDISongDone_5D7F0_5DD00(song_CBFD8_CBFC8))
		sosMIDIStopSong_5F420_5F930(song_CBFD8_CBFC8);
	sosMIDIUnInitSong_5F355_5F865(song_CBFD8_CBFC8);
	sosMIDIResetDriver_6091D_60E2D(midiDriver_CBFDC_CBFCC);
	currentMusic_939D2_939D2 = 0;
}

uint32_t TimerTick_357A0_35B60(uint32_t interval)
{
	// 357A0: inc dword_AC5D4; retf
	timerTicks_AC5D4_AC5C4 = timerTicks_AC5D4_AC5C4 + 1;
	return interval;
}

// port: the timer of DOS as a thread on the real time, the ticks at the times of DOS without a drift: the HMI event of
// 120 Hz (357C0) or the own one on int 8 with the PIT divisor 26AFh (5A459)
static std::thread timerThread;
static std::atomic<bool> timerRunning{ false };

static void StopTimerThread()
{
	timerRunning = false;
	if (timerThread.joinable())
		timerThread.join();
}

static void StartTimerThread(double hertz)
{
	StopTimerThread();
	timerRunning = true;
	timerThread = std::thread([hertz]() {
		const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		for (uint64_t tick = 1; timerRunning; tick++)
		{
			std::this_thread::sleep_until(start + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(tick / hertz)));
			if (timerRunning)
				TimerTick_357A0_35B60(0);
		}
	});
}

// the thread stopped before the statics go away when the game did not restore the timer
static struct TimerThreadGuard
{
	~TimerThreadGuard() { StopTimerThread(); }
} timerThreadGuard;

void SoundInit_357C0_35B80()
{
	if (soundInitialized_90AD4_90AD4)
		return;
	soundInitialized_90AD4_90AD4 = 1;
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x48))
	{
		PrintLoadingText_5AD10_5B220("Initialise Music");
		InitMusic_3C570_3C8B0();
		PrintLoadingText_5AD10_5B220("Initialise Sound");
		InitSound_3C800_3CB40();
	}
	else
	{
		PrintLoadingText_5AD10_5B220("Sound Disabled");
		byte_939CD_939CD = 0;
		byte_939E5_939E5 = 0;
		byte_939CC_939CC = 0;
		byte_939E4_939E4 = 0;
	}
	if (byte_939E4_939E4 || byte_939CC_939CC)
	{
		// 5D97B (sosTIMERRegisterEvent 120 Hz), port: the timer thread
		StartTimerThread(120.0);
		timerHandle_80010_80010 = 0;
		hmiTimer_90AD5_90AD5 = 1;
	}
	else
		InstallTimer_5A459_5A969();
	switch (mc1Settings_AE408_AE3F8->language_0x97_151)
	{
	case 0:
		if (!byte_939E4_939E4)
			*subtitles_90090_90090 = 1;
		break;
	case 1:
	case 2:
	case 3:
		*subtitles_90090_90090 = 1;
		break;
	}
}

void ShutdownSound_358A0_35C60()
{
	if (!hmiTimer_90AD5_90AD5)
	{
		RestoreTimer_5A4EA_5A9FA();
		return;
	}
	StopTimerThread(); // 5DC91: sosTIMERRemoveEvent
	timerHandle_80010_80010 = 0xFFFF;
	hmiTimer_90AD5_90AD5 = 0;
	ShutdownDigital_61F90_624A0();
	ShutdownMusic_62020_62530();
}

void InitMusic_3C570_3C8B0()
{
	// port: "MUSIC = %s %x" of sndsetup.inf from the setup in memory (LoadSoundSetup_505A0_508E0)
	strcpy(musicDriverName_CBF74_CBF64, musicDriver_12CA00_12C9F0);
	musicPort_CBF70_CBF60 = strtoul(musicPort_12CAA8_12CA98, nullptr, 16);
	if (const char* environment = getenv("BF_MUSIC"))
		sscanf(environment, "%31s %x", musicDriverName_CBF74_CBF64, &musicPort_CBF70_CBF60);
	if (!_stricmp(musicDriverName_CBF74_CBF64, "none"))
	{
		byte_939CC_939CC = 0;
		byte_939CD_939CD = 0;
	}
	if (!byte_939CC_939CC)
		return;
	static const char* const fmCards[] = { "ADLIB", "SBLAST", "SBPRO", "ADLIBG", "SB16FM", "GRAVIS", "PASFM", "COMPATIBLE" };
	for (const char* card : fmCards)
		if (!_stricmp(musicDriverName_CBF74_CBF64, card))
		{
			InitMusicDriver_43B20_43E60(0xA002); // _MIDI_FM
			return;
		}
	if (!_stricmp(musicDriverName_CBF74_CBF64, "ROLAND"))
		InitMusicDriver_43B20_43E60(0xA004); // _MIDI_MT_32
	else if (!_stricmp(musicDriverName_CBF74_CBF64, "SBAWE32"))
		InitMusicDriver_43B20_43E60(0xA008); // _MIDI_AWE32
	else if (!_stricmp(musicDriverName_CBF74_CBF64, "GENERAL") || !_stricmp(musicDriverName_CBF74_CBF64, "WBLAST"))
		InitMusicDriver_43B20_43E60(0xA001); // _MIDI_MPU_401
}

void InitSound_3C800_3CB40()
{
	// port: "SOUNDFX = %s %x %d %d" of sndsetup.inf from the setup in memory
	strcpy(soundDriverName_CC158_CC148, soundDriver_12C9A0_12C990);
	soundPort_CC14C_CC13C = strtoul(soundPort_12CA9E_12CA8E, nullptr, 16);
	soundIrq_CC150_CC140 = atoi(soundIrq_12CAB2_12CAA2);
	soundDma_CC1BC_CC1AC = atoi(soundDma_12CAC6_12CAB6);
	if (const char* environment = getenv("BF_SOUND"))
		sscanf(environment, "%31s %x %d %d", soundDriverName_CC158_CC148, &soundPort_CC14C_CC13C, &soundIrq_CC150_CC140,
			&soundDma_CC1BC_CC1AC);
	if (!_stricmp(soundDriverName_CC158_CC148, "none"))
	{
		byte_939E4_939E4 = 0;
		byte_939E5_939E5 = 0;
	}
	if (byte_939E4_939E4)
		InitDigitalDriver_43EC0_44200();
}

void InitMusicDriver_43B20_43E60(uint16_t device)
{
	// sosTIMERInitSystem / sosMIDIInitSystem: nothing to do in the port
	if (sosMIDIInitDriver_6035F_6086F(device, &midiDriver_CBFDC_CBFCC))
	{
		myprintf("\nError : %s\n", "MIDI driver");
		byte_939CC_939CC = 0;
		byte_939CD_939CD = 0;
		return;
	}
	switch (device)
	{
	case 0xA001:
		fmBanks_CBFEF_CBFDF = 0;
		musicFile_CBFEE_CBFDE = 2;
		break;
	case 0xA002:
		// port: not reached, the MIDI driver is the AWE32 only (the original loads data\inst.bnk and data\drum.bnk)
		fmBanks_CBFEF_CBFDF = 1;
		musicFile_CBFEE_CBFDE = 0;
		break;
	case 0xA004:
		fmBanks_CBFEF_CBFDF = 0;
		musicFile_CBFEE_CBFDE = 1;
		break;
	case 0xA008:
		fmBanks_CBFEF_CBFDF = 0;
		musicFile_CBFEE_CBFDE = 2;
		break;
	}
	if (byte_939CC_939CC && LoadMusicBank_5CEF0_5D400(0))
	{
		myprintf("\nError opening music files\n");
		sosMIDIUnInitDriver_6080C_60D1C(midiDriver_CBFDC_CBFCC, true);
		byte_939CC_939CC = 0;
		byte_939CD_939CD = 0;
	}
}

void InitDigitalDriver_43EC0_44200()
{
	// the original finds the card of SOUNDFX (sosDIGIDetect*) and mixes at 11025 Hz (byte_939EC 0 / 3) or
	// 22050 Hz (1); port: the SB16 of the MSS emulation
	uint16_t sampleRate = byte_939EC_939EC == 1 ? 0x5622 : 0x2B11;
	if (sosDIGIInitDriver_64846_64D56(sampleRate, &digiDriver_CC140_CC130))
	{
		myprintf("\nError : %s\n", "digital driver");
		byte_939E4_939E4 = 0;
		byte_939E5_939E5 = 0;
		FreeFileGroup_61610_61B20(soundFiles_9AE84_9AE84);
	}
	// the timer event of the driver (sosTIMERRegisterEvent with word_CC1C4) feeds the DMA buffer, not needed here
}

void InstallTimer_5A459_5A969()
{
	mc1Settings_AE408_AE3F8->timerDivisor_0xCE_206 = 0x26AF;
	mc1Settings_AE408_AE3F8->unknown_0xD2_210 = 0;
	// the original sets PIT channel 0 to the divisor (120.49 Hz) and hooks int 8 (5A3E3), port: the timer thread
	if (timerHandle_80010_80010 == 0xFFFF)
	{
		StartTimerThread(1193182.0 / 0x26AF);
		timerHandle_80010_80010 = 0;
	}
}

void RestoreTimer_5A4EA_5A9FA()
{
	// the original restores int 8 and the PIT
	if (timerHandle_80010_80010 == 0xFFFF)
		return;
	StopTimerThread();
	timerHandle_80010_80010 = 0xFFFF;
}

int32_t LoadMusicBank_5CEF0_5D400(uint16_t bank)
{
	if (!byte_939CC_939CC)
		return 1;
	sprintf(musicDatPath_9AE00_9AE00, "data/music%d-%d.dat", bank, musicFile_CBFEE_CBFDE);
	sprintf(musicTabPath_9AE2C_9AE2C, "data/music%d-%d.tab", bank, musicFile_CBFEE_CBFDE);
	if (LoadFileEntry_634E0_639F0(&musicFiles_9AE00_9AE00[0]) != 1)
		return 1;
	if (LoadFileEntry_634E0_639F0(&musicFiles_9AE00_9AE00[1]) != 1)
	{
		FreeFileEntry_634A0_639B0(&musicFiles_9AE00_9AE00[0]);
		return 1;
	}
	CountMusic_5CFA4_5D4B4();
	return 0;
}

void CountMusic_5CFA4_5D4B4()
{
	// the original also adds the data address to the offsets (+12h), the port keeps the offsets
	if (!musicBankTab_CBF60_CBF50 || !musicBankData_CBF68_CBF58)
		return;
	musicCount_939D6_939D6 = 0;
	for (uint8_t* entry = musicBankTab_CBF60_CBF50 + 32; entry < musicBankTabEnd_CBF64_CBF54; entry += 32)
		musicCount_939D6_939D6++;
}

void StopAllSamples_5D010_5D520()
{
	if (!byte_939E4_939E4)
		return;
	for (uint8_t slot = 0; slot < 32; slot++)
		while (!sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			sosDIGIStopSample_65965_65E75(digiDriver_CC140_CC130, slot);
}

int32_t LoadSoundBank_5D070_5D580(uint8_t bank)
{
	if (!byte_939E4_939E4)
		return 1;
	sprintf(soundDatPath_9AE84_9AE84, "data/snds%d-%d.dat", bank, byte_939EC_939EC);
	sprintf(soundTabPath_9AEB0_9AEB0, "data/snds%d-%d.tab", bank, byte_939EC_939EC);
	if (LoadFileEntry_634E0_639F0(&soundFiles_9AE84_9AE84[0]) != 1)
		return 1;
	if (LoadFileEntry_634E0_639F0(&soundFiles_9AE84_9AE84[1]) != 1)
	{
		FreeFileEntry_634A0_639B0(&soundFiles_9AE84_9AE84[0]);
		return 1;
	}
	CountSounds_5D138_5D648();
	return 0;
}

void CountSounds_5D138_5D648()
{
	// the original also adds the data address to the offsets (+12h), the port keeps the offsets
	if (!soundBankTab_CC154_CC144 || !soundBankData_CC130_CC120)
		return;
	soundCount_CC1C6_CC1B6 = 0;
	for (uint8_t* entry = soundBankTab_CC154_CC144 + 32; entry < soundBankTabEnd_CC1C0_CC1B0; entry += 32)
		soundCount_CC1C6_CC1B6++;
}

void PlayMusic_5D290_5D7A0(int16_t music)
{
	if (!byte_939CC_939CC || !byte_939CD_939CD || music > (int16_t)musicCount_939D6_939D6)
		return;
	if (currentMusic_939D2_939D2 == music)
		return;
	if (currentMusic_939D2_939D2)
	{
		if (!sosMIDISongDone_5D7F0_5DD00(song_CBFD8_CBFC8))
			sosMIDIStopSong_5F420_5F930(song_CBFD8_CBFC8);
		sosMIDIUnInitSong_5F355_5F865(song_CBFD8_CBFC8);
		sosMIDIResetDriver_6091D_60E2D(midiDriver_CBFDC_CBFCC);
		currentMusic_939D2_939D2 = 0;
	}
	initSong_12F030_12F020.songData_0x00_0 = BankData(musicBankTab_CBF60_CBF50, musicBankData_CBF68_CBF58, music);
	initSong_12F030_12F020.songCallback_0x06_6 = nullptr;
	if (sosMIDIInitSong_5EE22_5F332(&initSong_12F030_12F020, trackMap_9AF18_9AF18, &song_CBFD8_CBFC8))
	{
		myprintf("\nError : %s", "song");
		sosMIDIUnInitDriver_6080C_60D1C(midiDriver_CBFDC_CBFCC, true);
		byte_939CC_939CC = 0;
		return;
	}
	if (sosMIDIStartSong_5F398_5F8A8(song_CBFD8_CBFC8))
	{
		myprintf("\nError : %s", "song");
		sosMIDIUnInitDriver_6080C_60D1C(midiDriver_CBFDC_CBFCC, true);
		return;
	}
	sosMIDIEnableChannelStealing_5EDD6_5F2E6(0);
	musicVolume_939D0_939D0 = 100;
	currentMusic_939D2_939D2 = music;
}

static void StartSample(int16_t sample)
{
	startSample_939F0_939F0.size_0x08_8 = BankSize(soundBankTab_CC154_CC144, sample) - 0x10;
	startSample_939F0_939F0.sample_0x00_0 = BankData(soundBankTab_CC154_CC144, soundBankData_CC130_CC120, sample);
	sosDIGIStartSample_6535C_6586C(digiDriver_CC140_CC130, &startSample_939F0_939F0);
}

void PlaySampleLooped_5D460_5D970(int16_t owner, int16_t sample, int16_t loops)
{
	if (!byte_939E4_939E4 || !byte_939E5_939E5 || sample > (int16_t)soundCount_CC1C6_CC1B6)
		return;
	// the same owner and sample still playing: nothing
	for (uint8_t slot = 0; slot < 32; slot++)
		if (sampleOwners_CBFF0_CBFE0[slot][0] == (uint16_t)owner && sampleOwners_CBFF0_CBFE0[slot][1] == (uint16_t)sample
			&& !sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			return;
	uint8_t slot = 0;
	for (; slot < 32; slot++)
		if (sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			break;
	if (slot == 32)
		return;
	sampleOwners_CBFF0_CBFE0[slot][0] = owner;
	sampleOwners_CBFF0_CBFE0[slot][1] = sample;
	startSample_939F0_939F0.flags_0x1C_28 = SOS_LOOPING | SOS_VOLUME;
	startSample_939F0_939F0.loopCount_0x0C_12 = loops;
	startSample_939F0_939F0.volume_0x10_16 = 0x7FFF;
	sampleVolumes_CC070_CC060[slot] = startSample_939F0_939F0.volume_0x10_16;
	startSample_939F0_939F0.sampleId_0x12_18 = slot;
	StartSample(sample);
}

void PlaySample_5D610_5DB20(int16_t owner, int16_t sample)
{
	if (!byte_939E4_939E4 || !byte_939E5_939E5 || sample > (int16_t)soundCount_CC1C6_CC1B6)
		return;
	uint8_t slot = 0;
	for (; slot < 32; slot++)
		if (sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			break;
	if (slot == 32)
		return;
	sampleOwners_CBFF0_CBFE0[slot][0] = owner;
	sampleOwners_CBFF0_CBFE0[slot][1] = sample;
	startSample_939F0_939F0.sampleId_0x12_18 = slot;
	startSample_939F0_939F0.flags_0x1C_28 = SOS_VOLUME;
	startSample_939F0_939F0.volume_0x10_16 = 0x7FFF;
	sampleVolumes_CC070_CC060[slot] = startSample_939F0_939F0.volume_0x10_16;
	StartSample(sample);
}

void StopSample_5D750_5DC60(int16_t owner, int16_t sample)
{
	if (!byte_939E4_939E4)
		return;
	for (int16_t slot = 0; slot < 32; slot++)
		if (sampleOwners_CBFF0_CBFE0[slot][0] == (uint16_t)owner && sampleOwners_CBFF0_CBFE0[slot][1] == (uint16_t)sample
			&& !sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
		{
			sosDIGIStopSample_65965_65E75(digiDriver_CC140_CC130, slot);
			return;
		}
}

void ShutdownDigital_61F90_624A0()
{
	if (!byte_939E4_939E4)
		return;
	StopAllSamples_5D010_5D520();
	// 61FAA waits for sub_559A0 (playing samples) to reach 0, all are stopped above
	// 661A4 sets the master volume 0, 5DC91 removes the timer event of the driver, 5D948 / 6481C end the HMI systems
	sosDIGIUnInitDriver_64F78_65488(digiDriver_CC140_CC130, true, true);
	FreeFileGroup_61610_61B20(soundFiles_9AE84_9AE84);
}

void ShutdownMusic_62020_62530()
{
	if (!byte_939CC_939CC)
		return;
	if (currentMusic_939D2_939D2)
	{
		sosMIDIStopSong_5F420_5F930(song_CBFD8_CBFC8);
		sosMIDIUnInitSong_5F355_5F865(song_CBFD8_CBFC8);
		sosMIDIResetDriver_6091D_60E2D(midiDriver_CBFDC_CBFCC);
	}
	sosMIDIUnInitDriver_6080C_60D1C(midiDriver_CBFDC_CBFCC, true);
	// fmBanks_CBFEF_CBFDF: the original frees inst.bnk and drum.bnk (62128), not loaded in the port
	FreeFileGroup_61610_61B20(musicFiles_9AE00_9AE00);
}

void RestartMenuMusic_65E00_66310()
{
	if (!byte_939CC_939CC || !byte_939CD_939CD || !currentMusic_939D2_939D2)
		return;
	if (!sosMIDISongDone_5D7F0_5DD00(song_CBFD8_CBFC8))
		return;
	sosMIDIResetSong_5F529_5FA39(song_CBFD8_CBFC8, &initSong_12F030_12F020);
	sosMIDIStartSong_5F398_5F8A8(song_CBFD8_CBFC8);
}

// ---- the sounds of the game ----------------------------------------------------------------------------------

// 12CD20: the requests of the frame by the sound (sample) number, played by 55100
typedef struct {
	uint16_t state_0x00; // 1 restart, 2 play, 3 play when not playing, 4 the looped sample (200h stops it)
	uint16_t pan_0x02;
	uint16_t volume_0x04; // the loudest request of the frame
	uint16_t host_0x06; // the owner of the sample (the id of the entity)
	uint16_t word_0x08; // 2 after the request is played
} Type_Mc1SoundRequest;

static Type_Mc1SoundRequest soundRequests_12CD20_12CD10[47];
// the looped samples faded in (51FC0) or out (522E0) by the slot
static uint8_t fadeIn_12CC20_12CC10[32];
static uint16_t fadeInLimit_12CC40_12CC30[32]; // the volume of the end / 100h
static uint16_t fadeOutSpeed_12CC80_12CC70[32];
static uint8_t fadeOut_12CCC0_12CCB0[32];
static uint16_t fadeOutVolume_12CCE0_12CCD0[32];
static uint16_t fadeSpeed_968D8_968D8;
// the music of the danger (20D00): the volume of the channels 3-5, the mode 1 calm / 2 danger, the timer, the step
static uint8_t dangerVolume_9008C_9008C;
static uint8_t dangerMode_9008D_9008D;
static uint8_t dangerTimer_9008E_9008E;
static int8_t dangerStep_9008F_9008F;
static uint16_t dangerTimerHandle_80000_80000;

static Type_Mc1Entity* SoundEntities()
{
	return mc1Entities;
}

static bool SoundsOn()
{
	return byte_939E4_939E4 && byte_939E5_939E5;
}

void FadeInLoops_51FC0_52300()
{
	if (!SoundsOn())
	{
		memset(fadeIn_12CC20_12CC10, 0, sizeof(fadeIn_12CC20_12CC10));
		return;
	}
	for (int16_t slot = 0; slot < 32; slot++)
	{
		if (!fadeIn_12CC20_12CC10[slot])
			continue;
		bool done = false;
		if (!sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
		{
			// the high byte up by 8 (wraps within the byte)
			uint16_t& volume = sampleVolumes_CC070_CC060[slot];
			volume = (uint16_t)((volume & 0xFF) | (uint8_t)((volume >> 8) + 8) << 8);
			if (volume >= 0x7FFF)
			{
				volume = 0x7FFF;
				done = true;
			}
			const int32_t limit = (fadeInLimit_12CC40_12CC30[slot] << 8) - 1;
			if (volume >= limit)
			{
				volume = (uint16_t)limit;
				done = true;
			}
			sosDIGISetSampleVolume_66118_66628(digiDriver_CC140_CC130, slot, volume);
		}
		else
			done = true;
		if (done)
			fadeIn_12CC20_12CC10[slot] = 0;
	}
}

static void StartFadeIn_52120_52460(int16_t owner, int16_t sample)
{
	// the looped sample from the volume 0 up to fadeSpeed * 100h; already playing: only the fade out stopped
	if (!SoundsOn() || sample > (int16_t)soundCount_CC1C6_CC1B6)
		return;
	for (uint8_t slot = 0; slot < 32; slot++)
	{
		if (sampleOwners_CBFF0_CBFE0[slot][0] != (uint16_t)owner || sampleOwners_CBFF0_CBFE0[slot][1] != (uint16_t)sample
			|| sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			continue;
		if (fadeIn_12CC20_12CC10[slot])
			return;
		if (fadeOut_12CCC0_12CCB0[slot])
			fadeOut_12CCC0_12CCB0[slot] = 0;
		fadeIn_12CC20_12CC10[slot] = 1;
		fadeInLimit_12CC40_12CC30[slot] = fadeSpeed_968D8_968D8;
		return;
	}
	uint8_t slot = 0;
	for (; slot < 32; slot++)
		if (sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			break;
	if (slot == 32)
		return;
	startSample_939F0_939F0.flags_0x1C_28 = SOS_LOOPING | SOS_PANNING | SOS_VOLUME;
	startSample_939F0_939F0.volume_0x10_16 = 0;
	sampleOwners_CBFF0_CBFE0[slot][0] = owner;
	startSample_939F0_939F0.pan_0x32_50 = 0x7FFF;
	startSample_939F0_939F0.loopCount_0x0C_12 = -1;
	startSample_939F0_939F0.sampleId_0x12_18 = slot;
	sampleOwners_CBFF0_CBFE0[slot][1] = sample;
	sampleVolumes_CC070_CC060[slot] = 0;
	StartSample(sample);
	if (fadeIn_12CC20_12CC10[slot] && fadeOut_12CCC0_12CCB0[slot])
		fadeOut_12CCC0_12CCB0[slot] = 0;
	fadeIn_12CC20_12CC10[slot] = 1;
	fadeInLimit_12CC40_12CC30[slot] = fadeSpeed_968D8_968D8;
}

static void FadeInLoop_520F0_52430(int16_t owner, int16_t sample, uint8_t kind, int32_t speed)
{
	fadeSpeed_968D8_968D8 = (uint16_t)speed;
	if (kind == 3)
		StartFadeIn_52120_52460(owner, sample);
}

void FadeOutLoops_522E0_52620()
{
	if (!SoundsOn())
	{
		memset(fadeOut_12CCC0_12CCB0, 0, sizeof(fadeOut_12CCC0_12CCB0));
		return;
	}
	for (int16_t slot = 0; slot < 32; slot++)
	{
		if (!fadeOut_12CCC0_12CCB0[slot])
			continue;
		bool done = false;
		if (!sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
		{
			const int16_t volume = (int16_t)(fadeOutVolume_12CCE0_12CCD0[slot] - 0x800);
			fadeOutVolume_12CCE0_12CCD0[slot] = (uint16_t)volume;
			if (volume > 0x1000)
			{
				sampleVolumes_CC070_CC060[slot] = (uint16_t)volume;
				sosDIGISetSampleVolume_66118_66628(digiDriver_CC140_CC130, slot, (uint16_t)volume);
			}
			else
			{
				sosDIGIStopSample_65965_65E75(digiDriver_CC140_CC130, slot);
				fadeOutVolume_12CCE0_12CCD0[slot] = 0;
				sampleVolumes_CC070_CC060[slot] = 0;
				done = true;
			}
		}
		else
			done = true;
		if (done)
			fadeOut_12CCC0_12CCB0[slot] = 0;
	}
}

static void StartFadeOut_52400_52740(int16_t owner, int16_t sample, int32_t speed)
{
	if (!SoundsOn())
		return;
	for (int16_t slot = 0; slot < 32; slot++)
	{
		if (sampleOwners_CBFF0_CBFE0[slot][0] != (uint16_t)owner || sampleOwners_CBFF0_CBFE0[slot][1] != (uint16_t)sample
			|| sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			continue;
		if (fadeOut_12CCC0_12CCB0[slot])
			return;
		if (fadeIn_12CC20_12CC10[slot])
			fadeIn_12CC20_12CC10[slot] = 0;
		fadeOutSpeed_12CC80_12CC70[slot] = (uint16_t)speed;
		fadeOut_12CCC0_12CCB0[slot] = 1;
		fadeOutVolume_12CCE0_12CCD0[slot] = sampleVolumes_CC070_CC060[slot];
		return;
	}
}

// 48570: the sample of the owner on the first free slot with the volume and the pan
static void PlayPositioned_48570_488B0(int16_t owner, int16_t sample, int16_t volume, uint16_t pan)
{
	for (int16_t slot = 0; slot < 32; slot++)
	{
		if (!sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			continue;
		sampleOwners_CBFF0_CBFE0[slot][0] = owner;
		startSample_939F0_939F0.volume_0x10_16 = (uint16_t)volume;
		sampleOwners_CBFF0_CBFE0[slot][1] = sample;
		startSample_939F0_939F0.flags_0x1C_28 = SOS_PANNING | SOS_VOLUME;
		startSample_939F0_939F0.pan_0x32_50 = pan;
		startSample_939F0_939F0.sampleId_0x12_18 = slot;
		sampleVolumes_CC070_CC060[slot] = (uint16_t)(volume / 256);
		StartSample(sample);
		return;
	}
}

// 483C0: the same sample of the owner stopped, then played again
static void RestartPositioned_483C0_48700(int16_t owner, int16_t sample, int16_t volume, uint16_t pan)
{
	if (!SoundsOn() || sample > (int16_t)soundCount_CC1C6_CC1B6)
		return;
	for (int16_t slot = 0; slot < 32; slot++)
		if (sampleOwners_CBFF0_CBFE0[slot][0] == (uint16_t)owner && sampleOwners_CBFF0_CBFE0[slot][1] == (uint16_t)sample
			&& !sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
		{
			sosDIGIStopSample_65965_65E75(digiDriver_CC140_CC130, slot);
			break;
		}
	PlayPositioned_48570_488B0(owner, sample, volume, pan);
}

// 48470: played only when the same sample of the owner is not playing
static void PlayPositionedOnce_48470_487B0(int16_t owner, int16_t sample, int16_t volume, uint16_t pan)
{
	if (!SoundsOn() || sample > (int16_t)soundCount_CC1C6_CC1B6)
		return;
	for (int16_t slot = 0; slot < 32; slot++)
		if (sampleOwners_CBFF0_CBFE0[slot][0] == (uint16_t)owner && sampleOwners_CBFF0_CBFE0[slot][1] == (uint16_t)sample
			&& !sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot))
			return;
	PlayPositioned_48570_488B0(owner, sample, volume, pan);
}

static void PlayPositionedAgain_48520_48860(int16_t owner, int16_t sample, int16_t volume, uint16_t pan)
{
	if (!SoundsOn() || sample > (int16_t)soundCount_CC1C6_CC1B6)
		return;
	PlayPositioned_48570_488B0(owner, sample, volume, pan);
}

// 66250: the volume (0-80h) of the playing sample of the owner
static void SetSampleVolume_66250_66760(int16_t owner, int16_t sample, int32_t volume)
{
	if (!byte_939E4_939E4)
		return;
	for (int16_t slot = 0; slot < 32; slot++)
	{
		if (sampleOwners_CBFF0_CBFE0[slot][0] != (uint16_t)owner || sampleOwners_CBFF0_CBFE0[slot][1] != (uint16_t)sample
			|| sosDIGISampleDone_651F8_65708(digiDriver_CC140_CC130, slot) || (int16_t)volume < 0 || (int16_t)volume > 0x80)
			continue;
		sosDIGISetSampleVolume_66118_66628(digiDriver_CC140_CC130, slot, (uint16_t)(((int16_t)volume << 8) - 1));
		sampleVolumes_CC070_CC060[slot] = (uint16_t)((volume << 8) - 1);
		return;
	}
}

void PlayQueuedSounds_55100_55630()
{
	if (!SoundsOn() || (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x10000))
		return;
	FadeOutLoops_522E0_52620();
	FadeInLoops_51FC0_52300();
	for (int16_t sound = 0; sound < 47; sound++)
	{
		Type_Mc1SoundRequest& request = soundRequests_12CD20_12CD10[sound];
		switch (request.state_0x00)
		{
		case 1:
			RestartPositioned_483C0_48700((int16_t)request.host_0x06, sound, (int16_t)request.volume_0x04, request.pan_0x02);
			request.state_0x00 = 0;
			request.word_0x08 = 2;
			break;
		case 2:
			PlayPositionedAgain_48520_48860((int16_t)request.host_0x06, sound, (int16_t)request.volume_0x04, request.pan_0x02);
			request.state_0x00 = 0;
			request.word_0x08 = 2;
			break;
		case 3:
			PlayPositionedOnce_48470_487B0((int16_t)request.host_0x06, sound, (int16_t)request.volume_0x04, request.pan_0x02);
			request.state_0x00 = 0;
			request.word_0x08 = 2;
			break;
		case 4:
			if (request.volume_0x04 == 0x200)
				StopSample_5D750_5DC60(0, sound);
			else
			{
				PlaySampleLooped_5D460_5D970(0, sound, -1);
				SetSampleVolume_66250_66760(0, sound, (int16_t)(request.volume_0x04 >> 8));
			}
			request.state_0x00 = 0;
			request.word_0x08 = 2;
			break;
		}
		request.volume_0x04 = 0;
	}
}

// 55870: not quieter (by more than 8) than the request of the frame
static bool Louder_55870_55DA0(int32_t volume, int32_t other)
{
	return volume - other >= -8;
}

void QueueSound_55370_558A0(int16_t entityIndex, int32_t player, int32_t sound)
{
	// the volume and the pan from the distance and the angle to the camera (the wizard of the current player); the
	// flight sounds (1, 2, 5, 31) of the current player fade in as loops, the others wait for 55100
	if (!SoundsOn())
		return;
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Entity* entity = &SoundEntities()[entityIndex];
	uint16_t host = 0;
	int32_t volume;
	int32_t pan;
	if (entity > SoundEntities())
	{
		if (entity->flags_0x10_16 & 0x80)
			return;
		Type_Mc1Entity* camera = &SoundEntities()[state->players_0x340B_13323[state->currentPlayer_0x08_8].entityIndex_0x0A_10];
		if (Distance2DSquared_42410_42750(&camera->position_0x48_72, &entity->position_0x48_72) > 0x9000000)
			return;
		host = entity->id_0x18_24;
		const int32_t distance = (int32_t)Distance2D_423D0_42710(&camera->position_0x48_72, &entity->position_0x48_72);
		const int32_t yaw = (uint16_t)GetYawAngle_42150_42490((axis_3d*)&camera->position_0x48_72, (axis_3d*)&entity->position_0x48_72);
		uint16_t angle = (uint16_t)AngleDistance_42210_42550((uint16_t)camera->word_0x1E_30, yaw);
		// the range 12 * (400h - angle / 2): the full one ahead, a half behind
		const int32_t range = 3 * (0x400 - angle / 2) * 0x1000 / 0x400;
		volume = (range - distance) * 0x7FFF / range;
		const int32_t side = (int16_t)AngleDirection_42240_42580((uint16_t)camera->word_0x1E_30, yaw);
		if (volume < 0x200)
			return;
		if (volume > 0x7FFF)
			volume = 0x7FFF;
		if (distance <= 0x140)
			pan = 0x7FFF;
		else
		{
			if (angle > 0x200)
				angle = (uint16_t)(0x400 - angle);
			pan = (int32_t)((uint32_t)angle << 15) * side / 0x200 + 0x7FFF;
			if (pan < 0)
				pan = 0;
			if (pan > 0xFFFF)
				pan = 0xFFFF;
		}
	}
	else
	{
		volume = 0x7FFF;
		pan = 0x7FFF;
	}
	const bool current = (int16_t)player == state->currentPlayer_0x08_8;
	Type_Mc1SoundRequest* request = &soundRequests_12CD20_12CD10[(int16_t)sound];
	uint16_t queued;
	switch (sound)
	{
	case 1: case 2:
		if (current && byte_939EC_939EC != 3)
			FadeInLoop_520F0_52430(0, (int16_t)sound, 3, 0x46);
		return;
	case 5:
		if (current)
			FadeInLoop_520F0_52430(0, (int16_t)sound, 3, 0x78);
		return;
	case 31:
		if (current && byte_939EC_939EC != 3)
			FadeInLoop_520F0_52430(0, (int16_t)sound, 3, 0x55);
		return;
	case 3: case 9: case 15: case 16: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26:
	case 27: case 28: case 30: case 40: case 42: case 43: case 44: case 45:
		queued = 1;
		break;
	case 7: case 8: case 10: case 11: case 12: case 13: case 32: case 33: case 34: case 35: case 36: case 37: case 38:
	case 39: case 41:
		queued = 3;
		break;
	case 4: case 14: case 29:
	case 17:
		// the own sound without the host, a sound of all (-1) with it
		if (!Louder_55870_55DA0(volume, request->volume_0x04))
			return;
		if (current)
			host = 0;
		else if ((int16_t)player != -1)
			return;
		request->volume_0x04 = (uint16_t)volume;
		request->pan_0x02 = (uint16_t)pan;
		request->host_0x06 = host;
		request->state_0x00 = sound == 17 ? 3 : 1;
		return;
	default:
		return;
	}
	if (!Louder_55870_55DA0(volume, request->volume_0x04))
		return;
	request->volume_0x04 = (uint16_t)volume;
	request->pan_0x02 = (uint16_t)pan;
	request->host_0x06 = host;
	request->state_0x00 = queued;
}

void FadeOutSound_55890_55DC0(int16_t /*unused*/, int32_t player, int32_t sound)
{
	// the flight sounds of the current player fade out
	if (!byte_939E5_939E5 || !byte_939E4_939E4)
		return;
	const uint16_t kind = (uint16_t)sound;
	if (kind == 1 || kind == 2 || kind == 0x1F)
	{
		if (byte_939EC_939EC == 3)
			return;
	}
	else if (kind != 5)
		return;
	if ((int16_t)player != mc1State_AE400_AE3F0->currentPlayer_0x08_8)
		return;
	StartFadeOut_52400_52740(0, (int16_t)kind, 0);
}

// 20BD0: the volume (CC 7) of the MIDI channels 3-5 by the step, the timer ends at 7Eh in the danger, at 0 in the calm
static uint32_t DangerMusicTimer_20BD0_20BD0(uint32_t /*timer*/)
{
	if (!currentMusic_939D2_939D2 && dangerTimer_9008E_9008E)
	{
		ReleaseTimerEvent(dangerTimerHandle_80000_80000);
		dangerTimer_9008E_9008E = 0;
		return 0;
	}
	dangerVolume_9008C_9008C = (uint8_t)(dangerVolume_9008C_9008C + dangerStep_9008F_9008F);
	for (uint8_t channel = 3; channel <= 5; channel++)
	{
		const uint8_t message[3] = { (uint8_t)(0xB0 + channel), 7, dangerVolume_9008C_9008C };
		sosMIDISendMIDIData_5EDA0_5F2B0(midiDriver_CBFDC_CBFCC, message, 3);
	}
	if (dangerVolume_9008C_9008C == 0x7E && dangerMode_9008D_9008D == 2)
	{
		ReleaseTimerEvent(dangerTimerHandle_80000_80000);
		dangerTimer_9008E_9008E = 0;
	}
	if (!dangerVolume_9008C_9008C && dangerMode_9008D_9008D == 1)
	{
		ReleaseTimerEvent(dangerTimerHandle_80000_80000);
		dangerVolume_9008C_9008C = 0;
		dangerTimer_9008E_9008E = 0;
	}
	return 0;
}

void SetDangerMusic_20D00_20D00(uint8_t mode)
{
	// mode 2 the wizard was hit lately (pd+2Eh), 1 calm: the channels 3-5 fade in (60 Hz) or out (20 Hz); the song
	// at its end starts again calm
	if (!byte_939CC_939CC)
		return;
	if (!currentMusic_939D2_939D2 && dangerTimer_9008E_9008E)
	{
		ReleaseTimerEvent(dangerTimerHandle_80000_80000);
		dangerTimer_9008E_9008E = 0;
		return;
	}
	if (!byte_939CD_939CD || !currentMusic_939D2_939D2)
		return;
	if (sosMIDISongDone_5D7F0_5DD00(song_CBFD8_CBFC8))
	{
		if (dangerTimer_9008E_9008E)
			ReleaseTimerEvent(dangerTimerHandle_80000_80000);
		dangerVolume_9008C_9008C = 0;
		dangerStep_9008F_9008F = -2;
		dangerMode_9008D_9008D = 1;
		dangerTimer_9008E_9008E = 0;
		sosMIDIResetSong_5F529_5FA39(song_CBFD8_CBFC8, &initSong_12F030_12F020);
		sosMIDIStartSong_5F398_5F8A8(song_CBFD8_CBFC8);
		musicVolume_939D0_939D0 = 100;
		return;
	}
	if (mode == dangerMode_9008D_9008D)
		return;
	if (dangerTimer_9008E_9008E)
		ReleaseTimerEvent(dangerTimerHandle_80000_80000);
	dangerMode_9008D_9008D = mode;
	dangerTimer_9008E_9008E = 1;
	dangerStep_9008F_9008F = (int8_t)-dangerStep_9008F_9008F;
	if (mode == 1)
		sosTIMERRegisterEvent_5D97B_5DE8B(20, DangerMusicTimer_20BD0_20BD0, &dangerTimerHandle_80000_80000);
	else if (mode == 2)
		sosTIMERRegisterEvent_5D97B_5DE8B(60, DangerMusicTimer_20BD0_20BD0, &dangerTimerHandle_80000_80000);
}
