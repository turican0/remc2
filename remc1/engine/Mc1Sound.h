#pragma once

#include <cstdint>

// MC1 sound setup and the 120 Hz timer; the HMI sound of the original becomes the MSS emulation of remc2
// (the driver comes from the remc2 config, not from the sound setup screen)

extern uint8_t soundInitialized_90AD4_90AD4;
extern uint8_t hmiTimer_90AD5_90AD5; // 1 when the timer is the HMI event, 0 the own int 8 handler
extern volatile uint32_t timerTicks_AC5D4_AC5C4; // 120 Hz

// HMI music state used by the movie events
extern int32_t dword_9ADC0_9ADC0;
extern int32_t dword_9AF08_9AF08;
extern int32_t dword_CBFD8_CBFC8; // the music handle

void SoundInit_357C0_35B80();
uint32_t TimerTick_357A0_35B60(uint32_t interval);
void InstallTimer_5A459_5A969();
void RestoreTimer_5A4EA_5A9FA();
