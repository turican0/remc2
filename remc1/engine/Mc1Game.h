#pragma once

#include <cstdint>

#include "Mc1GameState.h"
#include "Mc1Shared.h"

// MC1 and Hidden Worlds in one code base (mc1Variant); names carry the addresses of the original CARPET.EXE and HIDDEN.EXE

enum Mc1Variant : uint8_t {
	MC1_VARIANT_MC1 = 0,
	MC1_VARIANT_HW = 1,
};

extern Mc1Variant mc1Variant;
extern Type_Mc1State_AE400_AE3F0* mc1State_AE400_AE3F0;
extern Type_Mc1Settings_AE408_AE3F8* mc1Settings_AE408_AE3F8;
extern uint8_t* blurBuffer_AE404_AE3F4;
extern uint8_t* textPointers_AE238_AE228[80];
// the video mode is videoMode_12F02E_12F01E (Mc1Shared.h)
extern uint8_t byte_939CC_939CC;
extern uint8_t byte_939CD_939CD;
extern uint8_t byte_939E4_939E4;
extern uint8_t byte_939E5_939E5;
extern uint8_t menuDone_96880_96880;
extern uint8_t* vesaInfo_12F080_12F070;
extern uint8_t debugOption_AE45A_AE44A;
extern uint8_t inputDevices_90B23_90B23;
extern uint8_t joystickFound_B7700_B76F0;
extern uint8_t byte_A9058_A9058;

int Main_404F0_40830(int argc, char** argv);
int TopProcedure_340B0(int argc, char** argv);
int TopProcedure_34470(int argc, char** argv);
void SkipMissingLevel_34070();
int16_t ProcessCommandLine_34DD0_35190(uint16_t argc, char** argv);
void Cleanup_34C60_35020();
void PrintStartTexts_5ACA0_5B1B0();
void PrintLoadingText_5AD10_5B220(const char* text);
void LoadGameData_34B00_34EC0();
void InitGameData_34B40_34F00();
