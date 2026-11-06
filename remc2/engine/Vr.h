#pragma once

#include "Basic.h"
#include "PlayerInput.h"

extern char vrDeviceActive_E12EC;
extern __int16 vrJoystick_17D6CC[3];
int InitVrTracker_75440();
int VrSendCommand3_754C0(int a1, x_DWORD* a2, uint8_t* a3);
int VrReadData_75540(int a1, uint8_t* a2);
int VrSendCommand5_755B0(int a1, x_DWORD* a2, uint8_t* a3);
bool DetectVrDevice_75650();
signed int ShutdownVrDevice_75910();
unsigned __int8 ReadVrPort_75A10(int a1, unsigned __int8* a2);
void ResetVrDevice_75AB0();
signed int InitJoystickType3_8B600(type_unk_18058Cstr a1);
signed int ReadSerialTracker_8B790(type_unk_18058Cstr a1);
char ReadJoystickValue_8B880(uint8_t** a1, char a2, signed int a3, int a4);
char WaitForJoystickResponse_8B980(int a1, int a2, char* a3, int a4);
void CalibrateJoystick_8BA10(int a2, int* a3, char* a4, int a5);
int TimeoutMultiplyBy100_9AD9C(int a1);
int TimeoutMultiplyBy10_9ADB4(int a1);
bool HasTimeElapsed_9AE04(int a1, int a2);
char SerialReadWithTimeout_9AE90(int eax0, int edx0, char* ebx0, int* a1, int8_t* a2, int a3, int a4);
void OriginalDebugOutput_9AEEC(x_DWORD** a1, char* a2);
char SerialDebugOutput_9B038(int* a1, char* a2, int a3);
signed int SerialSendS_9B260(x_DWORD** a1);
char ReadTrackerPacket_9B274(int* a1, int a2);
char* GetTrackerStatusText_9B498(char a1);
int ReadPort_9B63C(int a1);
void SerialInterruptHandler_9B688(_DWORD* a1);
int SerialTransmit_9B7E8(_DWORD* a1);
void NoOp_9BAB0();
int SerialSetBaudRate_9BAC4(uint8_t* a1, signed int a2);
int SerialOpenPort_9BE18(uint8_t* a1, int a2, char a3, unsigned int a4, unsigned int a5);
signed int SerialSendByte_9C810(x_DWORD* a1, char a2);
signed int SerialReceiveByte_9C938(x_DWORD* a1);
bool SerialIsReceiveEmpty_9C9D0(int a1);
void SerialClearBuffer_9CBBC(x_DWORD* a1);
int SerialSetRts_9CCB4(uint8_t* a1, int a2);
int SerialSetDtr_9CCF8(uint8_t* a1, int a2);
int SerialSetLineControl_9CD9C(uint8_t* a1, int a2);
void ComputeTrackerAngles_AC250(int a1, int a2, int a3, int a4, int a5, x_DWORD* a6, x_DWORD* a7, signed int* a8);
uint8_t* InitMovingAverage_B1138(uint8_t** a1, signed int a2);
int AddMovingAverage_B11E8(uint32_t* a1, int a2);
void DrawVrFrame_BD1B6(uint8_t* a1);
