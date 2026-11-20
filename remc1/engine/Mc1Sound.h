#pragma once

#include <cstdint>

#include "Mc1Files.h"
#include "Mc1Hmi.h"

// MC1 sound (the Bullfrog layer over HMI SOS) and the 120 Hz timer; the HMI calls are done with the MSS emulation
// of remc2 (Mc1Hmi), the setup comes from the port (AWE32 music, SB16 effects)

extern uint8_t soundInitialized_90AD4_90AD4;
extern uint8_t hmiTimer_90AD5_90AD5; // 1 when the timer is the HMI event, 0 the own int 8 handler
extern uint16_t timerHandle_80010_80010; // the HMI event of the 120 Hz timer
extern volatile uint32_t timerTicks_AC5D4_AC5C4; // 120 Hz

// setup read from sndsetup.inf (port: from the setup in memory)
extern char musicDriverName_CBF74_CBF64[32];
extern uint32_t musicPort_CBF70_CBF60;
extern char soundDriverName_CC158_CC148[32];
extern uint32_t soundPort_CC14C_CC13C;
extern uint32_t soundIrq_CC150_CC140;
extern uint32_t soundDma_CC1BC_CC1AC;

// drivers and songs
extern uint16_t midiDriver_CBFDC_CBFCC;
extern uint16_t song_CBFD8_CBFC8;
extern uint16_t digiDriver_CC140_CC130;
extern uint8_t musicFile_CBFEE_CBFDE; // the second number of music%d-%d: 0 FM, 1 MT-32, 2 General MIDI / AWE32
extern uint8_t fmBanks_CBFEF_CBFDF; // inst.bnk / drum.bnk loaded
extern uint8_t secondMidiDriver_9AF9C_9AF9C;
extern const uint32_t trackMap_9AF18_9AF18[32];
extern Type_SosMidiInitSong initSong_12F030_12F020;
extern Type_SosStartSample startSample_939F0_939F0;

// banks
extern uint8_t* musicBankTab_CBF60_CBF50;
extern uint8_t* musicBankTabEnd_CBF64_CBF54;
extern uint8_t* musicBankData_CBF68_CBF58;
extern uint8_t* soundBankData_CC130_CC120;
extern uint8_t* soundBankTab_CC154_CC144;
extern uint8_t* soundBankTabEnd_CC1C0_CC1B0;
extern char musicDatPath_9AE00_9AE00[28];
extern char musicTabPath_9AE2C_9AE2C[28];
extern char soundDatPath_9AE84_9AE84[28];
extern char soundTabPath_9AEB0_9AEB0[28];
extern Type_Mc1FileEntry musicFiles_9AE00_9AE00[3];
extern Type_Mc1FileEntry soundFiles_9AE84_9AE84[3];
extern uint16_t musicVolume_939D0_939D0;
extern uint16_t currentMusic_939D2_939D2; // 0 none
extern uint16_t musicCount_939D6_939D6;
extern uint16_t soundCount_CC1C6_CC1B6;
extern uint16_t sampleOwners_CBFF0_CBFE0[32][2]; // owner, sample
extern uint16_t sampleVolumes_CC070_CC060[32];

// HMI music state used by the movie events

void StopMusic_20E60_20E60();
void SoundInit_357C0_35B80();
uint32_t TimerTick_357A0_35B60(uint32_t interval);
void ShutdownSound_358A0_35C60();
void InitMusic_3C570_3C8B0();
void InitSound_3C800_3CB40();
void InitMusicDriver_43B20_43E60(uint16_t device);
void InitDigitalDriver_43EC0_44200();
void InstallTimer_5A459_5A969();
void RestoreTimer_5A4EA_5A9FA();
int32_t LoadMusicBank_5CEF0_5D400(uint16_t bank);
void CountMusic_5CFA4_5D4B4();
void StopAllSamples_5D010_5D520();
int32_t LoadSoundBank_5D070_5D580(uint8_t bank);
void CountSounds_5D138_5D648();
void PlayMusic_5D290_5D7A0(int16_t music);
void PlaySampleLooped_5D460_5D970(int16_t owner, int16_t sample, int16_t loops);
void PlaySample_5D610_5DB20(int16_t owner, int16_t sample);
void StopSample_5D750_5DC60(int16_t owner, int16_t sample);
void ShutdownDigital_61F90_624A0();
void ShutdownMusic_62020_62530();
void RestartMenuMusic_65E00_66310();
