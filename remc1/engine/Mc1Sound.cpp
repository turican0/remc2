#include "Mc1Sound.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Mc1Blocks.h"
#include "Mc1Game.h"
#include "Mc1Menu.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"

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

int32_t dword_9ADC0_9ADC0 = 0;
int32_t dword_9AF08_9AF08 = 0;

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

void SoundInit_357C0_35B80()
{
	if (soundInitialized_90AD4_90AD4)
		return;
	soundInitialized_90AD4_90AD4 = 1;
	if (!(mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x48))
	{
		PrintMessage_5AD10_5B220("Initialise Music");
		InitMusic_3C570_3C8B0();
		PrintMessage_5AD10_5B220("Initialise Sound");
		InitSound_3C800_3CB40();
	}
	else
	{
		PrintMessage_5AD10_5B220("Sound Disabled");
		byte_939CD_939CD = 0;
		byte_939E5_939E5 = 0;
		byte_939CC_939CC = 0;
		byte_939E4_939E4 = 0;
	}
	if (byte_939E4_939E4 || byte_939CC_939CC)
	{
		sosTIMERRegisterEvent_5D97B_5DE8B(0x78, TimerTick_357A0_35B60, &timerHandle_80010_80010);
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
	ReleaseTimerEvent(timerHandle_80010_80010); // 5DC91: sosTIMERRemoveEvent
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
		printf("\nError : %s\n", "MIDI driver");
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
		printf("\nError opening music files\n");
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
		printf("\nError : %s\n", "digital driver");
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
	// the original sets PIT channel 0 to the divisor (120.5 Hz) and hooks int 8 (5A3E3)
	if (timerHandle_80010_80010 == 0xFFFF)
		sosTIMERRegisterEvent_5D97B_5DE8B(120, TimerTick_357A0_35B60, &timerHandle_80010_80010);
}

void RestoreTimer_5A4EA_5A9FA()
{
	// the original restores int 8 and the PIT
	if (timerHandle_80010_80010 == 0xFFFF)
		return;
	ReleaseTimerEvent(timerHandle_80010_80010);
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
		printf("\nError : %s", "song");
		sosMIDIUnInitDriver_6080C_60D1C(midiDriver_CBFDC_CBFCC, true);
		byte_939CC_939CC = 0;
		return;
	}
	if (sosMIDIStartSong_5F398_5F8A8(song_CBFD8_CBFC8))
	{
		printf("\nError : %s", "song");
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
