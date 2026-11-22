#pragma once

#include "Mc1GameState.h"

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// temporary stubs of functions not ported yet; each one moves to its real file when ported from IDA

// the VFX1 head tracker: 5B210 reads it (vfxTrackerX / Y), 5B2B0 (int 33h 8)
int32_t sub_5B210_5B720();
void sub_5B2B0_5B7C0();
void sub_4D680_4D9C0();
void sub_433C0_43700();
void InitNetwork_43270_435B0();
int32_t InitVfx_5AF50_5B460();
void InitDigitalJoystick_3F7D0_3FB10(bitmap_pos_struct_t* sprite);
void InitAnalogJoystick_3F820(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
void InitAnalogJoystick_3FB60(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
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
