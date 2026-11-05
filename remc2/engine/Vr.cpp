#include "Vr.h"
#include "EventsFunctions.h"

#include "CommandLineParser.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "Player.h"
#include "PlayerInput.h"
//#include "engine_support.h"
//#include "CompareMaps.h"
#include "Events.h"
#include "Level.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


#if defined(__linux__) || defined(__APPLE__)
#include <strings.h>
#include <cstdlib>
#include <cstring>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define strnicmp strncasecmp
#define __cdecl
#include <ctype.h>
#include <iostream>
#include <functional>
#include <type_traits>
#else
#endif //__linux__


#include <filesystem>


//bitmap_pos_struct_t* fontSprites_EA3D4; // weak
//bitmap_pos_struct_t fontSprites_EA3D4[1700];//fix it

/*typedef struct {
	uint8_t* pointer;
	uint32_t size;
} SpriteStr;*/

x_DWORD unk_D5621;

float flt_D1F40 = 0.000061035156; // weak

void* off_D1F78 = &unk_D5621; // weak

double dbl_D1F7C = 1.003; // weak

//2aef50
//int x_DWORD_DDF54 = 32; // weak

char vrDeviceActive_E12EC = 0; // weak

char vrDeviceDetected_E12ED = 0; // weak

int x_DWORD_E36C8 = 1; // weak

int debugSerialEnabled_E3DE8 = 0; // weak

int trackerFiltersReady_E4CCC = 0; // weak

char vrDeviceInfo_17D440[255]; // fix it -  weak

uint8_t unk_17D540[256]; // weak

int vrHandle_17D640; // weak

int vrIndex_17D644; // weak

int vrDataSize_17D648; // weak

char vrPointer1_17D650[6]; // weak

char vrPointer2_17D674[6]; // weak

int vrDataPos_17D698; // weak

int vrPortNext_17D6A0; // weak

int vrRecordSize_17D6A4; // weak

int vrIndex2_17D6A8; // weak

uint8_t* vrBufferPtr_17D6AC; // weak

int vrPort_17D6B0; // weak

char* vrRecord1_17D6B8; // weak

char* vrRecord2_17D6BC; // weak

char* vrRecord3_17D6C0; // weak

int vrMemBlock_17D6C8; // weak

__int16 vrJoystick_17D6CC[3]; // weak

uint8_t vrBuffer_17D6D4[50]; // weak

uint8_t vrData_17D70A[7]; // weak//

char byte_17D711; // weak

//----- (00075420) --------------------------------------------------------
/*int sub_75420()//256420
{
	return dos_setvect(9, x_DWORD_17D430, (unsigned __int16)x_WORD_17D434);
}*/
// 99BDB: using guessed type x_DWORD dos_setvect(x_DWORD, x_DWORD, x_DWORD);
// 17D430: using guessed type int x_DWORD_17D430;
// 17D434: using guessed type __int16 x_WORD_17D434;

