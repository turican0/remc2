#pragma once

#include "Mc1GameState.h"

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// temporary stubs of functions not ported yet; each one moves to its real file when ported from IDA

void sub_4D680_4D9C0();
void sub_433C0_43700();
void InitNetwork_43270_435B0();
int16_t JoinNetworkSession_43430_43770(char* sessionName, uint16_t playerCount);

// class functions (generated, gen_classes.py)
// 45C90: the sounds of the flight (HMI)
// the network
void sub_42C70_42FB0(int16_t player);
void sub_42DE0_43120(uint32_t handle);
void sub_42D40_43080(uint32_t handle, char* name);
void sub_43150_43490();
void sub_438D0_43C10(uint16_t player, uint8_t* buffer, uint32_t size);
void sub_43870_43BB0(uint16_t player, uint8_t* buffer, uint32_t size);
