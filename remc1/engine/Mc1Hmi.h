#pragma once

#include <cstdint>

// port: the calls of the HMI Sound Operating System 3 (sosDIGI*, sosMIDI*, sosTIMER*) MC1 uses, done with the
// MSS emulation of remc2 the same way the HMI drivers did them: SB16.DIG for the samples, SBAWE32.MDI (AWE32
// emulation, its GM bank and the ROM) for the HMP songs; HMI loaded no AWE32 banks for MC1 either

// HMI _SOS_START_SAMPLE (32-bit HMI: far pointers of 6 bytes)
typedef struct {
	uint8_t* sample_0x00_0;   // lpSamplePtr
	uint32_t size_0x08_8;     // dwSampleSize
	int16_t loopCount_0x0C_12; // wLoopCount, -1 infinite
	uint16_t channel_0x0E_14; // wChannel
	uint16_t volume_0x10_16;  // wVolume, 7FFFh full
	uint16_t sampleId_0x12_18; // wSampleID
	uint16_t flags_0x1C_28;   // wSampleFlags
	uint16_t pan_0x32_50;     // wSamplePanLocation, 8000h the centre
} Type_SosStartSample;

// HMI _SOS_MIDI_INIT_SONG
typedef struct {
	uint8_t* songData_0x00_0;    // lpSongData, the HMP file
	void* songCallback_0x06_6;   // lpSongCallback (not used by MC1)
} Type_SosMidiInitSong;

enum {
	SOS_LOOPING = 0x4000, // _LOOPING
	SOS_VOLUME = 0x0100,  // _VOLUME
	SOS_PANNING = 0x0200, // _PANNING
	SOS_MIDI_MAP_TRACK = 0xFF, // _MIDI_MAP_TRACK, the track map of the HMP file decides
	SOS_MIDI_AWE32 = 0xA008,
};

// digital
uint16_t sosDIGIInitDriver_64846_64D56(uint16_t sampleRate, uint16_t* driverHandle);
uint16_t sosDIGIUnInitDriver_64F78_65488(uint16_t driverHandle, bool freeBuffer, bool closeDriver);
uint16_t sosDIGIStartSample_6535C_6586C(uint16_t driverHandle, Type_SosStartSample* sample);
bool sosDIGISampleDone_651F8_65708(uint16_t driverHandle, uint16_t sampleHandle);
uint16_t sosDIGIStopSample_65965_65E75(uint16_t driverHandle, uint16_t sampleHandle);
// the volume of the playing sample of the ID (wSampleID), 0Ah none
uint16_t sosDIGISetSampleVolume_66118_66628(uint16_t driverHandle, uint16_t sampleId, uint16_t volume);

// MIDI
uint16_t sosMIDIInitDriver_6035F_6086F(uint16_t deviceId, uint16_t* driverHandle);
uint16_t sosMIDIUnInitDriver_6080C_60D1C(uint16_t driverHandle, bool closeDriver);
uint16_t sosMIDIResetDriver_6091D_60E2D(uint16_t driverHandle);
uint16_t sosMIDIInitSong_5EE22_5F332(Type_SosMidiInitSong* initSong, const uint32_t* trackMap, uint16_t* songHandle);
uint16_t sosMIDIUnInitSong_5F355_5F865(uint16_t songHandle);
uint16_t sosMIDIResetSong_5F529_5FA39(uint16_t songHandle, Type_SosMidiInitSong* initSong);
uint16_t sosMIDIStartSong_5F398_5F8A8(uint16_t songHandle);
uint16_t sosMIDIStopSong_5F420_5F930(uint16_t songHandle);
bool sosMIDISongDone_5D7F0_5DD00(uint16_t songHandle);
uint16_t sosMIDIEnableChannelStealing_5EDD6_5F2E6(uint16_t enable);
// a MIDI message straight to the driver (the channel messages of MC1)
uint16_t sosMIDISendMIDIData_5EDA0_5F2B0(uint16_t driverHandle, const uint8_t* data, uint16_t length);

// timer
uint16_t sosTIMERRegisterEvent_5D97B_5DE8B(uint16_t rate, uint32_t (*event)(uint32_t), uint16_t* timerHandle);
void ReleaseTimerEvent(uint16_t timerHandle); // port: the timers of the original ended with the program
