#pragma once

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// temporary stubs of functions not ported yet; each one moves to its real file when ported from IDA

void sub_34C80();
void sub_35040();
void sub_3FCA0_3FFE0(int32_t mode, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY, int32_t stepX, int32_t stepY);
void sub_4BD60_4C0A0();
void sub_4D680_4D9C0();
void sub_4E5B0_4E8F0();
void sub_4D560_4D8A0();
void sub_34460_34820();
void sub_31600_31640();
void sub_407A0_40AE0();
void sub_59720_59C30();
void GameLoop_34610_349D0();
void sub_448E0_44C20();
void sub_408D0_40C10();
void sub_58F00_59410();
void sub_433C0_43700();
void InitNetwork_43270_435B0();
int32_t InitVfx_5AF50_5B460();
void InitDigitalJoystick_3F7D0_3FB10(bitmap_pos_struct_t* sprite);
void InitAnalogJoystick_3F820(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
void InitAnalogJoystick_3FB60(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
int16_t JoinNetworkSession_43430_43770(char* sessionName, uint16_t playerCount);
