#pragma once

#include <cstdint>

// temporary stubs of functions not ported yet; each one moves to its real file when ported from IDA

void CreateInstallDirectory_3EC90_3EFD0(char drive, const char* basePath, const char* subDirectory);
int16_t ProcessCommandLine_34DD0_35190(uint16_t argc, char** argv);
void sub_34B40_34F00();
void sub_3F1D0();
void sub_3F510();
void sub_58F90_594A0();
void sub_41230();
void sub_41570();
void sub_44730_44A70();
void sub_4A980_4ACC0();
void sub_40440_40780(const char* path);
void sub_44700_44A40(uint8_t* text, uint8_t** pointers, int32_t count);
void FadeInOut_61CC0_621D0(uint8_t* palette, uint8_t steps, int32_t mode);
void sub_31A00_31A40();
void sub_34C80();
void sub_35040();
void sub_41490_417D0();
void sub_42530_42870(int32_t value);
void sub_4AB20();
void sub_4AE60();
void sub_5C05C_5C56C(uint8_t* table);
void sub_34460_34820();
void sub_34B00_34EC0();
void sub_61610_61B20(const char* path);
void sub_41540_41880();
void sub_5D070_5D580(int32_t value);
void sub_5CEF0_5D400(int32_t value);
void sub_31A60_31AA0();
void sub_407A0_40AE0();
void* Malloc_42540_42880(uint32_t size);
void sub_5D290_5D7A0(int16_t variant);
void sub_59720_59C30();
void GameLoop_34610_349D0();
void sub_5D010_5D520();
void sub_20E60_20E60();
void sub_448E0_44C20();
void sub_408D0_40C10();
void sub_358A0_35C60();
void sub_34C60_35020();
void sub_433C0_43700();
