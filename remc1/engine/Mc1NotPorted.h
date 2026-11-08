#pragma once

#include <cstdint>

#include "../../remc2/portability/bitmap_pos_struct.h"

// temporary stubs of functions not ported yet; each one moves to its real file when ported from IDA

void sub_34C80();
void sub_35040();
void sub_3FCA0_3FFE0(int32_t mode, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY, int32_t stepX, int32_t stepY);
void sub_4D3B0_4D6F0();
void sub_4BD60_4C0A0();
void sub_4D680_4D9C0();
void sub_4E5B0_4E8F0();
void sub_4F1E0_4F520();
void sub_4EE70_4F1B0();
void sub_4D560_4D8A0();
void sub_65E00_66310();
void sub_34460_34820();
void FreeSoundFileGroup_61610_61B20(const char* group);
void sub_5D070_5D580(int32_t value);
void sub_5CEF0_5D400(int32_t value);
void sub_31600_31640();
void sub_407A0_40AE0();
void sub_5D290_5D7A0(int16_t variant);
void sub_59720_59C30();
void GameLoop_34610_349D0();
void sub_5D010_5D520();
void sub_20E60_20E60();
void sub_5D230_5D740(int32_t a1, int32_t a2, int16_t music);
void sub_5D460_5D970(int32_t a1, int16_t sample, int32_t a3);
void sub_5D610_5DB20(int32_t a1, int16_t sample);
void sub_5D750_5DC60(int32_t a1, int16_t sample);
int32_t sub_5D7F0_5DD00(int32_t music);
void sub_24AB0_24AB0(int16_t value);
void sub_24B80_24B80();
void sub_24BB0_24BB0(int16_t value);
void sub_24B50_24B50(const char* text);
void sub_24BF0_24BF0(uint8_t color);
void InitMusic_3C570_3C8B0();
void InitSound_3C800_3CB40();
void PrintMessage_5AD10_5B220(const char* text);
void sub_448E0_44C20();
void sub_408D0_40C10();
void sub_358A0_35C60();
void sub_58F00_59410();
void sub_433C0_43700();
void InitNetwork_43270_435B0();
int32_t InitVfx_5AF50_5B460();
void InitDigitalJoystick_3F7D0_3FB10(bitmap_pos_struct_t* sprite);
void InitAnalogJoystick_3F820(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
void InitAnalogJoystick_3FB60(bitmap_pos_struct_t* sprite1, bitmap_pos_struct_t* sprite2, bitmap_pos_struct_t* sprite3, uint8_t flags);
int16_t JoinNetworkSession_43430_43770(char* sessionName, uint16_t playerCount);
