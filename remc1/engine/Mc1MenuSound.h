#pragma once

#include <cstdint>

#include "Mc1Files.h"

// MC1 menu screen 1: the sound setup (card, port, IRQ, DMA, music card, music port, save) and the control device;
// the port keeps the screen and sndsetup.inf / .dat as in the original, the sound uses the MSS driver of the remc2 config

typedef struct {
	const char* text_0;  // shown in the menu
	const char* value_4; // written to sndsetup.inf
} Type_Mc1SoundOption;

typedef struct {
	int16_t x_0;
	int16_t y_2;
	int16_t sprite_4; // gcspr.tab record
	uint16_t flags_6; // 1 / 20h analog joystick, 2 digital joystick, 4 mouse, 8 VFX1
} Type_Mc1ControlDevice;

extern uint8_t* configSpritesTab_12CAF8_12CAE8;
extern uint8_t* configSpritesTabEnd_12CAFC_12CAEC;
extern uint8_t* configSpritesData_12CB00_12CAF0;
extern uint8_t* fontTab_12CB28_12CB18;
extern uint8_t* fontTabEnd_12CB2C_12CB1C;
extern uint8_t* fontData_12CB30_12CB20;
extern bitmap_pos_struct_t* configSpritesIndex; // port: the tabs with real pointers
extern bitmap_pos_struct_t* fontIndex;
extern Type_Mc1FileEntry configFiles_49C88_49FC8[5];
extern const Type_Mc1ControlDevice controlDevices_4A8BE_4ABFE[7];

void SoundSetup_4AC70_4AFB0();
int32_t IsOptionValid_4B520_4B860();
void DrawSoundSetup_4B560_4B8A0();
const Type_Mc1SoundOption* GetSoundOptions_4B6A0_4B9E0();
void DrawSoundOptions_4B6F0_4BA30(const Type_Mc1SoundOption* options);
void DrawSoundSummary_4B8D0_4BC10(int32_t y);
void OpenSoundSetup_4BB20_4BE60();
void CloseSoundSetup_4BC90_4BFD0();
void SetControlDevice_51DF0_52130(uint16_t flags);
