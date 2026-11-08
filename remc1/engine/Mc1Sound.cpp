#include "Mc1Sound.h"

#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "../../remc2/engine/Sound.h"

uint8_t soundInitialized_90AD4_90AD4 = 0;
uint8_t hmiTimer_90AD5_90AD5 = 0;
volatile uint32_t timerTicks_AC5D4_AC5C4 = 0;
int32_t dword_9ADC0_9ADC0 = 0;
int32_t dword_9AF08_9AF08 = 0;
int32_t dword_CBFD8_CBFC8 = 0;

// port: both timers of the original (the HMI event and the own int 8 handler at 120.5 Hz) are one AIL timer
static int timerIndex = -1;

static void StartTimer()
{
	if (timerIndex != -1)
		return;
	timerIndex = AilRegisterTimer_92600(TimerTick_357A0_35B60);
	AilSetTimerFrequency_92930(timerIndex, 120);
	AilStartTimer_92BA0(timerIndex);
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
		// sosTIMERRegisterEvent (5D97B) with rate 78h and the handler 357A0
		StartTimer();
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

void InstallTimer_5A459_5A969()
{
	mc1Settings_AE408_AE3F8->timerDivisor_0xCE_206 = 0x26AF;
	mc1Settings_AE408_AE3F8->unknown_0xD2_210 = 0;
	// the original sets PIT channel 0 to the divisor (120.5 Hz) and hooks int 8 (5A3E3)
	StartTimer();
}

void RestoreTimer_5A4EA_5A9FA()
{
	// the original restores int 8 and the PIT
	if (timerIndex == -1)
		return;
	AilReleaseTimer_92DC0(timerIndex);
	timerIndex = -1;
}