//----- (00075440) --------------------------------------------------------
int InitVrTracker_75440()
{
	signed int v0; // ebx
	int v1; // esi

	v0 = 96;
	do
	{
		v1 = v0 << 8;
		LOWORD(v1) = ((x_WORD)v0 << 8) | 0x7F;
		memset(vrBuffer_17D6D4, 0, 50);
		*(int32_t*)&vrBuffer_17D6D4[28] = v1;
		*(int32_t*)&vrBuffer_17D6D4[16] = 0;
		*(int32_t*)&vrBuffer_17D6D4[24] = 0;
		*(int32_t*)&vrBuffer_17D6D4[20] = 0;
		//sub_75AE0((int)vrBuffer_17D6D4);
		if ((v0 | 0x7F00) == *(int32_t*)&vrBuffer_17D6D4[28])
			return v1 & 0xFF00;
		++v0;
	} while (v0 <= 112);
	return 0;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 17D6E4: using guessed type int x_DWORD_17D6E4;
// 17D6E8: using guessed type int x_DWORD_17D6E8;
// 17D6EC: using guessed type int x_DWORD_17D6EC;
// 17D6F0: using guessed type int x_DWORD_17D6F0;

//----- (000754C0) --------------------------------------------------------
int VrSendCommand3_754C0(int a1, x_DWORD* a2, uint8_t* a3)
{
	int v3; // eax
	int v4; // ecx
	int i; // eax

	memset(vrBuffer_17D6D4, 0, 50);
	v3 = a1;
	LOBYTE(v3) = a1 | 3;
	*(int32_t*)&vrBuffer_17D6D4[24] = 0;
	*(int32_t*)&vrBuffer_17D6D4[28] = v3;
	*(int32_t*)&vrBuffer_17D6D4[20] = 0;
	*(int16_t*)&vrBuffer_17D6D4[34] = *(int16_t*)&vrBuffer_17D6D4[28];
	//sub_75AE0((int)vrBuffer_17D6D4);
	v4 = vrMemBlock_17D6C8;
	for (i = 0; i < *(int32_t*)&vrBuffer_17D6D4[24]; *(x_BYTE*)(a3 + i - 1) = *(x_BYTE*)(v4 + i - 1))
		++i;
	*a2 = *(int32_t*)&vrBuffer_17D6D4[24];
	return *(int32_t*)&vrBuffer_17D6D4[28];
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 17D6C8: using guessed type int vrMemBlock_17D6C8;
// 17D6E8: using guessed type int x_DWORD_17D6E8;
// 17D6EC: using guessed type int x_DWORD_17D6EC;
// 17D6F0: using guessed type int x_DWORD_17D6F0;
// 17D6F6: using guessed type __int16 x_WORD_17D6F6;
// 17D708: using guessed type __int16 x_WORD_17D708;

//----- (00075540) --------------------------------------------------------
int VrReadData_75540(int a1, uint8_t* a2)
{
	int v2; // ecx
	signed int v3; // eax
	int v4; // eax

	v2 = vrMemBlock_17D6C8;
	v3 = 0;
	do
	{
		++v3;
		*(x_BYTE*)(v2 + v3 - 1) = *(x_BYTE*)(a2 + v3 - 1);
	} while (v3 < 256);
	memset(vrBuffer_17D6D4, 0, 50);
	v4 = a1;
	LOBYTE(v4) = a1 | 4;
	*(int32_t*)&vrBuffer_17D6D4[24] = 0;
	*(int32_t*)&vrBuffer_17D6D4[28] = v4;
	*(int32_t*)&vrBuffer_17D6D4[20] = 0;
	*(int16_t*)&vrBuffer_17D6D4[34] = *(int16_t*)&vrBuffer_17D6D4[28];
	//sub_75AE0((int)vrBuffer_17D6D4);
	return *(int32_t*)&vrBuffer_17D6D4[28];
}

//----- (000755B0) --------------------------------------------------------
int VrSendCommand5_755B0(int a1, x_DWORD* a2, uint8_t* a3)
{
	int v3; // eax
	int v4; // ecx
	int i; // eax

	memset(vrBuffer_17D6D4, 0, 50);
	v3 = a1;
	LOBYTE(v3) = a1 | 5;
	*(int32_t*)&vrBuffer_17D6D4[24] = 0;
	*(int32_t*)&vrBuffer_17D6D4[28] = v3;
	*(int32_t*)&vrBuffer_17D6D4[20] = 0;
	*(int16_t*)&vrBuffer_17D6D4[34] = *(int16_t*)&vrBuffer_17D6D4[28];
	//sub_75AE0((int)vrBuffer_17D6D4);
	v4 = vrMemBlock_17D6C8;
	for (i = 0; i < *(int32_t*)&vrBuffer_17D6D4[24]; *(x_BYTE*)(a3 + i - 1) = *(x_BYTE*)(v4 + i - 1))
		++i;
	*a2 = *(int32_t*)&vrBuffer_17D6D4[24];
	return *(int32_t*)&vrBuffer_17D6D4[28];
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 17D6C8: using guessed type int vrMemBlock_17D6C8;
// 17D6E8: using guessed type int x_DWORD_17D6E8;
// 17D6EC: using guessed type int x_DWORD_17D6EC;
// 17D6F0: using guessed type int x_DWORD_17D6F0;
// 17D6F6: using guessed type __int16 x_WORD_17D6F6;
// 17D708: using guessed type __int16 x_WORD_17D708;

//----- (00075650) --------------------------------------------------------
bool DetectVrDevice_75650()//VR something
{
	const char* v1; // eax
	int v2; // eax
	char* v3; // eax
	int v4; // ebx
	int v5; // edx
	uint8_t* v6; // esi
	char v7; // dh
	char v8; // bh
	uint8_t* v9; // ecx
	int v10; // edx
	int v11; // ebx
	char v12; // dl
	char v13; // bl
	char v14; // dh
	unsigned __int8 v15[16]; // [esp+0h] [ebp-4h] BYREF

	uint16 __DS__ = 0;

	memset(vrJoystick_17D6CC, 0, 6);
	memset(vrData_17D70A, 0, 7);
	vrMemBlock_17D6C8 = AllocMemBlock_75B80(256, (int16_t*)&vrBuffer_17D6D4[28], (int16_t*)&vrBuffer_17D6D4[50]);
	if (!vrMemBlock_17D6C8)
		return false;
	v1 = mygetenv("VIPPORT");
	if (v1)
	{
		vrPort_17D6B0 = StringToLong2_99FF0((char*)v1, (char**)&v15, 16);
		vrPortNext_17D6A0 = vrPort_17D6B0 + 1;
		v2 = InitVrTracker_75440();
		vrHandle_17D640 = v2;
		if (!v2 || VrSendCommand3_754C0(v2, &vrDataSize_17D648, (uint8_t*)vrDeviceInfo_17D440))
		{
			ResetVrDevice_75AB0();
			return false;
		}
		vrIndex_17D644 = 0;
		vrIndex2_17D6A8 = 0;
		vrDataPos_17D698 = 0;
		vrBufferPtr_17D6AC = unk_17D540;
		while (vrDataPos_17D698 < vrDataSize_17D648)
		{
			vrRecordSize_17D6A4 = vrDeviceInfo_17D440[vrDataPos_17D698];
			v3 = &vrDeviceInfo_17D440[vrDataPos_17D698];
			switch (vrDeviceInfo_17D440[vrDataPos_17D698 + 1])
			{
			case 0:
				vrDataPos_17D698 = vrDataSize_17D648;
				break;
			case 1:
				vrRecord3_17D6C0 = &vrDeviceInfo_17D440[vrDataPos_17D698];
				break;
			case 2:
				vrRecord1_17D6B8 = &vrDeviceInfo_17D440[vrDataPos_17D698];
				byte_17D711 = strcmp(v3 + 34, "VFX1 CyberPuck") == 0;
				break;
			case 6:
				if (vrIndex2_17D6A8 < 6)
				{
					v4 = vrIndex2_17D6A8;
					v5 = 3 * vrIndex2_17D6A8;
					v6 = vrBufferPtr_17D6AC;
					vrRecord4_17D6C4 = &vrDeviceInfo_17D440[vrDataPos_17D698];
					*(_WORD*)&vrPointer2_17D674[2 * v5 + 4] = __DS__;
					*(uint8_t**)&vrPointer2_17D674[2 * v5] = v6;
					vrIndex2_17D6A8 = (int)(v4 + 1);
					vrBufferPtr_17D6AC = v6 + 6;
					if (byte_17D711)
					{
						v7 = v3[14];
						v3[22] |= 0x80u;
						v8 = v3[30];
						v3[14] = v7 | 0x80;
						v3[30] = v8 | 0x80;
					}
				}
				break;
			case 7:
				if (vrIndex_17D644 < 6)
				{
					v9 = vrBufferPtr_17D6AC;
					v10 = 3 * vrIndex_17D644;
					vrRecord2_17D6BC = &vrDeviceInfo_17D440[vrDataPos_17D698];
					v11 = vrIndex_17D644 + 1;
					*(uint8_t**)&vrPointer1_17D650[2 * v10] = vrBufferPtr_17D6AC;
					*(_WORD*)&vrPointer1_17D650[2 * v10 + 4] = __DS__;
					vrBufferPtr_17D6AC = v9 + 7;
					vrIndex_17D644 = v11;
					if (byte_17D711)
					{
						v12 = v3[22];
						v3[14] |= 0x80u;
						v13 = v3[33];
						v3[22] = v12 | 0x80;
						v14 = v3[30];
						v3[33] = v13 | 0x80;
						v3[30] = v14 | 0x80;
					}
				}
				break;
			default:
				break;
			}
			vrDataPos_17D698 += vrRecordSize_17D6A4;
		}
		if (VrReadData_75540(vrHandle_17D640, (uint8_t*)vrDeviceInfo_17D440))
		{
		LABEL_22:
			ResetVrDevice_75AB0();
			return false;
		}
		else
		{
			if (vrIndex2_17D6A8)
				vrDeviceActive_E12EC = 1;
			if (vrIndex_17D644)
				vrDeviceDetected_E12ED = 1;
			return true;
		}
	}
	else
	{
		vrPort_17D6B0 = 768;
		ResetVrDevice_75AB0();
		return false;
	}
}

//----- (00075910) --------------------------------------------------------
signed int ShutdownVrDevice_75910()
{
	signed int result = 0; // eax
	unsigned __int16 v1; // ax
	unsigned __int8 v2; // al

	if (VrSendCommand5_755B0(vrHandle_17D640, &vrDataSize_17D648, unk_17D540))
	{
		ResetVrDevice_75AB0();
		result = 0;
	}
	else
	{
		if (vrDeviceActive_E12EC)
		{
			//__GS__ = *(x_WORD *)&vrPointer2_17D674[4];
			vrJoystick_17D6CC[0] = __readgsx_WORD(*(unsigned int*)vrPointer2_17D674);
			v1 = __readgsx_WORD(*(x_DWORD*)vrPointer2_17D674 + 4);
			vrJoystick_17D6CC[1] = __readgsx_WORD(*(x_DWORD*)vrPointer2_17D674 + 2);
			vrJoystick_17D6CC[2] = v1;
		}
		if (vrDeviceDetected_E12ED)
		{
			//__GS__ = *(x_WORD *)&vrPointer1_17D650[4];
			*(int16_t*)&vrData_17D70A[0] = __readgsx_WORD(*(unsigned int*)vrPointer1_17D650);
			*(int16_t*)&vrData_17D70A[2] = __readgsx_WORD(*(x_DWORD*)vrPointer1_17D650 + 2);
			v2 = __readgsx_BYTE(*(x_DWORD*)vrPointer1_17D650 + 6);
			*(int16_t*)&vrData_17D70A[4] = __readgsx_WORD(*(x_DWORD*)vrPointer1_17D650 + 4);
			vrData_17D70A[6] = v2;
		}
		result = 1;
	}
	return result;
}

//----- (00075A10) --------------------------------------------------------
unsigned __int8 ReadVrPort_75A10(int a1, unsigned __int8* a2)//256a10 // fix
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	unsigned __int8 v2; // al
	unsigned __int8* v3; // ebx
	char v4; // dl
	//unsigned __int8 v5; // al
	signed int v6; // ecx
	//unsigned __int8 v7; // al
	unsigned __int8* v8; // ebx
	unsigned __int8 v9; // al
	unsigned __int8 result; // al

	v2 = 0;//fix

	/*__outx_BYTE(vrPort_17D6B0, 7u);
	__outx_BYTE(vrPortNext_17D6A0, 0xFFu);
	__outx_BYTE(vrPort_17D6B0, 6u);
	v2 = __inx_BYTE(vrPortNext_17D6A0);*/
	v3 = a2;
	v4 = v2 & 0xF8;
	//if (a1)
	//{
	//	if (a1 == 1)
	//		v5 = v4 | 1;
	//	else
	//		v5 = (v2 | 4) & 0xFE;
	//}
	//else
	//{
	//	v5 = v4 | 3;
	//}
	/*__outx_BYTE(vrPortNext_17D6A0, v5);
	__outx_BYTE(vrPort_17D6B0, 9u);
	__outx_BYTE(vrPortNext_17D6A0, 0);*/
	v6 = 0;
	//__outx_BYTE(vrPort_17D6B0, 0xAu);
	do
	{
		//v7 = *v3;
		v8 = v3 + 1;
		//__outx_BYTE(vrPortNext_17D6A0, v7);
		v9 = *v8++;
		//__outx_BYTE(vrPortNext_17D6A0, v9);
		++v6;
		result = *v8;
		v3 = v8 + 1;
		//__outx_BYTE(vrPortNext_17D6A0, result);
	} while (v6 < 256);
	return result;
}
// 17D6A0: using guessed type int vrPortNext_17D6A0;
// 17D6B0: using guessed type int vrPort_17D6B0;

//----- (00075AB0) --------------------------------------------------------
void ResetVrDevice_75AB0()
{
	//int result; // eax

	if (vrMemBlock_17D6C8)
	{
		//removed sub_75B50(*(int16_t*)&vrBuffer_17D6D4[50]);
		vrMemBlock_17D6C8 = 0;
	}
	//return result;
}
// 17D6C8: using guessed type int vrMemBlock_17D6C8;
// 17D706: using guessed type __int16 x_WORD_17D706;

//----- (0008B600) --------------------------------------------------------
signed int InitJoystickType3_8B600(type_unk_18058Cstr a1)//26c600
{
	//int v1; // edi
	//char v2; // bl
	//__int16 v3; // dx
	//char v4; // al
	//int v5; // edx
	//int v6; // eax
	//int v7; // eax
	//char v9x[60]; // [esp+0h] [ebp-40h]
	//char v10; // [esp+3Ch] [ebp-4h]

	//fix it
	//v5 = 0;
	//fix it
	/*
	v1 = TimeoutMultiplyBy100_9AD9C(3);
	memset(v9x, 0, 60);
	memset(a1.unk_1805CE, 0, 84);
	v2 = 1;
	switch (a1.x_WORD_1805C6_vio2_analog_joy)
	{
	case 1:
		v2 = 0;
		break;
	case 3:
		v2 = 2;
		break;
	case 4:
		v2 = 3;
		break;
	default:
		break;
	}
	//v3 = a1.x_WORD_1805C4_vio1_analog_joy;
	switch (a1.x_WORD_1805C4_vio1_analog_joy)
	{
	case 1200:
		v4 = ReadJoystickValue_8B880(a1.unk_1805CE, v2, 1200, v1);
		break;
	case 2400:
		v4 = ReadJoystickValue_8B880(a1.unk_1805CE, v2, 2400, v1);
		break;
	case 4800:
		v4 = ReadJoystickValue_8B880(a1.unk_1805CE, v2, 4800, v1);
		break;
	case 9600:
		v4 = ReadJoystickValue_8B880(a1.unk_1805CE, v2, 9600, v1);
		break;
	case 19200:
		v4 = ReadJoystickValue_8B880(a1.unk_1805CE, v2, 19200, v1);
		break;
	default:
		goto LABEL_16;
	}
	v10 = v4;
LABEL_16:
	GetTrackerStatusText_9B498(v10);
	myprintf("%s.\n");
	CalibrateJoystick_8BA10( v5, a1.unk_1805CE, v9x, v1);
	myprintf(v9x);
	myprintf("\n");
	v6 = TimeoutMultiplyBy100_9AD9C(2);
	SerialDebugOutput_9B038(a1.unk_1805CE, (char*)&off_D1F10, v6);
	v7 = TimeoutMultiplyBy100_9AD9C(2);
	if (!SerialDebugOutput_9B038(a1.unk_1805CE, (char*)"!M1,P,B\r", v7))
		return 1;
	FreeAndClear_9B234(a1.unk_1805CE);
	myprintf("Could not send Info to Tracker\n");
	*/
	return -1;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D1F10: using guessed type void *off_D1F10;

//----- (0008B790) --------------------------------------------------------
signed int ReadSerialTracker_8B790(type_unk_18058Cstr a1)//26c790
{
	int v1; // eax
	signed int v2; // esi
	signed int v3; // eax
	double v4; // st7
	double v5; // st7
	int v6; // eax
	double v7; // st7
	int v8; // eax
	int v9; // ecx
	int v10; // edi
	int v11; // eax
	int v12; // ecx
	int v13; // eax
	int v14; // edx

	//fix it
	v6 = 0;
	v8 = 0;
	//fix it

	v1 = TimeoutMultiplyBy100_9AD9C(1);
	v2 = 1;
	if (ReadTrackerPacket_9B274((x_DWORD*)((char*)a1.serialBuffer_1805CE), v1 / 2))
		v2 = -1;
	v3 = SerialSendS_9B260((x_DWORD**)((char*)a1.serialBuffer_1805CE));
	v4 = (double)*(signed int*)((char*)&a1.serialBuffer_1805CE[10]) * flt_D1F40;
	_CHP(v3);
	a1.joyAxisX_180590 = (signed int)v4;
	v5 = (double)*(signed int*)((char*)&a1.serialBuffer_1805CE[6]) * flt_D1F40;
	_CHP(v6);
	a1.joyAxisY_180594 = (signed int)v5;
	v7 = (double)*(signed int*)((char*)&a1.serialBuffer_1805CE[14]) * flt_D1F40;
	_CHP(v8);
	a1.joyAxisZ_180598 = (signed int)v7;
	if (LastPressedKey_1806E4 == 0x2e)
	{
		inputState_18058C.joyCenterX_1805A4 = inputState_18058C.joyAxisX_180590;
		inputState_18058C.joyCenterY_1805A8 = inputState_18058C.joyAxisY_180594;
		inputState_18058C.joyCenterZ_1805AC = inputState_18058C.joyAxisZ_180598;
	}
	else
	{
		v9 = a1.joyAxisY_180594;
		v10 = a1.joyAxisZ_180598;
		v11 = a1.joyCenterY_1805A8;
		a1.joyAxisX_180590 -= a1.joyCenterX_1805A4;
		v12 = v9 - v11;
		v13 = a1.joyCenterZ_1805AC;
		a1.joyAxisY_180594 = v12;
		a1.joyAxisZ_180598 = v10 - v13;
	}
	v14 = -a1.joyAxisY_180594;
	a1.joyAxisX_180590 = -a1.joyAxisX_180590;
	a1.joyAxisY_180594 = v14;
	return v2;
}
// 9B4F6: using guessed type double /*__fastcall*/ _CHP(x_DWORD);
// D1F40: using guessed type float flt_D1F40;
// 180590: using guessed type int x_DWORD_180590;
// 180594: using guessed type int x_DWORD_180594;
// 180598: using guessed type int x_DWORD_180598;
// 1805A4: using guessed type int x_DWORD_1805A4;
// 1805A8: using guessed type int x_DWORD_1805A8;
// 1805AC: using guessed type int x_DWORD_1805AC;
// 1806E4: using guessed type char x_BYTE_1806E4;

//----- (0008B880) --------------------------------------------------------
char ReadJoystickValue_8B880(uint8_t** a1, char a2, signed int a3, int a4)//26c880
{
	int v4; // esi
	char result; // al
	uint8_t* v6; // eax
	int v7; // eax
	int v8; // eax
	int v9; // edx

	//fix it
	v9 = 0;
	//fix it

	switch (a2)
	{
	case 0:
		v4 = 0;
		goto LABEL_7;
	case 1:
		v4 = 1;
		goto LABEL_7;
	case 2:
		v4 = 2;
		goto LABEL_7;
	case 3:
		v4 = 3;
	LABEL_7:
		v6 = (uint8_t*)calloc(1, 90);
		a1[0] = v6;
		if (v6)
		{
			v7 = SerialOpenPort_9BE18(v6, v4, 1, 0x400u, 0x400u);
			if (v7)
			{
				if (v7 == -1)
				{
					myprintf("NO SERIAL\n");
					result = 1;
				}
				else
				{
					myprintf("NO MEMORY\n");
					result = 9;
				}
			}
			else
			{
				SerialSetBaudRate_9BAC4(a1[0], a3);
				SerialSetDtr_9CCF8(a1[0], 1);
				SerialSetLineControl_9CD9C(a1[0], 1);
				v8 = SerialSetRts_9CCB4(a1[0], 0);
				result = WaitForJoystickResponse_8B980(v8, v9, (char*)a1, a4);
			}
		}
		else
		{
			myprintf("No memory for serial data.\n");
			result = 1;
		}
		break;
	default:
		result = 1;
		break;
	}
	return result;
}
// 9B513: using guessed type x_DWORD calloc(x_DWORD, x_DWORD);

//----- (0008B980) --------------------------------------------------------
char WaitForJoystickResponse_8B980(int a1, int a2, char* a3, int a4)//26c980
{
	//int v4; // edi
	//bool v5; // eax
	//int v6; // edx
	//int v7; // esi
	//int v9; // eax
	//int v10; // edx
	//int v11; // eax

	//fix it
	//v5 = 0;
	//v6 = 0;
	//v10 = 0;
	//fix it
	/*
	v4 = j_j___clock(a1, a2, a3);
	SerialClearBuffer_9CBBC(*a3);
	do
	{
		v7 = j_j___clock(v5, v6, a3);
		sub_9AEEC(a3, (char*)&off_D1F10);
		do
		{
			if (!SerialIsReceiveEmpty_9C9D0(*a3) && SerialReceiveByte_9C938(*a3) == 79)
				return 0;
			v9 = TimeoutMultiplyBy10_9ADB4(1);
			v11 = HasTimeElapsed_9AE04(v7, v9);
		} while (!v11);
		v5 = HasTimeElapsed_9AE04(v4, a4);
	} while (!v5);
	*/
	return 3;
}
// 9ADFC: using guessed type int /*__fastcall*/ j_j___clock(x_DWORD, x_DWORD, x_DWORD);
// D1F10: using guessed type void *off_D1F10;

//----- (0008BA10) --------------------------------------------------------
void CalibrateJoystick_8BA10(int a2, int* a3, char* a4, int a5)//26ca10
{
	int v5; // esi
	bool v6; // eax
	int v7; // edx
	int v8; // eax
	int v9; // edx
	char v10; // ah
	unsigned int i; // edx
	char v12; // cl
	double v13; // rax
	int v15; // eax
	//int v16; // edx
	int v17; // eax
	int v18; // [esp+8h] [ebp-4h]

	//fix it
	int a1 = 0;
	v6 = 0;
	v7 = 0;
	v8 = 0;
	v9 = 0;
	//v16 = 0;
	//fix it

	v5 = j_j___clock(a1, a2, a4);
	SerialClearBuffer_9CBBC((x_DWORD*)*a3);
	while (2)
	{
		v18 = j_j___clock(v6, v7, a4);
		OriginalDebugOutput_9AEEC((x_DWORD**)a3, (char*)&off_D1F78);
		do
		{
			OriginalDebugOutput_9AEEC((x_DWORD**)a3, (char*)&off_D1F78);
			SerialReadWithTimeout_9AE90(v8, v9, a4, a3, (int8_t*)a4, 60, a5);
			v10 = *a4;
			a4[59] = 0;
			if (v10 == 77)
			{
				for (i = 0; i < strlen(a4); i++)
				{
					v12 = a4[i];
					if (v12 == 70 || v12 == 102)
					{
						LODWORD(v13) = strtod(&a4[i + 1], 0);
						if (v13 < dbl_D1F7C)
							x_DWORD_E36C8 = 0;
						return;
					}
				}
				return;
			}
			v15 = TimeoutMultiplyBy10_9ADB4(1);
			v17 = HasTimeElapsed_9AE04(v18, v15);
		} while (!v17);
		v6 = HasTimeElapsed_9AE04(v5, a5);
		if (!v6)
			continue;
		break;
	}
}

//----- (0009AD9C) --------------------------------------------------------
int TimeoutMultiplyBy100_9AD9C(int a1)//27Bd9c
{
	return 100 * a1;
}

//----- (0009ADB4) --------------------------------------------------------
int TimeoutMultiplyBy10_9ADB4(int a1)//27Bdb4
{
	return 100 * a1 / 10;
}

//----- (0009AE04) --------------------------------------------------------
bool HasTimeElapsed_9AE04(int a1, int a2)//27Be04
{
	return j___clock() - a1 >= a2;
}
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);

//----- (0009AE90) --------------------------------------------------------
char SerialReadWithTimeout_9AE90(int eax0, int edx0, char* ebx0, int* a1, int8_t* a2, int a3, int a4)//27Be90
{
	int8_t* v7; // esi
	int v8; // ebp
	int v9; // ebx
	int v10; // eax

	v7 = a2;
	v8 = j_j___clock(eax0, edx0, ebx0);
	v9 = 0;
	do
	{
		v10 = SerialIsReceiveEmpty_9C9D0(*a1);
		if (!v10)
		{
			v9++;
			*v7 = SerialReceiveByte_9C938((int32_t*)*a1);
			v10 = a3;
			v7++;
			if (v9 == a3)
				return 0;
		}
	} while (!HasTimeElapsed_9AE04(v8, a4));
	return 3;
}
// 9ADFC: using guessed type int /*__fastcall*/ j_j___clock(x_DWORD, x_DWORD, x_DWORD);

//----- (0009AEEC) --------------------------------------------------------
void OriginalDebugOutput_9AEEC(x_DWORD** a1, char* a2)//27Beec
{
	char* v2; // ebx
	char v4; // [esp-4h] [ebp-Ch]

	v2 = (char*)a2;
	if (debugSerialEnabled_E3DE8)
		Logger->debug("Writing %s\n", a2);
	while (*v2)
	{
		v4 = *v2++;
		SerialSendByte_9C810(*a1, v4);
	}
}

//----- (0009AFC4) --------------------------------------------------------
char SerialWaitForByte_9AFC4(int a1, int a2, char* a3, int* a4, int a5)//27Bfc4
{
	int v5; // edi
	int v6; // eax
	//int v7; // edx

	//fix it
	//v7 = 0;
	//fix it

	v5 = j_j___clock(a1, a2, a3);
	do
	{
		v6 = SerialIsReceiveEmpty_9C9D0(*a4);
		if (!v6)
		{
			v6 = SerialReceiveByte_9C938((x_DWORD*)*a4);
			//a3 = v6;
			if (debugSerialEnabled_E3DE8)
				/*v6 = */myprintf("Response: %c\n");
			if (v6 == 79)
				return 0;
			if (v6 == 69)
				return 8;
		}
	} while (!HasTimeElapsed_9AE04(v5, a5));
	return 3;
}
// 9ADFC: using guessed type int /*__fastcall*/ j_j___clock(x_DWORD, x_DWORD, x_DWORD);
// E3DE8: using guessed type int debugSerialEnabled_E3DE8;

//----- (0009B038) --------------------------------------------------------
char SerialDebugOutput_9B038(int* a1, char* a2, int a3)//27C038
{
	//int v3; // eax
	//int v4; // edx

	//fix it
	//v3 = 0;
	//v4 = 0;
	//fix it

	OriginalDebugOutput_9AEEC((x_DWORD**)a1, a2);
	//return SerialWaitForByte_9AFC4(v3, v4, a1, a1, a3);
	return 0;
}

//----- (0009B260) --------------------------------------------------------
signed int SerialSendS_9B260(x_DWORD** a1)//27C260
{
	return SerialSendByte_9C810(*a1, 83);
}

//----- (0009B274) --------------------------------------------------------
char ReadTrackerPacket_9B274(int* a1, int a2)//27C274
{
	char result = 0; // al
	//int v3; // ebp
	//int v4; // esi
	//char v5; // dl
	//int v6; // edi
	//signed int v7; // eax
	//char v8; // dh
	//char v9; // [esp+0h] [ebp-44h]
	//unsigned __int8 v10; // [esp+1h] [ebp-43h]
	//unsigned __int8 v11; // [esp+2h] [ebp-42h]
	//unsigned __int8 v12; // [esp+3h] [ebp-41h]
	//unsigned __int8 v13; // [esp+4h] [ebp-40h]
	//unsigned __int8 v14; // [esp+5h] [ebp-3Fh]
	//unsigned __int8 v15; // [esp+6h] [ebp-3Eh]
	//unsigned __int8 v16; // [esp+7h] [ebp-3Dh]
	//unsigned __int8 v17; // [esp+8h] [ebp-3Ch]
	//unsigned __int8 v18; // [esp+9h] [ebp-3Bh]
	//unsigned __int8 v19; // [esp+Ah] [ebp-3Ah]
	//char v20; // [esp+Bh] [ebp-39h]
	//int v21; // [esp+20h] [ebp-24h]
	//int v22; // [esp+24h] [ebp-20h]
	//int v23; // [esp+28h] [ebp-1Ch]
	//int v24; // [esp+2Ch] [ebp-18h]
	//int v25; // [esp+30h] [ebp-14h]

	//fix it
	//v10 = 0;
	//v11 = 0;
	//v12 = 0;
	//v13 = 0;
	//v14 = 0;
	//v15 = 0;
	//v16 = 0;
	//v17 = 0;
	//v18 = 0;
	//v19 = 0;
	//v20 = 0;
	//fix it

	/*
	if (SerialReadWithTimeout_9AE90(&v9, a2, a1, a1, (int8_t*)&v9, 12, a2))
	{
		SerialClearBuffer_9CBBC((x_DWORD*)*a1);
		result = 3;
	}
	else
	{
		v3 = (signed __int16)(v10 << 8) | v11;
		v24 = v13 | (signed __int16)(v12 << 8);
		v25 = v15 | (signed __int16)(v14 << 8);
		v4 = v17 | (signed __int16)(v16 << 8);
		v5 = 0;
		v6 = (signed __int16)(v18 << 8) | v19;
		v7 = 0;
		do
		{
			v8 = *(&v9 + v7++);
			v5 += v8;
		} while (v7 < 11);
		if (v5 == v20)
		{
			ComputeTrackerAngles_AC250(
				(signed __int16)(v10 << 8) | v11,
				v24,
				v25,
				v17 | (signed __int16)(v16 << 8),
				(signed __int16)(v18 << 8) | v19,
				&v23,
				&v22,
				&v21);
			a1[1] = v3;
			a1[4] = v4;
			a1[5] = v6;
			a1[2] = v24;
			a1[3] = v25;
			a1[6] = v22;
			a1[7] = v23;
			a1[8] = v21;
			result = 0;
		}
		else
		{
			SerialClearBuffer_9CBBC((x_DWORD*)*a1);
			result = 4;
		}
	}
	*/
	return result;
}

//----- (0009B498) --------------------------------------------------------
char* GetTrackerStatusText_9B498(char a1)//27C498
{
	char* result; // eax

	switch (a1)
	{
	case 0:
		result = (char*)"Tracker initialized ok";
		break;
	case 1:
		result = (char*)"No serial port";
		break;
	case 2:
		result = (char*)"Tracker not present";
		break;
	case 3:
		result = (char*)"Tracker is not responding";
		break;
	case 4:
		result = (char*)"Tracker data is invalid";
		break;
	case 5:
		result = (char*)"Tracker header data is invalid";
		break;
	case 6:
		result = (char*)"A verified write to tracker memory failed";
		break;
	case 7:
		result = (char*)"Invalid parameter";
		break;
	case 8:
		result = (char*)"Tracker command returned an error";
		break;
	case 9:
		result = (char*)"A memory allocation failed";
		break;
	case 10:
		result = (char*)"Tracker version string is invalid";
		break;
	default:
		result = (char*)"Undefined";
		break;
	}
	return result;
}

//----- (0009B540) --------------------------------------------------------
// bool sub_9B540_lock_linear_mem_region(unsigned int a1, unsigned int a2)//27C540 NOTE: removed (DOS memory handling)

//----- (0009B5B4) --------------------------------------------------------
// bool sub_9B5B4_unlock_mem_region(unsigned int a1, unsigned int a2)//27C5B4 NOTE: removed (DOS memory handling)

//----- (0009B628) --------------------------------------------------------
// void sub_9B628()//27C628 NOTE: removed (empty function)

//----- (0009B63C) --------------------------------------------------------
int ReadPort_9B63C(int a1)//27C63C
{
	return x_inp(a1);
}
// A0EE1: using guessed type x_DWORD inp(x_DWORD);

//----- (0009B688) --------------------------------------------------------
void SerialInterruptHandler_9B688(_DWORD* a1)//27C688
{
	//x_WORD* result; // eax
	__int16 v2; // dx
	__int16 v3; // t1
	signed int v4; // [esp+10h] [ebp-34h]
	x_WORD* v5; // [esp+30h] [ebp-14h]
	x_WORD* v6; // [esp+34h] [ebp-10h]
	unsigned __int8 i; // [esp+38h] [ebp-Ch]
	char v8; // [esp+40h] [ebp-4h]

	//result = (x_WORD*)a1;
	if (a1[7])
	{
		if (a1[5])
		{
			v6 = *(x_WORD**)((char*)a1 + 44);
			if (!(x_inp(a1[7] + 6) & 0x10))
			{
				//result = v6;
				v6[2] |= 2u;
				return;
			}
			v6[2] &= 0xFFFDu;
		}
		//result = (x_WORD*)x_inp(*(x_DWORD*)(a1 + 28) + 5);
		if (x_inp(a1[7] + 5) & 0x20)
		{
			if (a1[6])
				v4 = 16;
			else
				v4 = 1;
			v5 = *(x_WORD**)a1;
			for (i = 0; ; i++)
			{
				//result = v5;
				if (v5[2] <= 0)
					break;
				//result = (x_WORD*)i;
				if (i >= v4)
					break;
				v2 = (*v5)++;
				v8 = *(x_BYTE*)(v2 + a1[1]);
				if (*v5 == v5[3])
					*v5 = 0;
				v3 = v5[2]--;
				x_outp(a1[7], v8);
			}
		}
	}
	return;
}

//----- (0009B7E8) --------------------------------------------------------
int SerialTransmit_9B7E8(_DWORD* a1)
{
	__int16 v1; // dx
	char v3; // [esp+44h] [ebp-1Ch]
	_WORD* v4; // [esp+50h] [ebp-10h]
	char v5; // [esp+54h] [ebp-Ch]
	char v6; // [esp+58h] [ebp-8h]

	if (a1[7])
	{
		*(_WORD*)(a1[11] + 4) |= 0x10u;
		while (1)
		{
			v3 = x_inp(a1[7] + 2);
			if ((v3 & 1) != 0)
				break;
			switch (v3 & 7)
			{
			case 0:
				v6 = x_inp(a1[7] + 6);
				if (a1[5])
				{
					if ((v6 & 0x10) != 0)
					{
						if ((*(_WORD*)(a1[11] + 4) & 2) != 0)
							SerialInterruptHandler_9B688(a1);
					}
					else
					{
						*(_WORD*)(a1[11] + 4) |= 2u;
					}
				}
				break;
			case 2:
				SerialInterruptHandler_9B688(a1);
				break;
			case 4:
				while ((x_inp(a1[7] + 5) & 1) != 0)
				{
					v5 = x_inp(a1[7]);
					v4 = *(_WORD**)((char*)a1 + 10);
					if (v4[2] < v4[3])
					{
						v1 = v4[1];
						v4[1] = v1 + 1;
						*(_BYTE*)(*(_DWORD*)((char*)a1 + 14) + v1) = v5;
						if (v4[1] == v4[3])
							v4[1] = 0;
						++v4[2];
					}
				}
				break;
			case 6:
				x_inp(a1[7] + 5);
				break;
			default:
				continue;
			}
		}
	}
	return x_outp(32, 32);
}

//----- (0009BA00) --------------------------------------------------------
/* void sub_9BA00(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int _0; // [esp+0h] [ebp+0h]

	((void(*)(x_DWORD, x_DWORD, x_DWORD, x_DWORD, int, int, int, int, int *, int, int, int))_GETDS)(
		(unsigned __int16)__GS__,
		(unsigned __int16)__FS__,
		(unsigned __int16)__ES__,
		(unsigned __int16)__DS__,
		a1,
		a2,
		a3,
		a4,
		&_0,
		a5,
		a6,
		a7);
}*/

//----- (0009BA2C) --------------------------------------------------------
/*void sub_9BA2C(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int _0; // [esp+0h] [ebp+0h]

	((void(*)(x_DWORD, x_DWORD, x_DWORD, x_DWORD, int, int, int, int, int *, int, int, int))_GETDS)(
		(unsigned __int16)__GS__,
		(unsigned __int16)__FS__,
		(unsigned __int16)__ES__,
		(unsigned __int16)__DS__,
		a1,
		a2,
		a3,
		a4,
		&_0,
		a5,
		a6,
		a7);
}*/

//----- (0009BA58) --------------------------------------------------------
/*void sub_9BA58(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int _0; // [esp+0h] [ebp+0h]

	((void(*)(x_DWORD, x_DWORD, x_DWORD, x_DWORD, int, int, int, int, int *, int, int, int))_GETDS)(
		(unsigned __int16)__GS__,
		(unsigned __int16)__FS__,
		(unsigned __int16)__ES__,
		(unsigned __int16)__DS__,
		a1,
		a2,
		a3,
		a4,
		&_0,
		a5,
		a6,
		a7);
}*/

//----- (0009BA84) --------------------------------------------------------
/*void sub_9BA84(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int _0; // [esp+0h] [ebp+0h]

	((void(*)(x_DWORD, x_DWORD, x_DWORD, x_DWORD, int, int, int, int, int *, int, int, int))_GETDS)(
		(unsigned __int16)__GS__,
		(unsigned __int16)__FS__,
		(unsigned __int16)__ES__,
		(unsigned __int16)__DS__,
		a1,
		a2,
		a3,
		a4,
		&_0,
		a5,
		a6,
		a7);
}*/

//----- (0009BAB0) --------------------------------------------------------
void NoOp_9BAB0()//27CAB0
{
	;
}

//----- (0009BAC4) --------------------------------------------------------
int SerialSetBaudRate_9BAC4(uint8_t* a1, signed int a2)//27CAC4
{
	int v2; // ST50_4

	if (!*(x_DWORD*)(a1 + 28))
		return 0;
	if (a2 >= 300)
	{
		if (a2 > 115200)
			a2 = 115200;
	}
	else
	{
		a2 = 300;
	}
	v2 = 115200 / a2;
	//sub_AC449();
	x_outp(*(x_DWORD*)(a1 + 28) + 3, 131);
	x_outp(*(x_DWORD*)(a1 + 28) + 1, SBYTE1(v2));
	x_outp(*(x_DWORD*)(a1 + 28), 115200 / a2);
	x_outp(*(x_DWORD*)(a1 + 28) + 3, 3);
	//sub_AC44B();
	return 115200 / (115200 / a2);
}
// A0ED7: using guessed type x_DWORD outp(x_DWORD, char);

//----- (0009BE18) --------------------------------------------------------
int SerialOpenPort_9BE18(uint8_t* a1, int a2, char a3, unsigned int a4, unsigned int a5)//27CE18
{
	//char v5; // STFC_1
	//void (*v6)(); // eax
	//__int16 v7; // dx
	//char v8; // ST4C_1
	//int v10; // [esp+114h] [ebp-30h]
	//int v11; // [esp+118h] [ebp-2Ch]
	//int v12; // [esp+11Ch] [ebp-28h]
	//int v13; // [esp+120h] [ebp-24h]
	int v14 = 0; // [esp+130h] [ebp-14h]
	//__int64 v15; // [esp+134h] [ebp-10h]
	//int v16; // [esp+13Ch] [ebp-8h]
	//int v17; // [esp+140h] [ebp-4h]

	//fix it
	//v7 = 0;
	//v12 = 0;
	//v13 = 0;
	//fix it
	/*
	//fix  sub_9C58C(a1);
	allert_error();
	//fix

	if (a2 && a2 != 2)
	{
		if (a2 != 1 && a2 != 3)
		{
			*(x_DWORD*)(a1 + 28) = 0;
		}
		else
		{
			*(x_DWORD*)(a1 + 36) = 11;
			*(x_DWORD*)(a1 + 40) = 8;
			*(x_DWORD*)(a1 + 28) = *(unsigned __int16*)(2 * a2 + x_DWORD_E3DF8);
		}
	}
	else
	{
		*(x_DWORD*)(a1 + 36) = 12;
		*(x_DWORD*)(a1 + 40) = 16;
		*(x_DWORD*)(a1 + 28) = *(unsigned __int16*)(2 * a2 + x_DWORD_E3DF8);
	}
	if (*(x_DWORD*)(a1 + 28) && !x_DWORD_E3DFC[a2])
	{
		*(x_DWORD*)(a1 + 32) = a2;
		x_DWORD_E3DFC[a2] = a1;
		if (
		//sub_9BD28_allocate_and_lock_memory2(a1)&&
			sub_9BC68_allocate_and_lock_memory((x_WORD*)(*(x_DWORD*)(a1 + 44) + 16), a1, a4)
			&& sub_9BC68_allocate_and_lock_memory((x_WORD*)(*(x_DWORD*)(a1 + 44) + 6), a1 + 10, a5))
		{
			//sub_AC449();
			v16 = x_inp(*(x_DWORD*)(a1 + 28) + 3);
			*(x_DWORD*)(a1 + 66) = v16;
			v16 |= 0x80u;
			x_outp(*(x_DWORD*)(a1 + 28) + 3, v16);
			LODWORD(v15) = x_inp(*(x_DWORD*)(a1 + 28) + 1);
			HIDWORD(v15) = x_inp(*(x_DWORD*)(a1 + 28));
			if (v15)
				*(x_DWORD*)(a1 + 86) = 115200 / (((x_DWORD)v15 << 8) | HIDWORD(v15));
			else
				*(x_DWORD*)(a1 + 86) = 1200;
			v16 &= 0xFFFFFF7F;
			x_outp(*(x_DWORD*)(a1 + 28) + 3, v16);
			*(x_DWORD*)(a1 + 70) = x_inp(*(x_DWORD*)(a1 + 28) + 4);
			*(x_DWORD*)(a1 + 74) = x_inp(33);
			*(x_DWORD*)(a1 + 78) = x_inp(*(x_DWORD*)(a1 + 28) + 1);
			*(x_DWORD*)(a1 + 82) = x_inp(*(x_DWORD*)(a1 + 28) + 2);
			v5 = x_inp(33);
			x_outp(33, *(x_BYTE*)(a1 + 40) | v5);
			x_outp(*(x_DWORD*)(a1 + 28) + 4, 0);
			x_outp(*(x_DWORD*)(a1 + 28) + 1, 0);
			x_outp(*(x_DWORD*)(a1 + 28) + 2, 0);
			v10 = 0x200;
			v11 = *(x_DWORD*)(a1 + 36);
//removed int386(0x31, (REGS*)&v10, (REGS*)&v10);//Get Real Mode Interrupt Vector
			*(x_WORD*)(a1 + 50) = v12;
			*(x_WORD*)(a1 + 52) = v13;
			//v6 = dos_getvect(*(x_DWORD *)(a1 + 36));
			*(x_WORD*)(a1 + 64) = v7;
			// *(x_DWORD *)(a1 + 60) = (int)v6;
	exit(0);//fix
	//dos_setvect(
	//				*(x_DWORD *)(a1 + 36),
	//				(x_DWORD)*(void( **)())((char *)&off_E3E10 + 6 * a2),
	//(unsigned __int16)seg_E3E14[3 * a2]);
	v10 = 0x201;
	v11 = *(x_DWORD*)(a1 + 36);
	v12 = (*(x_DWORD*)(a1 + 44) >> 4) & 0xFFFF;
	v13 = *(x_DWORD*)(a1 + 44) & 0xF;
	//removed int386(0x31, (REGS*)&v10, (REGS*)&v10);//Set Real Mode Interrupt Vector
	sub_9B540_lock_linear_mem_region(sub_9B628, (char*)NoOp_9BAB0 - (char*)sub_9B628);
	if (a3 & 1)
	{
		x_outp(*(x_DWORD*)(a1 + 28) + 2, 193);
		v17 = x_inp(*(x_DWORD*)(a1 + 28) + 2);
		if ((v17 & 0xF8) == 192)
		{
			*(x_DWORD*)(a1 + 24) = 1;
			*(x_WORD*)(*(x_DWORD*)(a1 + 44) + 4) |= 8u;
			x_outp(*(x_DWORD*)(a1 + 28) + 2, 65);
		}
		else
		{
			x_outp(*(x_DWORD*)(a1 + 28) + 2, 0);
		}
	}
	x_outp(*(x_DWORD*)(a1 + 28) + 1, 11);
	x_outp(*(x_DWORD*)(a1 + 28) + 4, 11);
	v8 = x_inp(33);
	x_outp(33, ~*(x_BYTE*)(a1 + 40) & v8);
	x_inp(*(x_DWORD*)(a1 + 28) + 2);
	x_inp(*(x_DWORD*)(a1 + 28));
	x_inp(*(x_DWORD*)(a1 + 28) + 5);
	x_inp(*(x_DWORD*)(a1 + 28) + 6);
	//sub_AC44B();
	SerialSetLineControl_9CD9C(a1, 1);
	SerialSetDtr_9CCF8(a1, 1);
	v14 = 0;
}
		else
		{
			//fix  sub_9C58C(a1);
			allert_error();
			//fix
			v14 = -2;
			}
	}
	else
	{
		//fix  sub_9C58C(a1);
		allert_error();
		//fix
		v14 = -1;
	}
	*/
		return v14;
}
// 99BA7: using guessed type x_DWORD dos_getvect(x_DWORD);
// 99BDB: using guessed type x_DWORD dos_setvect(x_DWORD, x_DWORD, x_DWORD);
// A0ED7: using guessed type x_DWORD outp(x_DWORD, char);
// A0EE1: using guessed type x_DWORD inp(x_DWORD);
// E3DF8: using guessed type int x_DWORD_E3DF8;
// E3DFC: using guessed type int x_DWORD_E3DFC[];
// E3E10: using guessed type void (/*__noreturn*/ *off_E3E10)();
// E3E14: using guessed type __int16 seg_E3E14[];

//----- (0009C810) --------------------------------------------------------
signed int SerialSendByte_9C810(x_DWORD* a1, char a2)
{
	//__int16 v2; // dx
	//__int16 v3; // t1
	//x_WORD* v5; // [esp+0h] [ebp-8h]

	/*
	if (!a1[7])
		return 1;
	v5 = (x_WORD*)*a1;
	if (*(x_WORD*)(*a1 + 4) != *(x_WORD*)(*a1 + 6))
	{
		//sub_AC449();
		v2 = v5[1]++;
		*(x_BYTE*)(a1[1] + v2) = a2;
		if (v5[1] == v5[3])
			v5[1] = 0;
		v3 = v5[2]++;
		SerialInterruptHandler_9B688(a1);
		//sub_AC44B();
		return 1;
	}
	*/
	return 0;
}

//----- (0009C938) --------------------------------------------------------
signed int SerialReceiveByte_9C938(int32_t* a1)//27d938
{
	__int16 v1; // dx
	__int16 v2; // t1
	x_WORD* v5; // [esp+4h] [ebp-8h]
	int v6; // [esp+8h] [ebp-4h]

	if (!a1[7])
		return -1;
	v5 = *(x_WORD**)((char*)a1 + 10);
	if (v5[2] <= 0)
		return -1;
	//sub_AC449();
	v1 = (*v5)++;
	v6 = *(unsigned __int8*)(*(x_DWORD*)((char*)a1 + 14) + v1);
	if (*v5 == v5[3])
		*v5 = 0;
	v2 = v5[2]--;
	//sub_AC44B();
	return v6;
}

//----- (0009C9D0) --------------------------------------------------------
bool SerialIsReceiveEmpty_9C9D0(int a1)//27d9d0
{
	bool v2; // [esp+4h] [ebp-4h]

	if (*(x_DWORD*)(a1 + 28))
		v2 = *(x_WORD*)(*(x_DWORD*)(a1 + 10) + 4) == 0;
	else
		v2 = 1;
	return v2;
}

//----- (0009CBBC) --------------------------------------------------------
void SerialClearBuffer_9CBBC(x_DWORD* a1)//27dbbc
{
	if (a1[7])
	{
		//sub_AC449();
		*(x_WORD*)(*a1 + 4) = 0;
		*(x_WORD*)*a1 = 0;
		*(x_WORD*)(*a1 + 2) = 0;
		*(x_WORD*)(*(x_DWORD*)((char*)a1 + 10) + 4) = 0;
		**(x_WORD**)((char*)a1 + 10) = 0;
		*(x_WORD*)(*(x_DWORD*)((char*)a1 + 10) + 2) = 0;
		*(x_WORD*)(a1[11] + 4) &= 0xFFFDu;
		x_inp(a1[7] + 5);
		x_inp(a1[7] + 6);
		x_inp(a1[7] + 2);
		x_inp(a1[7]);
		//sub_AC44B();
	}
}
// A0EE1: using guessed type x_DWORD inp(x_DWORD);

//----- (0009CCB4) --------------------------------------------------------
int SerialSetRts_9CCB4(uint8_t* a1, int a2)//27dcb4
{
	int result; // eax

	if (a2)
	{
		*(x_DWORD*)(a1 + 20) = 1;
		result = *(x_DWORD*)(a1 + 44);
		*(x_WORD*)(result + 4) |= 1u;
	}
	else
	{
		*(x_DWORD*)(a1 + 20) = 0;
		result = *(x_DWORD*)(a1 + 44);
		*(x_WORD*)(result + 4) &= 0xFFFEu;
	}
	return result;
}

//----- (0009CCF8) --------------------------------------------------------
int SerialSetDtr_9CCF8(uint8_t* a1, int a2)//27dcf8
{
	int result; // eax
	int v3; // ST18_4
	char v4; // al
	int v5; // ST08_4
	char v6; // al

	//result = a1;
	if (*(x_DWORD*)(a1 + 28))
	{
		if (a2)
		{
			v3 = *(x_DWORD*)(a1 + 28) + 4;
			v4 = ReadPort_9B63C(v3) | 1;
			result = x_outp(v3, v4);
		}
		else
		{
			v5 = *(x_DWORD*)(a1 + 28) + 4;
			v6 = ReadPort_9B63C(v5) & 0xFE;
			result = x_outp(v5, v6);
		}
	}
	return result;
}
// A0ED7: using guessed type x_DWORD outp(x_DWORD, char);

//----- (0009BC68) --------------------------------------------------------
// signed int sub_9BC68_allocate_and_lock_memory(x_WORD* a1, uint8_t* a2, unsigned int a3)//27CC68 NOTE: removed (DOS memory handling)

//----- (0009BD28) --------------------------------------------------------
// signed int sub_9BD28_allocate_and_lock_memory2(int a1) // NOTE: removed (DOS memory handling)

//----- (0009CD9C) --------------------------------------------------------
int SerialSetLineControl_9CD9C(uint8_t* a1, int a2)//27dd9c
{
	int result = 0; // eax
	//int v3; // ST18_4
	//char v4; // al
	//int v5; // ST08_4
	//char v6; // al
	/*
	result = a1;
	if (*(x_DWORD*)(a1 + 28))
	{
		if (a2)
		{
			v3 = *(x_DWORD*)(a1 + 28) + 4;
			v4 = ReadPort_9B63C(v3) | 2;
			result = x_outp(v3, v4);
		}
		else
		{
			v5 = *(x_DWORD*)(a1 + 28) + 4;
			v6 = ReadPort_9B63C(v5) & 0xFD;
			result = x_outp(v5, v6);
		}
	}*/
	return result;
}
// A0ED7: using guessed type x_DWORD outp(x_DWORD, char);

//----- (000AC250) --------------------------------------------------------
void ComputeTrackerAngles_AC250(int a1, int a2, int a3, int a4, int a5, x_DWORD* a6, x_DWORD* a7, signed int* a8)//28d250
{
	uint8_t* result; // eax
	int v9; // ebx
	int v10; // esi
	int v11; // ebp
	int v12; // edi
	int v13; // ecx
	int v14; // eax
	int sinCosA[2]; // [ebp-1Ch] sin/cos of v11 - FixedSinCos_B14F8 writes two ints
	int sinCosB[2]; // [ebp-24h] sin/cos of v12
	int v19; // [esp+1Ch] [ebp-14h]

	if (trackerFiltersReady_E4CCC)
		goto LABEL_8;
	result = InitMovingAverage_B1138(trackerFilterA_182188, 3);
	if (result)
	{
		result = InitMovingAverage_B1138(trackerFilterB_1821A0, 3);
		if (result)
		{
			result = InitMovingAverage_B1138(trackerFilterC_1821B8, 3);
			if (result)
			{
				result = InitMovingAverage_B1138(trackerFilterD_1821D0, 6);
				if (result)
				{
					result = InitMovingAverage_B1138(trackerFilterE_1821E8, 6);
					if (result)
					{
						trackerFiltersReady_E4CCC = 1;
					LABEL_8:
						v19 = AddMovingAverage_B11E8((uint32_t*)trackerFilterA_182188, a1);
						v9 = AddMovingAverage_B11E8((uint32_t*)trackerFilterB_1821A0, a2);
						v10 = AddMovingAverage_B11E8((uint32_t*)trackerFilterC_1821B8, a3);
						v11 = AddMovingAverage_B11E8(
							(uint32_t*)trackerFilterD_1821D0,
							((unsigned __int64)(51471 * a4) >> 32 != 0) + ((unsigned __int64)(51471 * a4) >> 14));
						v12 = AddMovingAverage_B11E8(
							(uint32_t*)trackerFilterE_1821E8,
							((unsigned __int64)(51471 * a5) >> 32 != 0) + ((unsigned __int64)(51471 * a5) >> 14));
						FixedSinCos_B14F8(sinCosA, v11);
						FixedSinCos_B14F8(sinCosB, v12);
						v13 = ((unsigned __int64)(v9 * (signed __int64)sinCosA[1]) >> 32 != 0)
							+ ((unsigned __int64)(v9 * (signed __int64)sinCosA[1]) >> 14)
							- (((unsigned __int64)(v10 * (signed __int64)sinCosA[0]) >> 32 != 0)
								+ ((unsigned __int64)(v10 * (signed __int64)sinCosA[0]) >> 14));
						v14 = FixedAtan2_B1304(
							((unsigned __int64)(v10 * (signed __int64)sinCosA[1]) >> 32 != 0)
							+ ((unsigned __int64)(v10 * (signed __int64)sinCosA[1]) >> 14)
							+ ((unsigned __int64)(v9 * (signed __int64)sinCosA[0]) >> 32 != 0)
							+ ((unsigned __int64)(v9 * (signed __int64)sinCosA[0]) >> 14),
							((unsigned __int64)(v19 * (signed __int64)sinCosB[1]) >> 32 != 0)
							+ ((unsigned __int64)(v19 * (signed __int64)sinCosB[1]) >> 14)
							- (((unsigned __int64)(v13 * (signed __int64)sinCosB[0]) >> 32 != 0)
								+ ((unsigned __int64)(v13 * (signed __int64)sinCosB[0]) >> 14)));
						*a6 = ((unsigned __int64)(0xE52EE * (signed __int64)v14) >> 32 != 0)
							+ ((unsigned __int64)(0xE52EE * (signed __int64)v14) >> 14);
						*a7 = ((unsigned __int64)(0xE52EE * (signed __int64)v11) >> 32 != 0)
							+ ((unsigned __int64)(0xE52EE * (signed __int64)v11) >> 14);
						*a8 = ((unsigned __int64)(0xE52EE * (signed __int64)v12) >> 32 != 0)
							+ ((unsigned __int64)(0xE52EE * (signed __int64)v12) >> 14);
					}
				}
			}
		}
	}
}
// E4CCC: using guessed type int trackerFiltersReady_E4CCC;

//----- (000B1138) --------------------------------------------------------
uint8_t* InitMovingAverage_B1138(uint8_t** a1, signed int a2)
{
	uint8_t* result = 0; // eax
	//uint8_t* zero = NULL; // eax
	//signed int v3; // eax
	//int v4; // edx
	//signed int v5; // eax
	/*
	result = (uint8_t*)calloc(a2, 4);
	a1[0] = result;
	if (result)
	{
		result = (uint8_t*)calloc(a2, 4);
		a1[1] = result;
		if (result)
		{
			a1[2] = a2;
			v3 = 0;
			if (a1[2] > 0)
			{
				v4 = 0;
				do
				{
					*(x_DWORD*)(a1[0] + v4) = 0x4000;
					++v3;
					v4 += 4;
				} while (v3 < a1[2]);
			}
			a1[3] = a1[0];
			v5 = a1[0];
			a1[4] = 4 * (a1[2] - 1) + a1[0];
			a1[5] = v5 - 4;
			result = 1+ zero;
		}
	}*/
	return result;
}
// 9B513: using guessed type x_DWORD calloc(x_DWORD, x_DWORD);

//----- (000B11E8) --------------------------------------------------------
int AddMovingAverage_B11E8(uint32_t* a1, int a2)
{
	//x_DWORD* v2; // eax
	//x_DWORD* v3; // esi
	int v4 = 0; // edx
	//x_DWORD* v5; // ecx
	/*
	v2 = (x_DWORD*)a1[3];
	v3 = v2;
	a1[3] = (v2 + 1);
	*v2 = a2;
	if (*a1 + 4 * a1[2] == a1[3])
		a1[3] = *a1;
	v4 = 0;
	do
	{
		v5 = v2;
		--v2;
		v4 += *v5;
		if (v2 == (x_DWORD*)a1[5])
			v2 = (x_DWORD*)a1[4];
	} while (v2 != v3);*/
	return v4 / a1[2];
}

int debugcounter_297253 = 0;

void DrawVrFrame_BD1B6(uint8_t* a1)
{
	int v1; // ecx
	int v2; // eax
	x_BYTE* v3; // edx
	x_BYTE* v4; // ecx
	int v5; // ST04_4
	x_BYTE* v6; // ST08_4
	x_BYTE* v7; // ST0C_4
	int v8; // eax
	x_BYTE* v9; // edx
	x_BYTE* v10; // ecx
	int v11; // ST04_4
	x_BYTE* v12; // ST08_4
	x_BYTE* v13; // ST0C_4
	int v14; // eax
	x_BYTE* v15; // edx
	x_BYTE* v16; // ecx
	int v17; // ST04_4
	x_BYTE* v18; // ST08_4
	x_BYTE* v19; // ST0C_4
	int v20; // eax
	x_BYTE* v21; // edx
	x_BYTE* v22; // ecx
	int v23; // ST04_4
	x_BYTE* v24; // ST08_4
	x_BYTE* v25; // ST0C_4
	int v26; // eax
	x_BYTE* v27; // edx
	x_BYTE* v28; // ecx
	int v29; // ST04_4
	x_BYTE* v30; // ST08_4
	x_BYTE* v31; // ST0C_4
	int v32; // eax
	x_BYTE* v33; // edx
	x_BYTE* v34; // ecx
	int v35; // ST04_4
	x_BYTE* v36; // ST08_4
	x_BYTE* v37; // ST0C_4
	int v38; // eax
	x_BYTE* v39; // edx
	x_BYTE* v40; // ecx
	int v41; // ST04_4
	x_BYTE* v42; // ST08_4
	x_BYTE* v43; // ST0C_4
	int v44; // eax
	x_BYTE* v45; // edx
	x_BYTE* v46; // ecx
	int v47; // ST04_4
	x_BYTE* v48; // ST08_4
	x_BYTE* v49; // ST0C_4
	int v50; // eax
	x_BYTE* v51; // edx
	x_BYTE* v52; // ecx
	int v53; // ST04_4
	x_BYTE* v54; // ST08_4
	x_BYTE* v55; // ST0C_4
	//int v56; // [esp-Ch] [ebp-18h]
	//int v57; // [esp+8h] [ebp-4h]
	int savedregs; // [esp+Ch] [ebp+0h]

	//fix it
	v1 = 0;
	v3 = 0;
	v4 = 0;
	v9 = 0;
	v10 = 0;
	v15 = 0;
	v16 = 0;
	v19 = 0;
	v21 = 0;
	v22 = 0;
	v27 = 0;
	v28 = 0;
	v33 = 0;
	v34 = 0;
	v39 = 0;
	v40 = 0;
	v45 = 0;
	v46 = 0;
	v51 = 0;
	v52 = 0;
	//fix it

	//v56 = v1;
	SetVgaPaletteBank_9951B(0);
	//v57 = 4096;
	savedregs = 4096;
	v2 = BlitToVgaPlanar2_BD320(0, (int8_t*)pdwScreenBuffer_351628, (int8_t*)ptrMemoryBuffer_E9C3C, 0, savedregs, a1);
	v7 = v4;
	v6 = v3;
	v5 = v2;
	SetVgaPaletteBank_9951B(1);
	//v57 = 4096;
	savedregs = 4096;
	v8 = BlitToVgaPlanar2_BD320(v5, (int8_t*)v6, (int8_t*)v7, 0, savedregs, a1);
	v13 = v10;
	v12 = v9;
	v11 = v8;
	SetVgaPaletteBank_9951B(2);
	//v57 = 4096;
	savedregs = 4096;
	v14 = BlitToVgaPlanar2_BD320(v11, (int8_t*)v12, (int8_t*)v13, 0, savedregs, a1);
	v19 = v16;
	v18 = v15;
	v17 = v14;
	SetVgaPaletteBank_9951B(3);
	//v57 = 4096;
	savedregs = 4096;
	v20 = BlitToVgaPlanar2_BD320(v17, (int8_t*)v18, (int8_t*)v19, 0, savedregs, a1);
	v25 = v22;
	v24 = v21;
	v23 = v20;
	SetVgaPaletteBank_9951B(4);
	//v57 = 4096;
	savedregs = 4096;
	v26 = BlitToVgaPlanar2_BD320(v23, (int8_t*)v24, (int8_t*)v25, 0, savedregs, a1);
	v31 = v28;
	v30 = v27;
	v29 = v26;
	SetVgaPaletteBank_9951B(5);
	//v57 = 4096;
	savedregs = 4096;
	v32 = BlitToVgaPlanar2_BD320(v29, (int8_t*)v30, (int8_t*)v31, 0, savedregs, a1);
	v37 = v34;
	v36 = v33;
	v35 = v32;
	SetVgaPaletteBank_9951B(6);
	//v57 = 4096;
	savedregs = 4096;
	v38 = BlitToVgaPlanar2_BD320(v35, (int8_t*)v36, (int8_t*)v37, 0, savedregs, a1);
	v43 = v40;
	v42 = v39;
	v41 = v38;
	SetVgaPaletteBank_9951B(7);
	//v57 = 4096;
	savedregs = 4096;
	v44 = BlitToVgaPlanar2_BD320(v41, (int8_t*)v42, (int8_t*)v43, 0, savedregs, a1);
	v49 = v46;
	v48 = v45;
	v47 = v44;
	SetVgaPaletteBank_9951B(8);
	//v57 = 4096;
	savedregs = 4096;
	v50 = BlitToVgaPlanar2_BD320(v47, (int8_t*)v48, (int8_t*)v49, 0, savedregs, a1);
	v55 = v52;
	v54 = v51;
	v53 = v50;
	SetVgaPaletteBank_9951B(9);
	//v57 = 1536;
	savedregs = 1536;
	BlitToVgaPlanar2_BD320(v53, (int8_t*)v54, (int8_t*)v55, 0, savedregs, a1);
}
