#pragma once

#include "Mc1GameState.h"

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// temporary stubs of functions not ported yet; each one moves to its real file when ported from IDA

void sub_34C80();
void sub_35040();
// the VFX1 head tracker: 5B210 reads it (vfxTrackerX / Y), 5B2B0 (int 33h 8)
int32_t sub_5B210_5B720();
void sub_5B2B0_5B7C0();
void sub_4D680_4D9C0();
void sub_4E5B0_4E8F0();
void sub_4D560_4D8A0();
void sub_433C0_43700();
void InitNetwork_43270_435B0();
int32_t InitVfx_5AF50_5B460();
void InitDigitalJoystick_3F7D0_3FB10(bitmap_pos_struct_t* sprite);
void InitAnalogJoystick_3F820(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
void InitAnalogJoystick_3FB60(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
int16_t JoinNetworkSession_43430_43770(char* sessionName, uint16_t playerCount);

// class functions (generated, gen_classes.py)
// 45C90: the sounds of the flight (HMI)
// the movie (demo): the state and the level saved / loaded, the tmaps of the movie
int32_t sub_3E350_3E690(int16_t movie);
int32_t sub_3E410_3E750(int16_t movie);
int32_t sub_3E4B0_3E7F0(int16_t movie);
int32_t sub_3E580_3E8C0(int16_t movie);
// the network
void sub_42C70_42FB0(int16_t player);
void sub_42DE0_43120(uint32_t handle);
void sub_42D40_43080(uint32_t handle, char* name);
void sub_43150_43490();
void sub_438D0_43C10(uint16_t player, uint8_t* buffer, uint32_t size);
void sub_43870_43BB0(uint16_t player, uint8_t* buffer, uint32_t size);
// 20FB0: the icons and the spells
void sub_5A560_5AA70();
// 16B00: the game loaded (3E690 the state, 3E7F0 the maps) and saved (3E750, 3E8C0)
int32_t sub_3E690_3E9D0(int16_t number);
int32_t sub_3E750_3EA90(int16_t number);
int32_t sub_3E7F0_3EB30(int16_t number);
int32_t sub_3E8C0_3EC00(int16_t number);
