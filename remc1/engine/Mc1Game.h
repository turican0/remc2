#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 (CARPET.EXE) and Hidden Worlds (HIDDEN.EXE) entry; names carry the MC1 and HW addresses

enum Mc1Variant : uint8_t {
	MC1_VARIANT_MC1 = 0,
	MC1_VARIANT_HW = 1,
};

extern Mc1Variant mc1Variant;
extern Type_Mc1State_AE400_AE3F0* mc1State_AE400_AE3F0;
extern Type_Mc1Settings_AE408_AE3F8* mc1Settings_AE408_AE3F8;
extern uint8_t* blurBuffer_AE404_AE3F4;
extern uint8_t* textData_AE41C_AE40C;
extern uint8_t* textPointers_AE238_AE228[80];
extern uint8_t* pointersTable_AE450_AE440;
extern uint8_t byte_939CC_939CC;
extern uint8_t byte_939CD_939CD;
extern uint8_t byte_939E4_939E4;
extern uint8_t byte_939E5_939E5;
extern uint16_t word_939D6_939D6;
extern uint8_t menuDone_96880_96880;
extern uint8_t byte_12CA7C_12CA6C;
extern int16_t videoMode_12F02E_12F01E;

int Main_404F0_40830(int argc, char** argv);
int TopProcedure_340B0(int argc, char** argv);
int TopProcedure_34470(int argc, char** argv);
void SkipMissingLevel_34070();
