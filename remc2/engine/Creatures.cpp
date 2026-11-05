#include "Creatures.h"
#include "EventsFunctions.h"
#include "Castle.h"
#include "Effects.h"
#include "Projectiles.h"
#include "WizardAi.h"

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

char manaSpriteTable_D400C[8][8] =
{
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x03,0x02,0x01,0x00,0x00,0x00,0x00,0x00},
	{0x04,0x03,0x02,0x01,0x00,0x00,0x00,0x00},
	{0x05,0x04,0x03,0x03,0x01,0x00,0x00,0x00},
	{0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x00},
	{0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00}
}; // idb

int32_t hydraSegmentOffsets_D40BC[17][3] =  // weak//min 16*3 //2a50bc
{
{0x00000000, 0x00000000, 0x00000000},
{0x0000006A, 0x00000024, 0x00000000},
{0x00000097, 0x00000033, 0x00000000},
{0x000000BF, 0x00000041, 0x00000000},
{0x000000DC, 0x0000004B, 0x00000000},
{0x000000F6, 0x00000054, 0x00000000},
{0x00000113, 0x0000005E, 0x00000000},
{0x00000129, 0x00000066, 0x00000000},
{0x0000013E, 0x0000006D, 0x00000000},
{0x00000152, 0x00000074, 0x00000000},
{0x00000169, 0x0000007C, 0x00000000},
{0x0000017C, 0x00000082, 0x00000000},
{0x0000018E, 0x00000088, 0x00000000},
{0x000001A0, 0x0000008F, 0x00000000},
{0x000001B5, 0x00000096, 0x00000000},
{0x000001C6, 0x0000009C, 0x00000000},
{0x00000000, 0x00000000, 0x00000000} };


/*uint8_t x_WORD_D93C0_bldgprmbuffer[0x13f] = {
0x02,0x00,0x03,0x00,//0
0x02,0x00,0x03,0x00,//1
0x02,0x00,0x03,0x00,//2
0x02,0x00,0x03,0x00,//3
0x02,0x00,0x03,0x00,//4
0x02,0x00,0x03,0x00,//5
0x02,0x00,0x03,0x00,//6
0x02,0x00,0x03,0x00,//7
0x02,0x00,0x03,0x00,//8
0x02,0x00,0x03,0x00,//9
0x02,0x00,0x03,0x00,//a
0x02,0x00,0x03,0x00,//b
0x02,0x00,0x03,0x00,//c
0x02,0x00,0x03,0x00,//d
0x02,0x00,0x03,0x00,//e
0x02,0x00,0x03,0x0E,//f
0x02,0x00,0x03,0x10,//10
0xF4,0x01,0x18,0x00,//11
0x06,0x00,0x08,0x15,//12
0x09,0x00,0x03,0x00,//13
0x59,0x01,0x01,0x10,0x06,
0x00,0x08,0x36,0x0F,0x00,
0x03,0x00,0x11,0x00,0x08,
0x18,0x0F,0x00,0x08,0x1C,
0x5E,0x01,0x08,0x19,0x14,
0x00,0x03,0x00,0x8A,0x00,
0x08,0x29,0x07,0x00,0x08,
0x00,0x1C,0x00,0x03,0x00,
0x19,0x00,0x03,0x00,0x1D,
0x00,0x03,0x00,0x33,0x00,
0x03,0x00,0x57,0x00,0x03,
0x00,0x76,0x00,0x03,0x00,0x1C,0x00,0x03,0x00,0xF0,0x00,
0x08,0x24,0x14,0x00,0x08,0x25,0xCC,0x01,0x08,0x26,0x1E,0x00,0x03,0x00,0x64,0x00,
0x03,0x00,0x5E,0x00,0x08,0x31,0x34,0x00,0x03,0x00,0xE6,0x00,0x08,0x2C,0xC3,0x00,
0x08,0x2D,0x78,0x00,0x08,0x00,0xA8,0x00,0x01,0x00,0x22,0x00,0x03,0x00,0x55,0x01,
0x01,0x00,0x4E,0x00,0x08,0x00,0x18,0x00,0x03,0x00,0x1E,0x00,0x08,0x00,0x1E,0x00,
0x08,0x00,0x1E,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x3C,0x00,0x08,0x37,0x33,0x00,
0x08,0x38,0x33,0x00,0x08,0x39,0x90,0x01,0x18,0x00,0x35,0x00,0x08,0x3B,0x29,0x00,
0x08,0x3C,0x6B,0x01,0x00,0x00,0xD7,0x00,0x03,0x00,0xCC,0x01,0x08,0x3F,0x9A,0x01,
0x08,0x40,0x29,0x00,0x08,0x41,0x33,0x00,0x08,0x42,0x90,0x01,0x1C,0x00,0x13,0x01,
0x08,0x45,0xC3,0x00,0x08,0x48,0xBE,0x00,0x08,0x46,0x5A,0x00,0x08,0x47,0x64,0x00,
0x08,0x49,0x50,0x00,0x08,0x00,0xF5,0x00,0x03,0x00,0x28,0x00,0x09,0x00,0xAA,0x00,
0x63,0x0D,0x00,0x33,0x00,0x11,0x3B,0x00,0x33,0x00,0x88,0x3B,0x00

}; // weak//2aa3c2*/
//char x_BYTE_D93C3[] = { '\0' }; // weak //x_BYTE_D93C2[1]
//char x_BYTE_D94F0[] = { '\xAA' }; // weak//x_BYTE_D93C2[0x12e]
//char x_BYTE_D94F1[] = { '\0' }; // weak///x_BYTE_D93C2[0x12f]
//char x_BYTE_D94F2[] = { 'c' }; // weak//x_BYTE_D93C2[0x130]
//char x_BYTE_D94F3[12] = { '\r', '\0', '3', '\0', '\x11', ';', '\0', '3', '\0', '\x88', ';', '\0' }; // idb
//x_BYTE_D93C2[0x131]
//x_BYTE_D93C2[0x13d]
//__int16 x_WORD_D951C[] = { 8 }; // weak //fix it

int8_t dataBlock_D9F50[2248] = {
0x8C,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,//D9F50
0xC8,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,//D9F50
0x18,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x68,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xB8,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x08,0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x58,0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x32,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x64,0x00,0x00,0x01,0x00,0x00,0x69,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x3C,0x00,0x00,0xFF,0x00,0x00,0x3C,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x64,0x00,0x00,0xFF,0x00,0x00,0x68,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x64,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x90,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x00,0x01,0x00,0x01,0x00,0x00,0x6E,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x00,0x01,0x00,0x01,0x00,0x00,0x71,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x00,0x01,0x00,0x01,0x00,0x00,0x74,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x3C,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x00,0x01,0x00,0x01,0x00,0x00,0x79,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xF0,0x00,0x00,0x01,0x00,0x00,0x7C,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xDC,0x05,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x2C,0x01,0x00,0x01,0x00,0x00,0x90,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x03,0xFF,0x00,0x00,0x91,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0xFF,0x00,0x00,0x92,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0xFF,0x00,0x00,0x93,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0xFF,0x00,0x00,0x94,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0xFF,0x00,0x00,0x95,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0xFF,0x00,0x00,0x96,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0xFF,0x00,0x00,0x97,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x32,0x00,0x03,0xFF,0x00,0x00,0x84,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x87,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x8A,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x9E,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x7D,0x00,0x00,0x01,0x00,0x00,0x8D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x9F,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xA8,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xF4,0x01,0x00,0x01,0x00,0x00,0xA8,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xF4,0x01,0x03,0x01,0x00,0x00,0xA8,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xF4,0x01,0x00,0x01,0x00,0x00,0xB2,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x20,0x03,0x00,0x04,0x00,0x00,0xB3,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x20,0x03,0x00,0x04,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x32,0x00,0x00,0xFF,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x2C,0x01,0x00,0x01,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC2,0x01,0x00,0x01,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x58,0x02,0x00,0x01,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xBC,0x02,0x00,0x01,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x20,0x03,0x00,0x01,0x00,0x00,0x4D,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xB6,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xA9,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x98,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x9B,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xB4,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0xFF,0x00,0x00,0xB7,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xBA,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x2C,0x01,0x00,0xFF,0x00,0x00,0xC5,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xA0,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xC6,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x90,0x01,0x00,0x01,0x00,0x00,0xC7,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x2C,0x01,0x00,0xFF,0x00,0x00,0xCA,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xD3,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0xFF,0x00,0x00,0xF3,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0xFF,0x00,0x00,0xCB,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xFB,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xEB,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xE3,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xDB,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0x03,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x4B,0x00,0x00,0xFF,0x00,0x00,0x04,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x05,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x06,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x07,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x0E,0x01,0x00,0x01,0x00,0x00,0x0F,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xBC,0x02,0x00,0x01,0x00,0x00,0x17,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0x1F,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0x27,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x30,0x02,0x00,0x01,0x00,0x00,0x2F,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x52,0x03,0x00,0x01,0x00,0x00,0x37,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x58,0x02,0x00,0x01,0x00,0x00,0x3F,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x3C,0x00,0x00,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x46,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x50,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x5A,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x64,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x6E,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x78,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x82,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xB4,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xDC,0x00,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x18,0x01,0x02,0x01,0x00,0x00,0x47,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x68,0x01,0x02,0x01,0x00,0x00,0x48,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x50,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x58,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x60,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x68,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x70,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x78,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0x80,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0x88,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x78,0x00,0x00,0x01,0x00,0x00,0x89,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x3C,0x00,0x00,0x01,0x00,0x00,0x8A,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x20,0x03,0x00,0x01,0x00,0x00,0x92,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x04,0x01,0x00,0x01,0x00,0x00,0x9A,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x78,0x00,0x00,0x01,0x00,0x00,0x9B,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x40,0x01,0x00,0x01,0x00,0x00,0xA3,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xA0,0x00,0x00,0x01,0x00,0x00,0xA4,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x78,0x00,0x00,0x01,0x00,0x00,0xA5,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x78,0x00,0x00,0x01,0x00,0x00,0xA6,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xA7,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xA8,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x64,0x00,0x00,0x01,0x00,0x00,0xA8,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x96,0x00,0x00,0x01,0x00,0x00,0xA8,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC8,0x00,0x00,0x01,0x00,0x00,0xA8,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xFA,0x00,0x00,0x01,0x00,0x00,0xA9,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xAF,0x00,0x00,0x01,0x00,0x00,0xA9,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x06,0x01,0x00,0x01,0x00,0x00,0xA9,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x5E,0x01,0x00,0x01,0x00,0x00,0xA9,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xB5,0x01,0x00,0x01,0x00,0x00,0xAA,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x3C,0x00,0x00,0xFF,0x00,0x00,0xAA,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x64,0x00,0x00,0xFF,0x00,0x00,0xAA,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0x8C,0x00,0x00,0xFF,0x00,0x00,0xB5,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,
0xC2,0x01,0x00,0x01,0x00,0x00,0xBD,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da776
0xC2,0x01,0x00,0x01,0x00,0x00,0xC5,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da784
0x20,0x03,0x00,0x01,0x00,0x00,0xCD,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da792
0xBC,0x02,0x00,0x01,0x00,0x00,0xCE,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7a0
0xE8,0x03,0x00,0x01,0x00,0x00,0xCF,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7ae
0x64,0x00,0x00,0x01,0x00,0x00,0xD0,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7bc
0xB0,0x04,0x00,0x01,0x00,0x00,0xD8,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7ca
0xB0,0x04,0x00,0x01,0x00,0x00,0xE0,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7d8
0xB0,0x04,0x00,0x01,0x00,0x00,0xE8,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7e6
0xB0,0x04,0x00,0x01,0x00,0x00,0xF0,0x01,0x2C,0x21,0x2A,0x00,0x00,0x00,//da7f4
0xB0,0x04,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//da802
0x00, //DA810
0x00, //DA811
0x00, //DA812
0x00, //DA813
0x00, //DA814
0x00, //DA815
0x00, //DA816
0xFF };//DA817

int segmentPoseFlag_E9BA8; // weak

//----- (0001B5F0) --------------------------------------------------------
void KillAllCreatures_1B5F0()//1fc5f0
{
	for (int index = 0; index < 29; index++)
	{
		switch (index)
		{
			case 10:
				break;
			case 27:
				for (type_entity_0x6E8E* entity = gameState_D41A4.creatureLists_38403x[27]; entity > Entities_EA3E4[0]; entity = entity->next_0)
				{
					entity->actionIndex_0x45_69 = 221;
					entity->ownerPlayerId_0x24_36 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240;
				}
				break;
			default:
				for (type_entity_0x6E8E* entity = gameState_D41A4.creatureLists_38403x[index]; entity > Entities_EA3E4[0]; entity = entity->next_0)
				{
					entity->life_0x8 = -1;
					entity->ownerPlayerId_0x24_36 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240;
				}
				break;
		}
	}
}

//----- (0001B6B0) --------------------------------------------------------
void CreatureFollowParent_1B6B0(type_entity_0x6E8E* a1x)//1fc6b0
{
	type_entity_0x6E8E* v1x; // esi
	axis_3d* v2x; // esi
	int v3; // esi
	int v4; // edi
	__int16 v5; // ax

	v1x = Entities_EA3E4[a1x->leaderId_0x32_50];
	if (v1x->class_0x3F_63 != 5)
		DisableEntityDrawing04_57F10(a1x);
	if (a1x->timer_0x39_57)
	{
		a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		a1x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
		v2x = &v1x->position_0x4C_76;
		predictedAxis_EB398ar = *v2x;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->pitch_0x1E_30, -a1x->param3_0x36_54);
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			v3 = a1x->mail_0x5E_94.channel[0].amount.dword;
			v4 = a1x->life_0x8;
			v5 = a1x->mail_0x5E_94.channel[0].source;
			a1x->mail_0x5E_94.channel[0].source = 0;
			a1x->hitById_0x26_38 = v5;
			a1x->life_0x8 = v4 - v3;
		}
		else
		{
			a1x->hitById_0x26_38 = 0;
		}
	}
	else if (!(a1x->instanceIndex_0x3E_62 & 3))
	{
		CopyEntityPosition_57CF0(a1x, &v1x->position_0x4C_76);
		a1x->yaw_0x1C_28 = v1x->yaw_0x1C_28;
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

int debugcount_1fc8c0 = 0;

//----- (0001B8C0) --------------------------------------------------------
signed int MoveCreature_1B8C0(type_entity_0x6E8E* a1x)//1fc8c0
{
	//char v1; // ah
	//char v2; // dl
	signed int result; // eax
	//signed __int16 *v4; // eax
	__int16 v5; // ST10_2
	int v6; // ST0C_4
	int v7; // ST08_4
	__int16 v8; // ax
	__int16 v9; // ax
	//int v10; // esi
	__int16 v11; // ax
	char v12; // ch
	__int16 v13; // ax
	//signed __int16 *v14; // eax
	__int16 v15; // ST10_2
	int v16; // ST0C_4
	int v17; // ST08_4
	__int16 v18; // ax
	//int v19; // esi
	__int16 v20; // ax
	__int16 v21; // ax
	//signed __int16 *v22; // eax
	__int16 v23; // ST10_2
	int v24; // ST0C_4
	int v25; // ST08_4
	__int16 v26; // ax
	//int v27; // esi
	__int16 v28; // ax
	//signed __int16 *v29; // eax
	__int16 v30; // ST10_2
	int v31; // ST0C_4
	int v32; // ST08_4
	__int16 v33; // ax
	//int v34; // esi
	__int16 v35; // ax
	//int v36; // ebx
	__int16 v37; // [esp+0h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences2()) {
		//add_compare(0x1fc8c4, CommandLineParams.DoDebugafterload());
	}

	//v1 = a1x->flags_0xc_12_15.byte[1];
	if (a1x->flags_0xc_12_15.byte[1] & 8)
	{
		//v2 = a1x->flags_0xc_12_15.byte[1] & 0xF7;
		result = 4;
		a1x->flags_0xc_12_15.byte[1] &= 0xF7;
	}
	else if (a1x->model_0x40_64 == 27)
	{
		result = (unsigned __int8)MoveDragonEntity_2AF10(a1x, 1);
	}
	else
	{
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		//v4 = *(signed __int16 **)(a1x->dword_0xA0_160);
		v5 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		v6 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
		v7 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
		v8 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v8, v7, v6, v5);
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		if (a1x->position_0x4C_76.x >> 8 != predictedAxis_EB398ar.x >> 8 || a1x->position_0x4C_76.y >> 8 != predictedAxis_EB398ar.y >> 8)
		{
			//adress 1fc9e4
			if (CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1)
				|| GetTileSlope_1B7A0(&predictedAxis_EB398ar) >= a1x->typeParams_0xA0_160x->maxSlope_160_0x10_16)
			{
				a1x->flags_0xc_12_15.byte[2] |= 4u;
				v13 = a1x->yaw_0x1C_28;
				v37 = v13;
				v13 += 341;
				v13 &= 0x7ffu;
				a1x->yaw_0x1C_28 = v13;//je z a1+28
				predictedAxis_EB398ar = a1x->position_0x4C_76;
				//v14 = *(signed __int16 **)(a1x->dword_0xA0_160);
				v15 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
				v16 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
				v17 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
				v18 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
				ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v18, v17, v16, v15);
				MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
				if (CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1)
					|| GetTileSlope_1B7A0(&predictedAxis_EB398ar) >= a1x->typeParams_0xA0_160x->maxSlope_160_0x10_16)
				{
					LOBYTE(v21) = v37 - 85;
					HIBYTE(v21) = ((unsigned __int16)(v37 - 341) >> 8) & 7;
					a1x->yaw_0x1C_28 = v21;
					predictedAxis_EB398ar = a1x->position_0x4C_76;
					//v22 = *(signed __int16 **)(a1x->dword_0xA0_160);
					v23 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
					v24 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
					v25 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
					v26 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v26, v25, v24, v23);
					MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
					if (CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1)
						|| GetTileSlope_1B7A0(&predictedAxis_EB398ar) >= a1x->typeParams_0xA0_160x->maxSlope_160_0x10_16)
					{
						a1x->yaw_0x1C_28 = (v37 + 0x400) & 0x700 + (uint8_t)v37;
						predictedAxis_EB398ar = a1x->position_0x4C_76;
						//v29 = (short*)a1x->dword_0xA0_160;
						v30 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
						v31 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
						v32 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
						v33 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
						ClampAboveTerrain_580E0(&predictedAxis_EB398ar, v33, v32, v31, v30);
						MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
						if (CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1)
							|| GetTileSlope_1B7A0(&predictedAxis_EB398ar) >= a1x->typeParams_0xA0_160x->maxSlope_160_0x10_16)
						{
							if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 1 || TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
								a1x->life_0x8 = -1;
							if (a1x->life_0x8 == -1)
							{
								//v36 = *(x_DWORD *)(0x364D1 + x_D41A0_BYTEARRAY_0 + 1);
								if (world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2)
									world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2--;
							}
							result = 4;
						}
						else
						{
							CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
							v35 = a1x->yaw_0x1C_28
								+ TurnTowardsAngle_58350(
									a1x->yaw_0x1C_28,
									a1x->roll_0x20_32,
									a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
									a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
							v35 &= 0x7ffu;
							a1x->yaw_0x1C_28 = v35;
							result = 3;
						}
					}
					else
					{
						CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
						v28 = a1x->yaw_0x1C_28
							+ TurnTowardsAngle_58350(
								a1x->yaw_0x1C_28,
								a1x->roll_0x20_32,
								a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
								a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
						v28 &= 0x7ffu;
						a1x->yaw_0x1C_28 = v28;
						result = 3;
					}
				}
				else
				{
					CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
					v20 = a1x->yaw_0x1C_28
						+ TurnTowardsAngle_58350(
							a1x->yaw_0x1C_28,
							a1x->roll_0x20_32,
							a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
							a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
					v20 &= 0x7ffu;
					a1x->yaw_0x1C_28 = v20;
					result = 3;
				}
			}
			else
			{
				CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
				v11 = a1x->yaw_0x1C_28
					+ TurnTowardsAngle_58350(
						a1x->yaw_0x1C_28,
						a1x->roll_0x20_32,
						a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
						a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
				v11 &= 0x7ffu;
				v12 = a1x->flags_0xc_12_15.byte[2] & 0xFB;
				a1x->yaw_0x1C_28 = v11;
				result = 2;
				a1x->flags_0xc_12_15.byte[2] = v12;
			}
		}
		else
		{
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
			v9 = a1x->yaw_0x1C_28
				+ TurnTowardsAngle_58350(
					a1x->yaw_0x1C_28,
					a1x->roll_0x20_32,
					a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
					a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
			v9 &= 0x7ffu;
			a1x->flags_0xc_12_15.byte[2] &= 0xFBu;
			a1x->yaw_0x1C_28 = v9;
			result = 1;
		}
	}
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];

//----- (0001BD90) --------------------------------------------------------
void CreatureIdle_1BD90(type_entity_0x6E8E* a1x, char a2)//1fcd90
{
	unsigned int v2; // esi
	__int16 v3; // ax
	int i; // eax
	int v5; // edx
	type_entity_0x6E8E* v6x; // eax
	char v7; // al
	//int v8; // esi
	type_entity_0x6E8E* jx; // esi
	int v10; // eax
	unsigned int v11; // edi
	signed int v12; // ST08_4
	__int16 v13; // ax
	unsigned int v15; // [esp+4h] [ebp-Ch]
	unsigned int v16; // [esp+8h] [ebp-8h]
	type_entity_0x6E8E* v17x; // [esp+Ch] [ebp-4h]

	v2 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v2 = 1;
		a1x->hitById_0x26_38 = v3;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				v2 = 1;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v2 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v2 >= 1)
	{
		if (v2 <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
				v7 = a2 + 6;
			else
				v7 = a2 + 2;
		}
		else
		{
			if (v2 != 2)
			{
				KeepCreatureAboveTerrain_1EEE0(a1x);
				return;
			}
			v7 = a2 + 4;
		}
		goto LABEL_35;
	}
	if (!v2)
	{
		//v8 = a1x->dword_0xA0_160;
		if (!(a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 4) && !(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
		{
			v16 = -1;
			v15 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			v17x = 0;
			for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				if (!jx->leaderId_0x32_50 && jx != a1x)
				{
					v10 = (signed __int16)(jx->position_0x4C_76.x - a1x->position_0x4C_76.x);
					v11 = (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
						* (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
						+ v10 * v10;
					if (v11 <= v15)
					{
						v12 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
						v13 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
						if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v13) < v12 && v11 < v16)
						{
							v17x = jx;
							v16 = v11;
						}
					}
				}
			}
			if (v17x)
			{
				a1x->leaderId_0x32_50 = v17x - world_D41A0.entities_0x6E8E;
				v7 = a2 + 3;
			LABEL_35:
				a1x->actionIndex_0x45_69 = v7;
				return KeepCreatureAboveTerrain_1EEE0(a1x);
			}
		}
	}
	return KeepCreatureAboveTerrain_1EEE0(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001BF90) --------------------------------------------------------
void CreatureWander_1BF90(type_entity_0x6E8E* a1x, char a2)//1fcf90
{
	unsigned int v2; // esi
	__int16 v3; // ax
	int i; // eax
	int v5; // edx
	type_entity_0x6E8E* v6x; // eax
	char v7; // al
	char v8; // al
	unsigned __int16 v9; // ax
	char v10; // cl
	int v11; // eax
	type_entity_0x6E8E* v12x; // esi
	int v13; // eax
	unsigned int v14; // edi
	signed int v15; // ST0C_4
	__int16 v16; // ax
	//int v17; // eax
	type_entity_0x6E8E* jx; // esi
	int v19; // eax
	unsigned int v20; // edi
	signed int v21; // ST0C_4
	__int16 v22; // ax
	unsigned int v23; // [esp+0h] [ebp-1Ch]
	type_entity_0x6E8E* v24x; // [esp+8h] [ebp-14h]
	type_entity_0x6E8E* v25x; // [esp+Ch] [ebp-10h]
	unsigned int v26; // [esp+10h] [ebp-Ch]
	unsigned int v27; // [esp+14h] [ebp-8h]
	unsigned int v28; // [esp+18h] [ebp-4h]

	v2 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v2 = 1;
		a1x->hitById_0x26_38 = v3;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				v2 = 1;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v2 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v2 < 1)
	{
		if (!v2)
		{
			MoveCreature_1B8C0(a1x);
			if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v9 = a1x->rand_0x14_20;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v9 % 0x9Du) / 79) - 1);
				v10 = a1x->timer_0x39_57;
				a1x->roll_0x20_32 &= 0x7ff;
				if (v10)
				{
					v11 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					v23 = -1;
					v24x = 0;
					v12x = gameState_D41A4.class3List_38519;
					v27 = v11 * v11;
					while (v12x > Entities_EA3E4[0])
					{
						v13 = (signed __int16)(v12x->position_0x4C_76.x - a1x->position_0x4C_76.x);
						v14 = (signed __int16)(v12x->position_0x4C_76.y - a1x->position_0x4C_76.y)
							* (signed __int16)(v12x->position_0x4C_76.y - a1x->position_0x4C_76.y)
							+ v13 * v13;
						if (v14 <= v27 && !(v12x->flags_0xc_12_15.byte[0] & 0x20))
						{
							v15 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
							v16 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v12x->position_0x4C_76);
							if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v16) < v15 && v14 < v23)
							{
								v24x = v12x;
								v23 = v14;
							}
						}
						v12x = v12x->next_0;
					}
					if (v24x)
					{
						a1x->targetId_0x96_150 = v24x - world_D41A0.entities_0x6E8E;
						if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
							a1x->actionIndex_0x45_69 = a2 + 6;
						else
							a1x->actionIndex_0x45_69 = a2 + 2;
					}
					else
					{
						//v17 = a1x->dword_0xA0_160;
						if (!(a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 4))
						{
							v28 = -1;
							v26 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
							v25x = 0;
							for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
							{
								if (!jx->leaderId_0x32_50 && jx != a1x)
								{
									v19 = (signed __int16)(jx->position_0x4C_76.x - a1x->position_0x4C_76.x);
									v20 = (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
										* (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
										+ v19 * v19;
									if (v20 <= v26)
									{
										v21 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
										v22 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &jx->position_0x4C_76);
										if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v22) < v21 && v20 < v28)
										{
											v25x = jx;
											v28 = v20;
										}
									}
								}
							}
							if (v25x)
							{
								a1x->leaderId_0x32_50 = v25x - world_D41A0.entities_0x6E8E;
								v7 = a2 + 3;
								goto LABEL_49;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (v2 > 1)
		{
			if (v2 != 2)
				return;
			v7 = a2 + 4;
		LABEL_49:
			a1x->actionIndex_0x45_69 = v7;
			return;
		}
		a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
		if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
			v8 = a2 + 6;
		else
			v8 = a2 + 2;
		a1x->actionIndex_0x45_69 = v8;
		KeepCreatureAboveTerrain_1EEE0(a1x);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;lo
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001C310) --------------------------------------------------------
signed int CreatureAttack_1C310(type_entity_0x6E8E* a1x, char a2, unsigned __int16(*a3)(type_entity_0x6E8E*, type_entity_0x6E8E*))//1fd310
{
	signed int v3; // edi
	unsigned int v4; // eax
	__int16 v5; // ax
	int i; // edx
	int v7; // edx
	type_entity_0x6E8E* v8x; // ecx
	type_entity_0x6E8E* v10x; // eax
	type_entity_0x6E8E* v11x; // esi
	__int16 v12; // ax
	type_entity_0x6E8E* jx; // ecx
	unsigned int v16; // ST0C_4

	v3 = 0;
	v4 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		v5 = a1x->mail_0x5E_94.channel[0].source;
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		a1x->hitById_0x26_38 = v5;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v4 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v8x->nextLinkedId_0x34_52)
		{
			v8x = Entities_EA3E4[i];
			if (v8x == Entities_EA3E4[0])
				break;
			v7 = v8x->life_0x8;
			if (v7 < a1x->life_0x8)
			{
				a1x->life_0x8 = v7;
				a1x->hitById_0x26_38 = v8x->hitById_0x26_38;
				v4 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v4 = 2;
	}
	if (v4 < 1)
	{
		if (!v4)
		{
			MoveCreature_1B8C0(a1x);
			v10x = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
			v11x = v10x;
			if (v10x == Entities_EA3E4[0] || v10x->life_0x8 < 0 || v10x->flags_0xc_12_15.byte[1] & 4)
			{
				a1x->actionIndex_0x45_69 = a2 + 1;
				return 0;
			}
			if (!(a1x->instanceIndex_0x3E_62 & 3))
			{
				v12 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v10x->position_0x4C_76);
				a1x->roll_0x20_32 = v12;
				// neighbour distance: movsx on each coordinate, 32-bit subtraction (0x1FD474)
				for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
				{
					if (jx->id_0x1A_26 != a1x->id_0x1A_26
						&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch
						&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
					{
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
						break;
					}
				}
			}
			if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
			{
				v16 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Distance3D_583F0(&a1x->position_0x4C_76, &v11x->position_0x4C_76) >= v16)
				{
					a1x->actionIndex_0x45_69 = a2 + 1;
					return 0;
				}
				if (a3(a1x, v11x))
					v3 = 1;
			}
		}
	}
	else
	{
		if (v4 <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			KeepCreatureAboveTerrain_1EEE0(a1x);
			return 0;
		}
		if (v4 == 2)
			a1x->actionIndex_0x45_69 = a2 + 4;
	}
	return v3;
}

//----- (0001C560) --------------------------------------------------------
void CreatureFollowLeader_1C560(type_entity_0x6E8E* a1x, unsigned __int16 a2)//1fd560
{
	char v2; // al
	//uint8_t* v3; // eax
	int8_t v3y;
	type_entity_0x6E8E* v3x; // eax
	type_entity_0x6E8E* v4x; // esi
	signed __int16 v5; // cx
	int i; // eax
	int v7; // edx
	//int v8; // eax
	char v9; // al
	//int v10; // eax
	char v11; // al
	//int v12; // eax
	char v13; // al
	type_entity_0x6E8E* v14x; // ecx
	bool v16; // [esp+4h] [ebp-4h]

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x1fd564, CommandLineParams.DoDebugafterload());
	}

	//fix
	v3x = 0;
	//fix

	if (!a1x->leaderId_0x32_50)
	{
		v2 = a2;
	LABEL_57:
		v3y = v2 + 1;
	LABEL_58:
		a1x->actionIndex_0x45_69 = v3y;
		return;// v3y;
	}
	//LOWORD(v3) = a1x->word_0x32_50;
	v4x = Entities_EA3E4[a1x->leaderId_0x32_50];
	v16 = v4x->life_0x8 >= 0
		&& !(v4x->flags_0xc_12_15.byte[1] & 4)
		&& v4x->class_0x3F_63 == a1x->class_0x3F_63//(LOBYTE(v3) = v4x->byte_0x3F_63, (x_BYTE)v3 == a1x->byte_0x3F_63)
		&& v4x->model_0x40_64 == a1x->model_0x40_64;//(LOBYTE(v3) = v4x->byte_0x40_64, (x_BYTE)v3 == a1x->byte_0x40_64);
	v5 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		//LOWORD(v3) = a1x->word_0x62_98;
		a1x->hitById_0x26_38 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v5 = 1;
		//a1x->word_0x62_98 = (int16_t)v3;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v3x->nextLinkedId_0x34_52)
		{
			v3x = Entities_EA3E4[i];
			if (v3x == Entities_EA3E4[0])
				break;
			v7 = v3x->life_0x8;
			if (v7 < a1x->life_0x8)
			{
				a1x->life_0x8 = v7;
				//LOWORD(v3) = v3x->hitById_0x26_38;
				v5 = 1;
				a1x->hitById_0x26_38 = v3x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		//LOWORD(v3) = a1x->hitById_0x26_38;
		v5 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if ((unsigned __int16)v5 < 1u)
	{
		if (v5)
			return;// (int8_t)v3;
		MoveCreature_1B8C0(a1x);
		if (!v16)
		{
			v2 = a2;
			a1x->leaderId_0x32_50 = 0;
			goto LABEL_57;
		}
		//LOWORD(v3) = (signed __int16)a1x->instanceIndex_0x3E_62 / a1x->typeParams_0xA0_160x->word_160_0x1a_26;
		if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
		{
			switch (v4x->actionIndex_0x45_69 - a2)
			{
			case 0:
			case 1:
				goto LABEL_45;
			case 2:
				a1x->targetId_0x96_150 = v4x->targetId_0x96_150;
				a1x->leaderId_0x32_50 = 0;
				//LOBYTE(v3) = a2 + 2;
				a1x->actionIndex_0x45_69 = a2 + 2;
				return;// (int8_t)v3;
			case 3:
				a1x->leaderId_0x32_50 = v4x->leaderId_0x32_50;
			LABEL_45:
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(
					&a1x->position_0x4C_76,
					&Entities_EA3E4[a1x->leaderId_0x32_50]->position_0x4C_76);
				v14x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
				break;
			case 6:
				a1x->targetId_0x96_150 = v4x->targetId_0x96_150;
				a1x->leaderId_0x32_50 = 0;
				//LOBYTE(v3) = a2 + 6;
				a1x->actionIndex_0x45_69 = a2 + 6;
				return;// (int8_t)v3;
			default:
				a1x->leaderId_0x32_50 = 0;
				//LOBYTE(v3) = a2 + 1;
				a1x->actionIndex_0x45_69 = a2 + 1;
				return;// (int8_t)v3;
			}
			while (v14x > Entities_EA3E4[0])
			{
				if (v14x->id_0x1A_26 != a1x->id_0x1A_26
					&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v14x->position_0x4C_76.x) < 256
					&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v14x->position_0x4C_76.y) < 256)
				{
					a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v14x->position_0x4C_76, &a1x->position_0x4C_76);
					break;
				}
				v14x = v14x->next_0;
			}
			//LOWORD(v3) = v4x->word_0x86_134 + v4x->word_0x82_130;
			a1x->actSpeed_0x82_130 = v4x->maxSpeed_0x86_134 + v4x->actSpeed_0x82_130;
		}
	}
	else
	{
		if ((unsigned __int16)v5 > 1u)
		{
			if (v5 != 2)
				return;// (int8_t)v3;
			if (v16)
			{
				v4x->targetId_0x96_150 = a1x->hitById_0x26_38;
				//v8 = v4x->dword_0xA0_160;
				v4x->leaderId_0x32_50 = 0;
				if (v4x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
					v9 = a2 + 6;
				else
					v9 = a2 + 2;
				v4x->actionIndex_0x45_69 = v9;
			}
			a1x->leaderId_0x32_50 = 0;
			v3y = a2 + 4;
			goto LABEL_58;
		}
		if (v16)
		{
			v4x->targetId_0x96_150 = a1x->hitById_0x26_38;
			//v10 = v4x->dword_0xA0_160;
			v4x->leaderId_0x32_50 = 0;
			if (v4x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
				v11 = a2 + 6;
			else
				v11 = a2 + 2;
			v4x->actionIndex_0x45_69 = v11;
		}
		a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
		//v12 = a1x->dword_0xA0_160;
		a1x->leaderId_0x32_50 = 0;
		if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
			v13 = a2 + 6;
		else
			v13 = a2 + 2;
		a1x->actionIndex_0x45_69 = v13;
		/*LOBYTE(v3) = */KeepCreatureAboveTerrain_1EEE0(a1x);
	}
	//return (int8_t)v3;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001C980) --------------------------------------------------------
void CreatureFlee_1C980(type_entity_0x6E8E* a1x, char a2)//1fd980
{
	unsigned int v2; // ecx
	__int16 v3; // ax
	int i; // eax
	int v5; // edx
	type_entity_0x6E8E* v6x; // eax
	char v7; // al
	type_entity_0x6E8E* v9x; // eax
	type_entity_0x6E8E* v10x; // esi
	__int16 v11; // ax
	//int v12; // edx
	//int v13; // eax
	type_entity_0x6E8E* jx; // ecx
	//int v15; // ecx
	unsigned int v16; // ST08_4

	v2 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		a1x->hitById_0x26_38 = v3;
		v2 = 1;
		a1x->mail_0x5E_94.channel[0].source = 0;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				v2 = 1;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v2 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v2 < 1)
	{
		if (v2)
			return;// 0;
		MoveCreature_1B8C0(a1x);
		v9x = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
		v10x = v9x;
		if (v9x > Entities_EA3E4[0] && v9x->life_0x8 >= 0 && !(v9x->flags_0xc_12_15.byte[1] & 4))
		{
			if (!(a1x->instanceIndex_0x3E_62 & 3))
			{
				v11 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v9x->position_0x4C_76);
				HIBYTE(v11) += 4;
				a1x->roll_0x20_32 = v11;
				//v12 = 4 * *(char *)(a1 + 64);
				//v13 = gameState_D41A4.dwordindex_0;
				a1x->roll_0x20_32 &= 0x7ff;
				for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
				{
					if (jx->id_0x1A_26 != a1x->id_0x1A_26
						&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch
						&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
					{
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
						break;
					}
				}
			}
			//v15 = a1x->dword_0xA0_160;
			if (a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26)
				return;// 0;
			v16 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v10x->position_0x4C_76) < v16)
				return;// 0;
		}
		v7 = a2 + 1;
		goto LABEL_34;
	}
	if (v2 <= 1)
	{
		a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
		KeepCreatureAboveTerrain_1EEE0(a1x);
		return;// 0;
	}
	if (v2 == 2)
	{
		v7 = a2 + 4;
	LABEL_34:
		a1x->actionIndex_0x45_69 = v7;
	}
	//return 0;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001CC20) --------------------------------------------------------
signed int CreatureShootFireball_1CC20(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fdc20
{
	axis_3d* v2; // esi
	signed int v3; // edi
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // ebx
	char v6; // al

	v2 = &a1x->position_0x4C_76;
	v3 = 0;
	v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 0);
	v5x = v4x;
	if (v4x)
	{
		v4x->levelPar1_0x43_67 = 10;
		v4x->levelPar2_0x44_68 = 0;
		v4x->id_0x1A_26 = a1x->id_0x1A_26;
		v4x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v2, &a2x->position_0x4C_76);
		v5x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v2, &a2x->position_0x4C_76);
		v5x->position_0x4C_76.z += a1x->params_0x52_82.fov;
		v5x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v5x->typeParams_0xA0_160x = &str_D7BD6[65];//(type_str_160*)&unk_D7BD6[0x8a2];
		v5x->xsubtype_0x42_66 = a2x->model_0x40_64;
		v6 = a2x->class_0x3F_63;
		v5x->subSpellIndex_0x2A_42 = 500;
		v3 = 1;
		v5x->xtype_0x41_65 = v6;
		SetWizardHitTimer_5EF70(a2x);
	}
	return v3;
}

//----- (0001CCE0) --------------------------------------------------------
signed int CreatureShootArrow_1CCE0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fdce0
{
	axis_3d* v2; // edi
	signed int v3; // esi
	type_entity_0x6E8E* v4x; // ebx
	char v5; // al

	v2 = &a1x->position_0x4C_76;
	v3 = 0;
	v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 13);
	if (v4x)
	{
		v4x->id_0x1A_26 = a1x->id_0x1A_26;
		v4x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v2, &a2x->position_0x4C_76);
		v4x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v2, &a2x->position_0x4C_76);
		v4x->position_0x4C_76.z += a1x->params_0x52_82.fov / 2;
		v4x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v4x->xsubtype_0x42_66 = a2x->model_0x40_64;
		v5 = a2x->class_0x3F_63;
		v4x->subSpellIndex_0x2A_42 = 250;
		v4x->xtype_0x41_65 = v5;
		SetEntitySpriteDoubleSize_49E10(v4x, 195);
		v3 = 1;
		SetWizardHitTimer_5EF70(a2x);
	}
	return v3;
}

//----- (0001CDA0) --------------------------------------------------------
signed int CreatureShootArrow2_1CDA0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fdda0
{
	signed int v2; // edi
	type_entity_0x6E8E* v3x; // ebx
	axis_3d* v5; // [esp+4h] [ebp-4h]

	v2 = 0;
	v5 = &a1x->position_0x4C_76;
	v3x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 13);
	if (v3x)
	{
		v3x->id_0x1A_26 = a1x->id_0x1A_26;
		v3x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v5, &a2x->position_0x4C_76);
		v3x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v5, &a2x->position_0x4C_76);
		// [esi+58h]: the shooter's array_0x52_82 +6, which is fov, not roll (+4)
		v3x->position_0x4C_76.z += a1x->params_0x52_82.fov;
		v3x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v3x->xsubtype_0x42_66 = a2x->model_0x40_64;
		v3x->xtype_0x41_65 = a2x->class_0x3F_63;
		if (a1x->playerEntityIndex_0x94_148)
			v3x->subSpellIndex_0x2A_42 = 600;
		else
			v3x->subSpellIndex_0x2A_42 = 400;
		SetEntitySpriteDoubleSize_49E10(v3x, 203);
		v2 = 1;
		SetWizardHitTimer_5EF70(a2x);
	}
	return v2;
}

//----- (0001CE80) --------------------------------------------------------
signed int CreatureMeleeAttack1024_1CE80(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fde80
{
	signed int v2; // ebx

	v2 = 0;
	if ((signed int)Distance3D_583F0(&a1x->position_0x4C_76, &a2x->position_0x4C_76) < 1024)
	{
		v2 = 1;
		SendMailToEntity_11900(a1x, a2x, 0, a1x->subSpellIndex_0x2A_42);
	}
	return v2;
}

//----- (0001CED0) --------------------------------------------------------
signed int CreatureMeleeAttack768_1CED0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fded0
{
	signed int v2; // ebx

	v2 = 0;
	if ((signed int)Distance3D_583F0(&a1x->position_0x4C_76, &a2x->position_0x4C_76) < 768)
	{
		v2 = 1;
		SendMailToEntity_11900(a1x, a2x, 0, a1x->subSpellIndex_0x2A_42);
	}
	return v2;
}

//----- (0001CF20) --------------------------------------------------------
signed int CreatureMeleeAttack1536_1CF20(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fdf20
{
	signed int v2; // ebx

	v2 = 0;
	if ((signed int)Distance3D_583F0(&a1x->position_0x4C_76, &a2x->position_0x4C_76) < 1536)
	{
		v2 = 1;
		SendMailToEntity_11900(a1x, a2x, 0, a1x->subSpellIndex_0x2A_42);
	}
	return v2;
}

//----- (0001D0E0) --------------------------------------------------------
signed int CreatureShootProjectile20_1D0E0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fe0e0
{
	axis_3d* v2; // esi
	signed int v3; // edi
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // ebx
	char v6; // al

	v2 = &a1x->position_0x4C_76;
	v3 = 0;
	v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 20);
	v5x = v4x;
	if (v4x)
	{
		v4x->levelPar1_0x43_67 = 10;
		v4x->levelPar2_0x44_68 = 65;
		v4x->id_0x1A_26 = a1x->id_0x1A_26;
		v4x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v2, &a2x->position_0x4C_76);
		v5x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v2, &a2x->position_0x4C_76);
		v5x->position_0x4C_76.z += a1x->params_0x52_82.fov;
		v5x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v5x->typeParams_0xA0_160x = &str_D7BD6[65];// (type_str_160*)&unk_D7BD6[0x8a2];
		v5x->xsubtype_0x42_66 = a2x->model_0x40_64;
		v6 = a2x->class_0x3F_63;
		v5x->subSpellIndex_0x2A_42 = 780;
		v3 = 1;
		v5x->xtype_0x41_65 = v6;
		SetWizardHitTimer_5EF70(a2x);
	}
	return v3;
}

//----- (0001D1A0) --------------------------------------------------------
signed int CreatureShootProjectile21_1D1A0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fe1a0
{
	axis_3d* v2; // esi
	signed int v3; // edi
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // ebx
	//__int16 v6; // ax
	__int16 v7; // dx

	v2 = &a1x->position_0x4C_76;
	v3 = 0;
	v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 21);
	v5x = v4x;
	if (v4x)
	{
		v4x->levelPar1_0x43_67 = 10;
		v4x->levelPar2_0x44_68 = 66;
		v4x->id_0x1A_26 = a1x->id_0x1A_26;
		v4x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v2, &a2x->position_0x4C_76);
		///*v6 = */Maths::GetPitchAngle_58210(v2, &a2x->array_0x4C_76);
		v7 = v5x->position_0x4C_76.z;
		v5x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v2, &a2x->position_0x4C_76);
		v5x->position_0x4C_76.z = v7 + 128;
		v5x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v5x->typeParams_0xA0_160x = &str_D7BD6[65];//(type_str_160*)&unk_D7BD6[0x8a2];
		v5x->xsubtype_0x42_66 = a2x->model_0x40_64;
		//LOBYTE(v6) = a2x->byte_0x3F_63;
		v5x->subSpellIndex_0x2A_42 = 780;
		v3 = 1;
		v5x->xtype_0x41_65 = a2x->class_0x3F_63;
		SetWizardHitTimer_5EF70(a2x);
	}
	return v3;
}

//----- (0001D260) --------------------------------------------------------
signed int CreatureShootProjectile9_1D260(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fe260
{
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // ebx
	char v4; // al
	signed int v6; // [esp+0h] [ebp-4h]

	predictedAxis_EB398ar = a1x->position_0x4C_76;
	v6 = 0;
	predictedAxis_EB398ar.z += a1x->params_0x52_82.fov;
	v2x = CreateEntity_4A190(&predictedAxis_EB398ar, 9, 9);
	v3x = v2x;
	if (v2x)
	{
		v2x->levelPar1_0x43_67 = 10;
		v2x->levelPar2_0x44_68 = 23;
		v2x->id_0x1A_26 = a1x->id_0x1A_26;
		v2x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&predictedAxis_EB398ar, &a2x->position_0x4C_76);
		v3x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(&predictedAxis_EB398ar, &a2x->position_0x4C_76);
		v3x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v3x->typeParams_0xA0_160x = &str_D7BD6[64];//(type_str_160*)&unk_D7BD6[0x880];
		v3x->xsubtype_0x42_66 = a2x->model_0x40_64;
		v4 = a2x->class_0x3F_63;
		v3x->subSpellIndex_0x2A_42 = 4000;
		v3x->xtype_0x41_65 = v4;
		SetWizardHitTimer_5EF70(a2x);
		v6 = 1;
	}
	return v6;
}
// D8456: using guessed type void *off_D8456;
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (0001D460) --------------------------------------------------------
void CreatureShootSpread_1D460(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1fe460
{
	unsigned __int16 v2; // si
	signed __int16 v3; // di
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* v5x; // ebx
	__int16 v6; // ax
	__int16 v7; // cx
	axis_3d* v9; // [esp+4h] [ebp-8h]
	//signed int v10; // [esp+8h] [ebp-4h]

	v2 = 0;
	//v10 = 0;
	do
	{
		switch (v2)
		{
		case 0u:
			v3 = -226;
			break;
		case 1u:
			v3 = -113;
			break;
		case 2u:
			v3 = 0;
			break;
		case 3u:
			v3 = 113;
			break;
		default:
			v3 = 226;
			break;
		}
		v9 = &a1x->position_0x4C_76;
		v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 0);
		v5x = v4x;
		if (v4x)
		{
			v4x->levelPar1_0x43_67 = 10;
			v4x->levelPar2_0x44_68 = 0;
			v4x->typeParams_0xA0_160x = &str_D7BD6[61];//(type_str_160*)&unk_D7BD6[0x81a];
			v4x->xsubtype_0x42_66 = a2x->model_0x40_64;
			v4x->xtype_0x41_65 = a2x->class_0x3F_63;
			v4x->subSpellIndex_0x2A_42 = 800;
			v4x->id_0x1A_26 = a1x->id_0x1A_26;
			v4x->yaw_0x1C_28 = v3 + Maths::GetYawAngle_581E0(v9, &a2x->position_0x4C_76);
			v6 = Maths::GetPitchAngle_58210(v9, &a2x->position_0x4C_76);
			v7 = v5x->position_0x4C_76.z;
			v5x->pitch_0x1E_30 = v6;
			v5x->position_0x4C_76.z = v7 + 200;
			//v10 = 1;
			v5x->targetId_0x96_150 = a1x->targetId_0x96_150;
		}
		v2++;
	} while (v2 < 5u);
	//return v10;
}

//----- (0001D5D0) --------------------------------------------------------
void CreatureRunScript_1D5D0(type_entity_0x6E8E* a1x, __int16 a2)//1fe5d0
{
	switch (a1x->StageVar2_0x49_73)
	{
	case 1:
		CreatureScriptGoToPosition_1DDA0(a1x, a2);
		break;
	case 2:
		CreatureScriptGuardPosition_1DBF0(a1x, a2);
		break;
	case 3:
		CreatureScriptCmd3_1D7C0(a1x, a2);
		break;
	case 4:
		CreatureScriptCmd4_1D700(a1x, a2);
		break;
	case 5:
		CreatureScriptAttack_1D8C0(a1x, a2);
		break;
	case 6:
		CreatureScriptCmd6_1E000(a1x, a2);
		break;
	case 7:
		CreatureScriptCmd7_1E020(a1x, a2);
		break;
	case 8:
		CreatureScriptCmd8_1D880(a1x, a2);
		break;
	case 9:
		CreatureScriptCmd9_1D8A0(a1x, a2);
		break;
	case 0xA:
		CreatureScriptResume_1E040(a1x, a2);
		break;
	case 0xC:
		CreatureScriptFollowParent_1E4D0(a1x);
		break;
	case 0xD:
	case 0x10:
		CreatureScriptEscortParent_1E580(a1x, a2);
		break;
	case 0xE:
		CreatureScriptCmd14_1E9C0(a1x, a2);
		break;
	case 0x11:
		CreatureScriptChaseTarget_1E320(a1x);
		break;
	default:
		return;
	}
}

//----- (0001D700) --------------------------------------------------------
void CreatureScriptCmd4_1D700(type_entity_0x6E8E* a1x, __int16 a2)//1fe700
{
	type_str_0x3647Ac* v2x; // esi
	type_entity_0x6E8E* v3x; // esi
	unsigned __int16 v4; // dx
	unsigned int v5; // edi

	CreatureScriptAttack_1D8C0(a1x, a2);
	if (!(a1x->instanceIndex_0x3E_62 & 7))
	{
		v2x = &world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72];
		if (v2x->stage_0x3647A_1 & 2)
			v3x = Entities_EA3E4[a1x->nextStage_0x4A_74];
		else
			v3x = v2x->target_0x3647C_4.pointer_0x6E8E;
		if (v3x)
		{
			if (v3x > Entities_EA3E4[0])
			{
				v4 = v3x->targetId_0x96_150;
				if (v4 == 0xae02)return;//fix
				if (v4)
				{
					v5 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					if (Distance3D_583F0(&a1x->position_0x4C_76, &Entities_EA3E4[v4]->position_0x4C_76) <= v5)
					{
						a1x->targetId_0x96_150 = v3x->targetId_0x96_150;
						a1x->StageVar2_0x49_73 = 10;
						CreatureScriptResume_1E040(a1x, a2);
					}
				}
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001D7C0) --------------------------------------------------------
void CreatureScriptCmd3_1D7C0(type_entity_0x6E8E* a1x, __int16 a2)//1fe7c0
{
	type_str_0x3647Ac* v2x; // esi
	type_entity_0x6E8E* v3x; // esi
	unsigned int v4; // edi

	CreatureScriptAttack_1D8C0(a1x, a2);
	if (!(a1x->instanceIndex_0x3E_62 & 7))
	{
		v2x = &world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72];
		if (v2x->stage_0x3647A_1 & 2)
			v3x = Entities_EA3E4[a1x->nextStage_0x4A_74];
		else
			v3x = v2x->target_0x3647C_4.pointer_0x6E8E;
		if (v3x && v3x > Entities_EA3E4[0])
		{
			v4 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v3x->position_0x4C_76) <= v4)
			{
				a1x->targetId_0x96_150 = v3x - world_D41A0.entities_0x6E8E;
				a1x->StageVar2_0x49_73 = 10;
				CreatureScriptResume_1E040(a1x, a2);
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001D880) --------------------------------------------------------
void CreatureScriptCmd8_1D880(type_entity_0x6E8E* a1x, __int16 a2)//1fe880
{
	CreatureScriptIdle_1E1C0(a1x, a2);
}

//----- (0001D8A0) --------------------------------------------------------
void CreatureScriptCmd9_1D8A0(type_entity_0x6E8E* a1x, __int16 a2)//1fe8a0
{
	CreatureScriptIdle_1E1C0(a1x, a2);
}

//----- (0001D8C0) --------------------------------------------------------
void CreatureScriptAttack_1D8C0(type_entity_0x6E8E* a1x, __int16 a2)//1fe8c0
{
	unsigned int v2; // ecx
	__int16 v3; // ax
	int i; // eax
	int v5; // eax
	type_entity_0x6E8E* v6x; // esi
	type_entity_0x6E8E* v7x; // eax
	char v8; // al
	char v9; // cl
	char v10; // al
	type_entity_0x6E8E* v12x; // esi
	__int16 v13; // ax
	char v14; // dl
	unsigned __int16 v15; // ax
	type_entity_0x6E8E* jx; // ecx

	// fix it
	v12x = 0;
	// fix it

	v2 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v2 = 1;
		a1x->hitById_0x26_38 = v3;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52 && a1x->model_0x40_64 != 27)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				v2 = 1;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v2 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v2 < 1)
	{
		if (!v2)
		{
			MoveCreature_1B8C0(a1x);
			if (!(a1x->instanceIndex_0x3E_62 & 7))
			{
				if (world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72].stage_0x3647A_1 & 2)
				{
					if (!a1x->nextStage_0x4A_74)
						a1x->nextStage_0x4A_74 = FindScriptGroupLeader_1E3E0(a1x);
					v12x = Entities_EA3E4[a1x->nextStage_0x4A_74];
				}
				else
				{
					v12x = world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72].target_0x3647C_4.pointer_0x6E8E;
				}
				if (v12x && v12x > Entities_EA3E4[0])
				{
					if (!(a1x->flags_0xc_12_15.byte[2] & 4))
					{
						v13 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v12x->position_0x4C_76);
						v14 = a1x->instanceIndex_0x3E_62;
						a1x->roll_0x20_32 = v13;
						if (!(v14 & 0x3F))
						{
							a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
							v15 = a1x->rand_0x14_20;
							a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
							a1x->roll_0x20_32 += (2 * ((signed int)(v15 % 0x9Du) / 79) - 1) * ((a1x->rand_0x14_20 & 0xFF) + 85);
							a1x->roll_0x20_32 &= 0x7ff;
						}
					}
					for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
					{
						if (jx->id_0x1A_26 != a1x->id_0x1A_26
							&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch
							&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
						{
							a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
							break;
						}
					}
					//1FEB51 movsx edx,[ebx+4Ch]; movsx eax,[esi+4Ch]; sub edx,eax
					if (abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v12x->position_0x4C_76.x) < a1x->params_0x52_82.pitch
						+ v12x->params_0x52_82.pitch
						&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v12x->position_0x4C_76.y) < a1x->params_0x52_82.pitch
						+ v12x->params_0x52_82.pitch)
					{
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v12x->position_0x4C_76, &a1x->position_0x4C_76);
					}
				}
				else if (!(a1x->instanceIndex_0x3E_62 & 0xF) && !(a1x->flags_0xc_12_15.byte[2] & 4))
				{
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					a1x->roll_0x20_32 += a1x->rand_0x14_20 % 0x71u + 142;
					a1x->roll_0x20_32 &= 0x7ff;
				}
			}
		}
	}
	else if (v2 <= 1)
	{
		v7x = Entities_EA3E4[a1x->hitById_0x26_38];
		if (v7x->class_0x3F_63 != a1x->class_0x3F_63 || (v8 = v7x->model_0x40_64, v9 = a1x->model_0x40_64, v8 != v9))
			v10 = 1;
		else
			v10 = v9 ^ v8;
		if (v10)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			a1x->StageVar2_0x49_73 = 10;
			CreatureScriptResume_1E040(a1x, a2);
		}
		KeepCreatureAboveTerrain_1EEE0(a1x);
	}
	else if (v2 == 2)
	{
		a1x->actionIndex_0x45_69 = a2 + 4;
	}
}

//----- (0001DBF0) --------------------------------------------------------
void CreatureScriptGuardPosition_1DBF0(type_entity_0x6E8E* a1x, unsigned __int16 a2)//1febf0
{
	type_str_0x3647Ac* v2x; // esi
	//int result; // eax
	//int v4; // esi
	type_entity_0x6E8E* v5x; // edi
	type_entity_0x6E8E* v6x; // esi
	int v7; // ecx
	__int16 v8; // dx
	int v9; // ST08_4
	__int16 v10; // ax
	unsigned int v11; // [esp+4h] [ebp-Ch]
	unsigned int v12; // [esp+8h] [ebp-8h]
	unsigned int v13; // [esp+Ch] [ebp-4h]

	v2x = &world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72];
	if (abs((int16_t)v2x->target_0x3647C_4.axis.x - (int16_t)a1x->position_0x4C_76.x) > 3072
		|| abs((int16_t)v2x->target_0x3647C_4.axis.y - (int16_t)a1x->position_0x4C_76.y) > 3072)
	{
		CreatureScriptGoToPosition_1DDA0(a1x, a2);
	}
	else
	{
		CreatureScriptIdle_1E1C0(a1x, a2);
	}
	//result = a2 + 7;
	if (a1x->actionIndex_0x45_69 == a2 + 7 && a1x->StageVar2_0x49_73 == 2)
	{
		if (a1x->timer_0x39_57)
		{
			//v4 = a1x->dword_0xA0_160;
			//result = a1x->instanceIndex_0x3E_62 / a1x->typeParams_0xA0_160x->word_160_0x1a_26;
			if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
			{
				//result = a1x->typeParams_0xA0_160x->word_160_0x1c_28 * a1x->typeParams_0xA0_160x->word_160_0x1c_28;
				v5x = 0;
				v12 = -1;
				v6x = gameState_D41A4.class3List_38519;
				v11 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				while (v6x > Entities_EA3E4[0])
				{
					v7 = (signed __int16)(v6x->position_0x4C_76.x - a1x->position_0x4C_76.x);//0x1FECCE 16-bit sub + movsx
					v8 = (signed __int16)(v6x->position_0x4C_76.y - a1x->position_0x4C_76.y);//0x1FECE4 16-bit sub + movsx
					v13 = v8 * v8 + v7 * v7;
					//result = v13;
					if (v13 <= v11 && !(v6x->flags_0xc_12_15.byte[0] & 0x20))
					{
						v9 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
						v10 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v6x->position_0x4C_76);
						//result = (unsigned __int16)AngleDifference_582B0(a1x->word_0x1C_28, v10);
						if (AngleDifference_582B0(a1x->yaw_0x1C_28, v10) < v9)
						{
							//result = v13;
							if (v13 < v12)
							{
								v5x = v6x;
								v12 = v13;
							}
						}
					}
					v6x = v6x->next_0;
				}
				if (v5x)
				{
					a1x->targetId_0x96_150 = v5x - world_D41A0.entities_0x6E8E;
					a1x->StageVar2_0x49_73 = 10;
					/*result = */CreatureScriptResume_1E040(a1x, a2);
				}
			}
		}
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001DDA0) --------------------------------------------------------
void CreatureScriptGoToPosition_1DDA0(type_entity_0x6E8E* a1x, __int16 a2)//1feda0
{
	signed int v2x; // eax
	__int16 v3; // ax
	int i; // edx
	int v5; // edx
	type_entity_0x6E8E* v6x; // ecx
	type_entity_0x6E8E* v7x; // eax
	char v8; // al
	char v9; // cl
	char v10; // al
	unsigned __int16 v11; // ax
	type_entity_0x6E8E* jx; // ecx
	axis_3d v14x; // [esp+0h] [ebp-Ch]
	//char v16; // [esp+8h] [ebp-4h]

	//uint8_t origbyte20 = 0;
	//uint8_t remakebyte20 = 0;
	//int comp20;
	v2x = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v3;
		v2x = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52 && a1x->model_0x40_64 != 27)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				v2x = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v2x = 2;
	}
	switch (v2x)
	{
	case 0:
	{
		MoveCreature_1B8C0(a1x);
		if (!(a1x->instanceIndex_0x3E_62 & 7))
		{
			v14x.x = world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72].target_0x3647C_4.axis.x;
			v14x.y = world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72].target_0x3647C_4.axis.y;
			if (!(a1x->flags_0xc_12_15.byte[2] & 4))
			{
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v14x);
				if (!(a1x->instanceIndex_0x3E_62 & 0x3F))
				{
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					v11 = a1x->rand_0x14_20;
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v11 % 0x9Du) / 79) - 1);
					a1x->roll_0x20_32 &= 0x7ff;
				}
			}
			//v16 = 0;
			for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				if (jx->id_0x1A_26 != a1x->id_0x1A_26)
				{
					if (abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch)
					{
						if (abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
						{
							a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
							//v16 = 1;
							return;
						}
					}
				}
			}
		}
		break;
	}
	case 1:
	{
		v7x = Entities_EA3E4[a1x->hitById_0x26_38];
		if (v7x->class_0x3F_63 != a1x->class_0x3F_63 || (v8 = v7x->model_0x40_64, v9 = a1x->model_0x40_64, v8 != v9))
			v10 = 1;
		else
			v10 = v9 ^ v8;
		if (v10)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			a1x->StageVar2_0x49_73 = 10;
			CreatureScriptResume_1E040(a1x, a2);
		}
		KeepCreatureAboveTerrain_1EEE0(a1x);
		break;
	}
	case 2:
	{
		a1x->actionIndex_0x45_69 = a2 + 4;
		break;
	}
	}
	return;
}

//----- (0001E000) --------------------------------------------------------
void CreatureScriptCmd6_1E000(type_entity_0x6E8E* a1x, __int16 a2)//1ff000
{
	CreatureScriptIdle_1E1C0(a1x, a2);
}

//----- (0001E020) --------------------------------------------------------
void CreatureScriptCmd7_1E020(type_entity_0x6E8E* a1x, __int16 a2)//1ff020
{
	CreatureScriptIdle_1E1C0(a1x, a2);
}

//----- (0001E040) --------------------------------------------------------
void CreatureScriptResume_1E040(type_entity_0x6E8E* a1x, char a2)//1ff040
{
	//int result; // eax
	char v3; // bl

	//result = a1x->dword_0xA0_160;
	if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
		v3 = a2 + 6;
	else
		v3 = a2 + 2;
	a1x->actionIndex_0x45_69 = v3;
	//return result;
}

//----- (0001E1C0) --------------------------------------------------------
void CreatureScriptIdle_1E1C0(type_entity_0x6E8E* a1x, __int16 a2)//1ff1c0
{
	//unsigned int v2; // eax
	int v2x;
	__int16 v3; // ax
	int i; // edx
	int v5; // ecx
	type_entity_0x6E8E* v6x; // edx
	type_entity_0x6E8E* v7x; // eax
	char v8; // al
	char v9; // cl
	char v10; // al

	v2x = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v3;
		v2x = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52 && a1x->model_0x40_64 != 27)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				v2x = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v2x = 2;
	}
	if (v2x < 1)
	{
		if (!v2x)
		{
			//v2 = a1x->dword_0xA0_160;
			if (!(a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 2))
			{
				MoveCreature_1B8C0(a1x);
				//LOBYTE(v2) = a1x->instanceIndex_0x3E_62;
				if (!(a1x->instanceIndex_0x3E_62 & 0xF) && !(a1x->flags_0xc_12_15.byte[2] & 4))
				{
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					//v2 = a1x->word_0x14_20 % 0x71u + 142 + a1x->roll_0x20_32;
					a1x->roll_0x20_32 = a1x->rand_0x14_20 % 0x71u + 142 + a1x->roll_0x20_32;
					a1x->roll_0x20_32 &= 0x7ff;
				}
			}
		}
	}
	else if (v2x <= 1)
	{
		v7x = Entities_EA3E4[a1x->hitById_0x26_38];
		if (v7x->class_0x3F_63 != a1x->class_0x3F_63 || (v8 = v7x->model_0x40_64, v9 = a1x->model_0x40_64, v8 != v9))
			v10 = 1;
		else
			v10 = v9 ^ v8;
		if (v10)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			a1x->StageVar2_0x49_73 = 10;
			CreatureScriptResume_1E040(a1x, a2);
		}
		/*LOBYTE(v2) = */KeepCreatureAboveTerrain_1EEE0(a1x);
	}
	else if (v2x == 2)
	{
		//LOBYTE(v2) = a2 + 4;
		a1x->actionIndex_0x45_69 = a2 + 4;
	}
	//return v2;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001E320) --------------------------------------------------------
void CreatureScriptChaseTarget_1E320(type_entity_0x6E8E* a1x)//1ff320
{
	unsigned int v1; // eax
	type_entity_0x6E8E* v1x; // eax
	__int16 v2; // dx

	MoveCreature_1B8C0(a1x);
	if (a1x->life_0x8 < 0)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;
	}
	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (v1x <= Entities_EA3E4[0] || v1x->life_0x8 < 0 || v1x->flags_0xc_12_15.byte[1] & 4)
		goto LABEL_14;
	//LOWORD(v1) = Maths::GetYawAngle_581E0((uint16_t*)(a1 + 76), (uint16_t*)(v1 + 76));
	v2 = a1x->actSpeed_0x82_130;
	a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
	v2 -= 8;
	a1x->actSpeed_0x82_130 = v2;
	if (v2 <= 16)
	{
		LOBYTE(v1) = a1x->model_0x40_64;
		if ((unsigned __int8)v1 < 0x13u)
		{
			if (!(x_BYTE)v1)
				a1x->actSpeed_0x82_130 = 30;
		}
		else if ((unsigned __int8)v1 <= 0x13u)
		{
			a1x->actSpeed_0x82_130 = 76;
		}
		else if ((x_BYTE)v1 == 21)
		{
			a1x->actSpeed_0x82_130 = 96;
		}
	LABEL_14:
		a1x->StageVar2_0x49_73 = 16;
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001E3E0) --------------------------------------------------------
int FindScriptGroupLeader_1E3E0(type_entity_0x6E8E* a1x)//1ff3e0
{
	type_entity_0x6E8E* v1x; // edi
	int16_t v2x; // ebx
	type_entity_0x6E8E* ix; // eax
	type_entity_0x6E8E* jx; // esi
	unsigned int v6; // eax
	unsigned int v8; // [esp+0h] [ebp-4h]

	v8 = -1;
	v1x = 0;
	v2x = 0;
	if (world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72].stage_0x3647A_1 & 1)
	{
		for (ix = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; ix > Entities_EA3E4[0] && !v2x; ix = ix->next_0)
		{
			if (ix->StageVar1_0x48_72 == a1x->StageVar1_0x48_72)
			{
				if (ix->nextStage_0x4A_74)
					v2x = ix->nextStage_0x4A_74;
			}
		}
	}
	if (!v2x)
	{
		for (jx = gameState_D41A4.creatureLists_38403x[world_D41A0.StageVars2_0x365F4[a1x->StageVar1_0x48_72].target_0x3647C_4.axis.x]; jx > Entities_EA3E4[0]; jx = jx->next_0)
		{
			v6 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &jx->position_0x4C_76);
			if (v6 < v8)
			{
				v8 = v6;
				v1x = jx;
			}
		}
		if (v1x)
			v2x = v1x - world_D41A0.entities_0x6E8E;
	}
	return v2x;
}

//----- (0001E4D0) --------------------------------------------------------
void CreatureScriptFollowParent_1E4D0(type_entity_0x6E8E* a1x)//1ff4d0
{
	type_entity_0x6E8E* v1x; // ebx
	unsigned __int8 v2; // al
	int v3; // edx
	//int result; // eax

	v1x = Entities_EA3E4[a1x->parentId_0x28_40];
	if (v1x <= Entities_EA3E4[0] || v1x->life_0x8 < 0)
		DisableEntityDrawing04_57F10(a1x);
	predictedAxis_EB398ar.x = v1x->position_0x4C_76.x;
	v2 = a1x->model_0x40_64;
	predictedAxis_EB398ar.y = v1x->position_0x4C_76.y;
	v3 = v1x->position_0x4C_76.z;
	if (v2 >= 0x10u)
	{
		if (v2 <= 0x10u)
		{
			v3 -= 896;
		}
		else if (v2 > 0x13u && v2 == 25)
		{
			v3 -= 512;
		}
	}
	if (v3 <= 0)
		LOWORD(v3) = 0;
	predictedAxis_EB398ar.z = v3;
	/*result = */CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	a1x->roll_0x20_32 = v1x->roll_0x20_32;
	a1x->yaw_0x1C_28 = v1x->yaw_0x1C_28;
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (0001E580) --------------------------------------------------------
void CreatureScriptEscortParent_1E580(type_entity_0x6E8E* a1x, __int16 a2)//1ff580
{
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // esi
	unsigned int v4; // edi
	__int16 v5; // si
	char v6; // [esp+0h] [ebp-4h]

	v6 = 0;
	v2x = Entities_EA3E4[a1x->parentId_0x28_40];
	if (v2x <= Entities_EA3E4[0] || v2x->life_0x8 < 0 || v2x->flags_0xc_12_15.byte[1] & 4)
	{
		a1x->stage_0x2E_46 = 0;
	}
	else if (a1x->StageVar2_0x49_73 == 13)
	{
		--a1x->stage_0x2E_46;
	}
	if (a1x->stage_0x2E_46 <= 0)
	{
		v6 = 1;
	}
	else
	{
		v3x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v3x > Entities_EA3E4[0] && (v3x->life_0x8 <= 0 || v3x->flags_0xc_12_15.byte[1] & 4))
		{
			a1x->targetId_0x96_150 = 0;
			v3x = Entities_EA3E4[0];
		}
		if (v3x <= Entities_EA3E4[0] && !(a1x->instanceIndex_0x3E_62 & 7))
		{
			v3x = FindNearestOtherEntity_16FC0(v2x, v2x);
			a1x->targetId_0x96_150 = v3x - world_D41A0.entities_0x6E8E;
		}
		if (v3x <= Entities_EA3E4[0])
		{
			a1x->targetId_0x96_150 = a1x->parentId_0x28_40;
			CreatureIdleNoHerd_1E700(a1x, a2);
			v5 = a1x->stage_0x2E_46;
			a1x->targetId_0x96_150 = 0;
			a1x->stage_0x2E_46 = v5 - 4;
		}
		else
		{
			CreatureIdleNoHerd_1E700(a1x, a2);
			if (!(a1x->instanceIndex_0x3E_62 & 7))
			{
				v4 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Distance3D_583F0(&a1x->position_0x4C_76, &v3x->position_0x4C_76) < v4)
					a1x->actionIndex_0x45_69 = a2 + 2;
			}
		}
	}
	if (v6)
	{
		if (a1x->StageVar2_0x49_73 == 13)
			CreateEntity_4A190(&a1x->position_0x4C_76, 10, 73);
		CreateEntity_4A190(&a1x->position_0x4C_76, 10, 0);
		DisableEntityDrawing04_57F10(a1x);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001E700) --------------------------------------------------------
void CreatureIdleNoHerd_1E700(type_entity_0x6E8E* a1x, char a2)//1ff700
{
	unsigned int v2; // eax
	__int16 v3; // ax
	int i; // edx
	int v5; // edx
	type_entity_0x6E8E* v6x; // ecx
	type_entity_0x6E8E* v7x; // edx
	char v8; // al
	char v9; // al
	type_entity_0x6E8E* v10x; // eax
	__int16 v11; // ax
	char v12; // ch
	unsigned __int16 v13; // ax
	type_entity_0x6E8E* jx; // ecx

	v2 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v3;
		v2 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52 && a1x->model_0x40_64 != 27)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				v2 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v2 = 2;
	}
	if (v2 < 1)
	{
		if (!v2)
		{
			MoveCreature_1B8C0(a1x);
			if (!(a1x->instanceIndex_0x3E_62 & 7))
			{
				v10x = Entities_EA3E4[a1x->targetId_0x96_150];
				if (v10x > Entities_EA3E4[0])
				{
					if (!(a1x->flags_0xc_12_15.byte[2] & 4))
					{
						v11 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v10x->position_0x4C_76);
						v12 = a1x->instanceIndex_0x3E_62;
						a1x->roll_0x20_32 = v11;
						if (!(v12 & 0x3F))
						{
							a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
							v13 = a1x->rand_0x14_20;
							a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
							a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v13 % 0x9Du) / 79) - 1);
							a1x->roll_0x20_32 &= 0x7ff;
						}
					}
					for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
					{
						if (jx->id_0x1A_26 != a1x->id_0x1A_26
							&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch
							&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
						{
							a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
							return;
						}
					}
				}
			}
		}
	}
	else if (v2 <= 1)
	{
		MoveCreature_1B8C0(a1x);
		v7x = Entities_EA3E4[a1x->hitById_0x26_38];
		v8 = 1;
		if (v7x->class_0x3F_63 == a1x->class_0x3F_63 && v7x->model_0x40_64 == a1x->model_0x40_64)
			v8 = 0;
		if (a1x->parentId_0x28_40 == a1x->hitById_0x26_38)
			v8 = 0;
		if (v8)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			if (a1x->typeParams_0xA0_160x->flags_160_0x20_32 & 8)
				v9 = a2 + 6;
			else
				v9 = a2 + 2;
			a1x->actionIndex_0x45_69 = v9;
			SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0x13u, 1);
		}
	}
	else if (v2 == 2)
	{
		a1x->stage_0x2E_46 = 1;
	}
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001E9C0) --------------------------------------------------------
void CreatureScriptCmd14_1E9C0(type_entity_0x6E8E* a1x, __int16 a2)//1ff9c0
{
	__int16 v2; // cx
	type_entity_0x6E8E* v3x; // esi
	char v4; // al
	int i; // eax
	char v6; // dl
	type_entity_0x6E8E* v7x; // eax
	unsigned __int16 v8; // di
	unsigned __int16 v9; // ax
	type_entity_0x6E8E* v10x; // esi
	unsigned int v11; // edi
	//int v12; // esi
	//int v13; // edx
	int j; // eax
	type_entity_0x6E8E* v15x; // eax
	signed __int16 v16; // [esp+0h] [ebp-4h]

	//fix
	v7x = 0;
	//fix

	v2 = a1x->stage_0x2E_46 - 1;
	v16 = 0;
	a1x->stage_0x2E_46 = v2;
	if (v2 <= 0)
		goto LABEL_66;
	v3x = Entities_EA3E4[a1x->parentId_0x28_40];
	v4 = 0;
	if (v2 <= a1x->maxStage_0x30_48 - 16 && a1x->stage_0x2E_46 >= 16)
	{
		if ((a1x->instanceIndex_0x3E_62 & 0x1C) == 16)
			v4 = a1x->instanceIndex_0x3E_62 & 3;
	}
	else
	{
		v4 = a1x->instanceIndex_0x3E_62 & 1 ? 0 : 2;
	}
	switch (v4)
	{
	case 0:
		a1x->flags_0xc_12_15.byte[3] &= 0xF9u;
		break;
	case 1:
	case 3:
		a1x->flags_0xc_12_15.byte[3] = (a1x->flags_0xc_12_15.byte[3] | 2) & 0xFB;
		break;
	case 2:
		a1x->flags_0xc_12_15.byte[3] = a1x->flags_0xc_12_15.byte[3] & 0xF9 | 4;
		break;
	default:
		break;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v7x->nextLinkedId_0x34_52)
		{
			v7x = Entities_EA3E4[i];
			if (v7x == Entities_EA3E4[0])
				break;
			v6 = 0;
			if (a1x->stage_0x2E_46 <= a1x->maxStage_0x30_48 - 16 && a1x->stage_0x2E_46 >= 16)
			{
				if ((a1x->instanceIndex_0x3E_62 & 0x1C) == 16)
					v6 = a1x->instanceIndex_0x3E_62 & 3;
			}
			else if (a1x->instanceIndex_0x3E_62 & 1)
			{
				v6 = 0;
			}
			else
			{
				v6 = 2;
			}
			switch (v6)
			{
			case 0:
				v7x->flags_0xc_12_15.byte[3] &= 0xF9u;
				break;
			case 1:
			case 3:
				v7x->flags_0xc_12_15.byte[3] = (v7x->flags_0xc_12_15.byte[3] | 2) & 0xFB;
				break;
			case 2:
				v7x->flags_0xc_12_15.byte[3] = v7x->flags_0xc_12_15.byte[3] & 0xF9 | 4;
				break;
			default:
				continue;
			}
		}
	}
	if (v3x <= Entities_EA3E4[0] || v3x->life_0x8 <= 0)
	{
	LABEL_66:
		v16 = 1;
	}
	else
	{
		v8 = a1x->targetId_0x96_150;
		a1x->targetId_0x96_150 = a1x->parentId_0x28_40;
		CreatureIdleNoHerd_1E700(a1x, a2);
		v9 = v3x->targetId_0x96_150;
		if (!v9 && v3x->hitById_0x26_38)
			v9 = v3x->hitById_0x26_38;
		if (v9 && v8 != v9)
			v8 = v9;
		a1x->targetId_0x96_150 = v8;
		if (v8)
		{
			v10x = Entities_EA3E4[v8];
			if (v10x > Entities_EA3E4[0]
				&& (v10x->parentId_0x28_40 == a1x->parentId_0x28_40 && v10x->StageVar2_0x49_73 == 14
					|| v10x->life_0x8 <= 0
					|| v10x->flags_0xc_12_15.byte[1] & 4))
			{
				a1x->targetId_0x96_150 = 0;
				v10x = Entities_EA3E4[0];
			}
			if (v10x > Entities_EA3E4[0])
			{
				v11 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Distance3D_583F0(&a1x->position_0x4C_76, &v10x->position_0x4C_76) < v11)
				{
					a1x->targetId_0x96_150 = v10x - world_D41A0.entities_0x6E8E;
					a1x->actionIndex_0x45_69 = a2 + 2;
					SendSpellMessage_6D8B0(a1x->parentId_0x28_40, 0x18u, 1);
				}
			}
		}
	}
	if (v16)
	{
		a1x->StageVar2_0x49_73 = 10;
		a1x->targetId_0x96_150 = 0;
		a1x->parentId_0x28_40 = 0;
		//v12 = a1x->word_0x2E_46;
		//v13 = a1x->word_0x30_48 - 16;
		a1x->StageVar1_0x48_72 = a1x->nextStage_0x4A_74;
		//if (v12 <= v13)
		//	a1x->word_0x2E_46;
		switch (0)
		{
		case 0:
			a1x->flags_0xc_12_15.byte[3] &= 0xF9u;
			break;
		case 1:
		case 3:
			a1x->flags_0xc_12_15.byte[3] = (a1x->flags_0xc_12_15.byte[3] | 2) & 0xFB;
			break;
		case 2:
			a1x->flags_0xc_12_15.byte[3] = a1x->flags_0xc_12_15.byte[3] & 0xF9 | 4;
			break;
		default:
			break;
		}
		if (a1x->nextLinkedId_0x34_52)
		{
			for (j = a1x->nextLinkedId_0x34_52; ; j = v15x->nextLinkedId_0x34_52)
			{
				v15x = Entities_EA3E4[j];
				if (v15x == Entities_EA3E4[0])
					break;
				// if (a1x->word_0x2E_46 <= a1x->word_0x30_48 - 16)
				// 	a1x->word_0x2E_46;
				switch (0)
				{
				case 0:
					v15x->flags_0xc_12_15.byte[3] &= 0xF9u;
					break;
				case 1:
				case 3:
					v15x->flags_0xc_12_15.byte[3] = (v15x->flags_0xc_12_15.byte[3] | 2) & 0xFB;
					break;
				case 2:
					v15x->flags_0xc_12_15.byte[3] = v15x->flags_0xc_12_15.byte[3] & 0xF9 | 4;
					break;
				default:
					continue;
				}
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001ED30) --------------------------------------------------------
type_entity_0x6E8E* GetCreatureTarget_1ED30(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//1ffd30
{
	type_entity_0x6E8E* v2x; // ebx
	char v3; // dl
	int i; // edx
	char v5; // cl
	char v6; // cl
	type_entity_0x6E8E* v7x; // edx
	__int16 v8; // dx
	type_entity_0x6E8E* v9x; // eax
	unsigned __int16 v10; // ax

	v2x = a2x;
	if (a1x->StageVar2_0x49_73 == 14)
	{
		v3 = 0;
		if (a1x->maxStage_0x30_48 - 16 >= a1x->stage_0x2E_46 && a1x->stage_0x2E_46 >= 16)
		{
			if ((a1x->instanceIndex_0x3E_62 & 0x1C) == 16)
				v3 = a1x->instanceIndex_0x3E_62 & 3;
		}
		else if (a1x->instanceIndex_0x3E_62 & 1)
		{
			v3 = 0;
		}
		else
		{
			v3 = 2;
		}
		switch (v3)
		{
		case 0:
			a1x->flags_0xc_12_15.byte[3] &= 0xF9u;
			break;
		case 1:
		case 3:
			a1x->flags_0xc_12_15.byte[3] = (a1x->flags_0xc_12_15.byte[3] | 2) & 0xFB;
			break;
		case 2:
			a1x->flags_0xc_12_15.byte[3] = a1x->flags_0xc_12_15.byte[3] & 0xF9 | 4;
			break;
		default:
			break;
		}
		if (a1x->nextLinkedId_0x34_52)
		{
			for (i = a1x->nextLinkedId_0x34_52; ; i = v7x->nextLinkedId_0x34_52)
			{
				v7x = Entities_EA3E4[i];
				if (v7x == Entities_EA3E4[0])
					break;
				v5 = 0;
				if (a1x->stage_0x2E_46 <= a1x->maxStage_0x30_48 - 16 && a1x->stage_0x2E_46 >= 16)
				{
					if ((a1x->instanceIndex_0x3E_62 & 0x1C) == 16)
						v5 = a1x->instanceIndex_0x3E_62 & 3;
				}
				else if (a1x->instanceIndex_0x3E_62 & 1)
				{
					v5 = 0;
				}
				else
				{
					v5 = 2;
				}
				switch (v5)
				{
				case 0:
					v7x->flags_0xc_12_15.byte[3] &= 0xF9u;
					break;
				case 1:
				case 3:
					v6 = (v7x->flags_0xc_12_15.byte[3] | 2) & 0xFB;
					goto LABEL_26;
				case 2:
					v6 = v7x->flags_0xc_12_15.byte[3] & 0xF9 | 4;
				LABEL_26:
					v7x->flags_0xc_12_15.byte[3] = v6;
					break;
				default:
					continue;
				}
			}
		}
		v8 = a1x->stage_0x2E_46 - 1;
		a1x->stage_0x2E_46 = v8;
		if (v8 <= 0)
			return Entities_EA3E4[0];
		v9x = Entities_EA3E4[a1x->parentId_0x28_40];
		if (v9x > Entities_EA3E4[0])
		{
			if (a2x == v9x)
				return Entities_EA3E4[0];
			if (v9x->targetId_0x96_150)
			{
				v10 = v9x->targetId_0x96_150;
			}
			else
			{
				if (!v9x->hitById_0x26_38)
					return v2x;
				v10 = v9x->hitById_0x26_38;
			}
			if (Entities_EA3E4[v10] != a2x)
				return Entities_EA3E4[0];
		}
	}
	return v2x;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001EEE0) --------------------------------------------------------
void KeepCreatureAboveTerrain_1EEE0(type_entity_0x6E8E* a1x)//1ffee0
{
	//signed __int16 *v1; // eax
	__int16 v2; // ST10_2
	int v3; // ST0C_4
	int v4; // ST08_4
	__int16 v5; // ax

	//v1 = *(signed __int16 **)(a1x->dword_0xA0_160);
	v2 = a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
	v3 = a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
	v4 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
	v5 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	/*return */ClampAboveTerrain_580E0(&a1x->position_0x4C_76, v5, v4, v3, v2);
}

//----- (0001EF20) --------------------------------------------------------
void DragonIdle_1EF20(type_entity_0x6E8E* a1x)//1fff20
{
	CreatureIdle_1BD90(a1x, 0);
}

//----- (0001EF40) --------------------------------------------------------
void DragonWander_1EF40(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x)//1fff40
{
	CreatureWander_1BF90(a2x, 0);
	UpdateDragonBody_1F0C0(/*a1x,*/ a2x);
	ApplyDragonGravity_1F040(a2x);
}

//----- (0001EF70) --------------------------------------------------------
void DragonAttack_1EF70(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x)//1fff70
{
	if ((unsigned __int16)CreatureAttack_1C310(a2x, 0, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureShootFireball_1CC20))
		PrepareEventSound_6E450(a2x - world_D41A0.entities_0x6E8E, -1, 8);
	UpdateDragonBody_1F0C0(/*a1x,*/ a2x);
	ApplyDragonGravity_1F040(a2x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0001EFD0) --------------------------------------------------------
void DragonFollow_1EFD0(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x)//1fffd0
{
	CreatureFollowLeader_1C560(a2x, 0);
	UpdateDragonBody_1F0C0(/*a1x,*/ a2x);
	ApplyDragonGravity_1F040(a2x);
}

//----- (0001F000) --------------------------------------------------------
void DragonPreKill_1F000(type_entity_0x6E8E* a1x)//200000
{
	PreKillEntity_1C890(a1x, 0);
}

//----- (0001F020) --------------------------------------------------------
void DragonKill_1F020(type_entity_0x6E8E* a1x)//200020
{
	KillEntity_1C930(a1x);
}

//----- (0001F040) --------------------------------------------------------
void ApplyDragonGravity_1F040(type_entity_0x6E8E* a1x)//200040
{
	__int16 v1; // ax
	int result; // eax

	a1x->position_0x4C_76.z += a1x->param_0x10_16;
	v1 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	a1x->param_0x10_16 -= 5;
	result = v1 + 256;
	if (a1x->position_0x4C_76.z >= result)
	{
		if (isCaveLevel_D41B6)
		{
			result = GetCeilingAlt_10C60(&a1x->position_0x4C_76);
			if (a1x->position_0x4C_76.z > (signed __int16)result - 256)
				a1x->param_0x10_16 = -150;
		}
	}
	else
	{
		a1x->param_0x10_16 = 150;
	}
	//return result;
}
// D41B6: using guessed type char x_BYTE_D41B6;

//----- (0001F0C0) --------------------------------------------------------
void UpdateDragonBody_1F0C0(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x)//2000c0
{
	char v2; // ah
	int v3; // edi
	int i; // eax
	int v5; // eax
	//char v6; // cl
	type_entity_0x6E8E* v7x; // esi
	__int16 v8; // ax
	int v9; // [esp+0h] [ebp-14h]
	int v10; // [esp+4h] [ebp-10h]
	int v11; // [esp+8h] [ebp-Ch]
	int v12; // [esp+Ch] [ebp-8h]
	char v13; // [esp+10h] [ebp-4h]

	type_entity_0x6E8E* a1x = 0;

	v2 = a2x->fontTypeIndex_0x3D_61;
	if (v2)
	{
		a2x->fontTypeIndex_0x3D_61 = v2 - 1;
		if (a2x->levelPar3_0x46_70)
		{
			if (a2x->param2_0x2C_44)
			{
				v7x = Entities_EA3E4[a2x->ownerPlayerId_0x24_36];
				if (v7x <= Entities_EA3E4[0] || v7x->life_0x8 < 0 || v7x->flags_0xc_12_15.byte[1] & 4)
				{
					a2x->levelPar3_0x46_70 = 0;
					a2x->ownerPlayerId_0x24_36 = 0;
				}
				else
				{
					if (a2x->ownerPlayerId_0x24_36 & 1)
						v8 = v7x->yaw_0x1C_28 + 512;
					else
						v8 = v7x->yaw_0x1C_28 - 512;
					predictedAxis_EB398ar = a2x->position_0x4C_76;
					MoveEntity_57FA0(&predictedAxis_EB398ar, v8 & 0x7FF, 0, 48 * a2x->param2_0x2C_44);
					CopyEntityPosition_57CF0(a2x, &predictedAxis_EB398ar);
					a2x->param2_0x2C_44--;
				}
			}
			else
			{
				a2x->levelPar3_0x46_70 = 0;
				a2x->ownerPlayerId_0x24_36 = 0;
			}
		}
		else
		{
			v11 = (a2x->position_0x4C_76.x + 128) >> 8;
			v12 = (a2x->position_0x4C_76.y + 128) >> 8;
			v3 = AddE7EE0x_10080(0, 4);
			if (v3)
			{
				v13 = 0;
			LABEL_11:
				while (!v13 && GetNextSearchOffset_10130(v3, &v10, &v9) == 1)
				{
					for (i = mapEntityIndex_15B4E0[((unsigned __int8)(v9 + v12) << 8) + (unsigned __int8)(v10 + v11)];
						;
						i = a1x->oldMapEntity_0x16_22)
					{
						a1x = Entities_EA3E4[i];
						if (a1x == Entities_EA3E4[0])
							break;
						if (a1x->class_0x3F_63 == 9 && a1x->targetId_0x96_150 == a2x->id_0x1A_26)
						{
							v13 = 1;
							goto LABEL_11;
						}
					}
				}
				ResetEvent08_10100(v3);
				if (v13)
				{
					v5 = a1x - world_D41A0.entities_0x6E8E;
					//v6 = a2x->byte_0x46_70;
					a2x->param2_0x2C_44 = 5;
					a2x->levelPar3_0x46_70++;// = v6 + 1;
					a2x->ownerPlayerId_0x24_36 = v5;
				}
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (0001F300) --------------------------------------------------------
void DragonScripted_1F300(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x)//200300
{
	//int result; // eax

	CreatureRunScript_1D5D0(a2x, 0);
	//result = (unsigned __int8)(a2x->byte_0x49_73 - 1);
	switch (a2x->StageVar2_0x49_73)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 0xA:
	case 0xD:
	case 0xE:
	case 0x10:
		UpdateDragonBody_1F0C0(/*a1x,*/ a2x);
		goto LABEL_3;
	case 0x11:
	LABEL_3:
		/*result = */ApplyDragonGravity_1F040(a2x);
		break;
	default:
		return;// result;
	}
	//return result;
}

//----- (0001F340) --------------------------------------------------------
void GoatIdle_1F340(type_entity_0x6E8E* a1x)//200340
{
	CreatureIdle_1BD90(a1x, 8);
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	if (!(a1x->rand_0x14_20 % 0x4Du))
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 46);
	if (a1x->actionIndex_0x45_69 == 14)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0001F3C0) --------------------------------------------------------
void GoatWander_1F3C0(type_entity_0x6E8E* a1x)//2003c0
{
	CreatureWander_1BF90(a1x, 8);
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	if (!(a1x->rand_0x14_20 % 0x4Du))
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 46);
	if (a1x->actionIndex_0x45_69 == 14)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0001F440) --------------------------------------------------------
void GoatAttack_1F440(type_entity_0x6E8E* a1x)//200440
{
	a1x->actionIndex_0x45_69 = 14;
	a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	HitGoat_1F530(a1x);
}

//----- (0001F470) --------------------------------------------------------
void GoatFollow_1F470(type_entity_0x6E8E* a1x)//200470
{
	CreatureFollowLeader_1C560(a1x, 8u);
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	if (!(a1x->rand_0x14_20 % 0x4Du))
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 46);
	if (a1x->actionIndex_0x45_69 == 14)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
}

//----- (0001F4F0) --------------------------------------------------------
void PreKillGoat_1F4F0(type_entity_0x6E8E* entity)//2004f0
{
	PreKillEntity_1C890(entity, 8);
}

//----- (0001F510) --------------------------------------------------------
void KillGoat_1F510(type_entity_0x6E8E* entity)//200510
{
	KillEntity_1C930(entity);
}

//----- (0001F530) --------------------------------------------------------
void HitGoat_1F530(type_entity_0x6E8E* entity)//200530
{
	CreatureFlee_1C980(entity, 8);
	if (entity->actionIndex_0x45_69 != 14)
		entity->actSpeed_0x82_130 = entity->maxSpeed_0x86_134;
	entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
	if (!(entity->rand_0x14_20 % 0x2Bu))
		PrepareEventSound_6E450(entity - world_D41A0.entities_0x6E8E, -1, 46);
}

//----- (0001F5B0) --------------------------------------------------------
void GoatRunScript_1F5B0(type_entity_0x6E8E* event)//2005b0
{
	CreatureRunScript_1D5D0(event, 8);
	event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
	if (!(event->rand_0x14_20 % 0x4Du))
		PrepareEventSound_6E450(event - world_D41A0.entities_0x6E8E, -1, 46);
	if (event->actionIndex_0x45_69 == 14)
		event->actSpeed_0x82_130 = event->minSpeed_0x84_132;
	else
		event->actSpeed_0x82_130 = event->maxSpeed_0x86_134;
}

//----- (0001F630) --------------------------------------------------------
void BeeIdle_1F630(type_entity_0x6E8E* a1x)//200630
{
	//signed int result; // eax

	/*result = */CreatureIdle_1BD90(a1x, 16);
	//BYTE1(result) = *(x_BYTE *)(a1 + 69);
	if (a1x->actionIndex_0x45_69 == 18)
		a1x->param_0x10_16 = 1;
	//return result;
}

//----- (0001F660) --------------------------------------------------------
void BeeWander_1F660(type_entity_0x6E8E* a1x)//200660
{
	CreatureWander_1BF90(a1x, 16);
	if (a1x->actionIndex_0x45_69 == 18)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, (a1x->rand_0x14_20 & 1) + 12);
		a1x->param_0x10_16 = 1;
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0001F6D0) --------------------------------------------------------
void BeeAttack_1F6D0(type_entity_0x6E8E* a1x)//2006d0
{
	//int v1; // edx
	int v2; // eax
	unsigned __int16 v3; // dx
	//int v4; // eax
	int16_t v4x;

	//v1 = a1x->dword_0x10_16;
	if (a1x->param_0x10_16)
	{
		v2 = a1x->param_0x10_16;
		a1x->param_0x10_16--;
		if (v2 == 1)
			a1x->actSpeed_0x82_130 = 5 * a1x->minSpeed_0x84_132 >> 1;
	}
	v3 = a1x->targetId_0x96_150;
	if (v3)
	{
		v4x = a1x->position_0x4C_76.z
			- (Entities_EA3E4[v3]->params_0x52_82.yaw
				+ Entities_EA3E4[v3]->position_0x4C_76.z);
		if (a1x->position_0x4C_76.z != Entities_EA3E4[v3]->params_0x52_82.yaw
			+ Entities_EA3E4[v3]->position_0x4C_76.z)
		{
			if (v4x <= 0)
				v4x = -1;
			else
				v4x = 1;
		}
		a1x->position_0x4C_76.z += v4x * a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		if ((unsigned __int16)CreatureAttack_1C310(a1x, 16, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureMeleeAttack1024_1CE80))
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, (a1x->rand_0x14_20 & 1) + 12);
			a1x->actSpeed_0x82_130 = -a1x->maxSpeed_0x86_134;
			a1x->param_0x10_16 = 3 * a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
		}
	}
	else
	{
		a1x->actionIndex_0x45_69 = 17;
	}
	if (a1x->actionIndex_0x45_69 != 18)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001F800) --------------------------------------------------------
void BeeFollow_1F800(type_entity_0x6E8E* a1x)//200800
{
	//char result; // al

	/*result = */CreatureFollowLeader_1C560(a1x, 0x10u);
	if (a1x->actionIndex_0x45_69 == 18)
		a1x->param_0x10_16 = 1;
	//return result;
}

//----- (0001F830) --------------------------------------------------------
void BeePreKill_1F830(type_entity_0x6E8E* a1x)//200830
{
	PreKillEntity_1C890(a1x, 16);
}

//----- (0001F850) --------------------------------------------------------
void BeeKill_1F850(type_entity_0x6E8E* a1x)//200850
{
	KillEntity_1C930(a1x);
}

//----- (0001F8A0) --------------------------------------------------------
void BeeScripted_1F8A0(type_entity_0x6E8E* a1x)//2008a0
{
	int v1; // esi

	CreatureRunScript_1D5D0(a1x, 16);
	if (!(a1x->instanceIndex_0x3E_62 & 7) && (unsigned __int8)(a1x->StageVar2_0x49_73 - 1) <= 8u)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v1 = 2 * ((a1x->rand_0x14_20 % 0x9Du) / 79);
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		a1x->roll_0x20_32 += a1x->rand_0x14_20 % 0x55u * (v1 - 1);
		a1x->roll_0x20_32 &= 0x7ff;
	}
	if (a1x->actionIndex_0x45_69 == 18)
		a1x->param_0x10_16 = 1;
}

//----- (0001F950) --------------------------------------------------------
void WormIdle_1F950(type_entity_0x6E8E* a1x)//200950
{
	CreatureIdle_1BD90(a1x, 24);
}

//----- (0001F970) --------------------------------------------------------
void WormWander_1F970(type_entity_0x6E8E* a1x)//200970
{
	CreatureWander_1BF90(a1x, 24);
}

//----- (0001F990) --------------------------------------------------------
void WormAttack_1F990(type_entity_0x6E8E* a1x)//200990
{
	if ((unsigned __int16)CreatureAttack_1C310(a1x, 24, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureShootFireball_1CC20))
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 8);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0001F9E0) --------------------------------------------------------
void WormFollow_1F9E0(type_entity_0x6E8E* a1x)//2009e0
{
	/*return */CreatureFollowLeader_1C560(a1x, 0x18u);
}

//----- (0001FA00) --------------------------------------------------------
void WormPreKill_1FA00(type_entity_0x6E8E* a1x)//200a00
{
	PreKillEntity_1C890(a1x, 24);
}

//----- (0001FA20) --------------------------------------------------------
void WormKill_1FA20(type_entity_0x6E8E* a1x)//200a20
{
	KillEntity_1C930(a1x);
}

//----- (0001FA50) --------------------------------------------------------
void WormScripted_1FA50(type_entity_0x6E8E* a1x)//200a50
{
	CreatureRunScript_1D5D0(a1x, 24);
}

//----- (0001FA70) --------------------------------------------------------
void ArcherIdle_1FA70(type_entity_0x6E8E* a1x)//200a70
{
	//char result; // al

	/*result = */CreatureIdle_1BD90(a1x, 32);
	if (a1x->actionIndex_0x45_69 == 34)
		/*result = */ArcherStartShooting_20060(a1x);
	//return result;
}

//----- (0001FAA0) --------------------------------------------------------
void ArcherWander_1FAA0(type_entity_0x6E8E* a1x)//200aa0
{
	__int16 v1; // cx
	int jy; // eax
	int jk; // eax
	type_entity_0x6E8E* jz; // eax
	__int16 v3; // ax
	int i; // edx
	int v5; // edi
	type_entity_0x6E8E* v6x; // edx
	unsigned __int16 v7; // ax
	//int v8; // edi
	int v9; // ecx
	type_entity_0x6E8E* v10x; // esi
	type_entity_0x6E8E* v11x; // edi
	int v12; // eax
	signed int v13; // ST14_4
	__int16 v14; // ax
	type_entity_0x6E8E* v15x; // esi
	char v16; // dh
	int v17; // eax
	unsigned int v18; // esi
	type_entity_0x6E8E* v19x; // edi
	int v20; // edx
	unsigned int v21; // edx
	int v22; // eax
	type_entity_0x6E8E* v23x; // edi
	type_entity_0x6E8E* kx; // esi
	int v25; // eax
	signed int v26; // ST14_4
	__int16 v27; // ax
	type_entity_0x6E8E* v28x; // esi
	unsigned int v30; // [esp+0h] [ebp-28h]
	unsigned int v31; // [esp+4h] [ebp-24h]
	unsigned int v32; // [esp+8h] [ebp-20h]
	unsigned int v33; // [esp+14h] [ebp-14h]
	unsigned int v34; // [esp+18h] [ebp-10h]
	unsigned int v35; // [esp+1Ch] [ebp-Ch]
	unsigned int v36; // [esp+20h] [ebp-8h]

	v1 = a1x->mail_0x5E_94.channel[0].source;
	jy = 0;
	a1x->param_0x10_16 = 0;
	if (v1)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v3 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v3;
		jy = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
		{
			v6x = Entities_EA3E4[i];
			if (v6x == Entities_EA3E4[0])
				break;
			v5 = v6x->life_0x8;
			if (v5 < a1x->life_0x8)
			{
				a1x->life_0x8 = v5;
				a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
				jy = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		jy = 2;
	}
	if (jy >= 1)
	{
		if (jy <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			a1x->actionIndex_0x45_69 = 34;
			/*LOBYTE(j) = */KeepCreatureAboveTerrain_1EEE0(a1x);
		}
		else if (jy == 2)
		{
			//LOBYTE(jx) = 36;
			a1x->actionIndex_0x45_69 = 36;
		}
		goto LABEL_68;
	}
	if (!jy)
	{
		MoveCreature_1B8C0(a1x);
		//LOWORD(jx) = (signed __int16)a1x->instanceIndex_0x3E_62 / *(x_WORD *)(a1x->dword_0xA0_160 + 26);
		if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
		{
			if (a1x->targetId_0x96_150)
			{
				//LOWORD(jx) = a1x->targetId_0x96_150;
				v28x = Entities_EA3E4[a1x->targetId_0x96_150];
				if (v28x->class_0x3F_63 != 10 || v28x->model_0x40_64 != 45)
				{
					a1x->targetId_0x96_150 = 0;
				}
				else if (Distance3D_583F0(&a1x->position_0x4C_76, &v28x->position_0x4C_76) > 0x1000)
				{
					//LOWORD(jx) = Maths::GetYawAngle_581E0(&a1x->array_0x4C_76, &v28x->array_0x4C_76);
					a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v28x->position_0x4C_76);
				}
				else
				{
					a1x->param_0x10_16 = 1;
					//LOBYTE(jx) = 36;
					a1x->actionIndex_0x45_69 = 36;
					v28x->param_0x10_16++;
				}
				goto LABEL_68;
			}
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v7 = a1x->rand_0x14_20;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v7 % 0x9Du) / 79) - 1);
			//v8 = a1x->dword_0xA0_160;
			a1x->roll_0x20_32 &= 0x7ff;
			v9 = 4 * a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
			//jx = a1x->instanceIndex_0x3E_62 / v9;
			if (!(a1x->instanceIndex_0x3E_62 % v9))
			{
				//jx = *(signed __int16 *)(v8 + 28) * *(signed __int16 *)(v8 + 28);
				v32 = -1;
				v10x = gameState_D41A4.class3List_38519;
				v11x = 0;
				v31 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				while (v10x > Entities_EA3E4[0])
				{
					v12 = (signed __int16)(v10x->position_0x4C_76.x - a1x->position_0x4C_76.x);
					v35 = (signed __int16)(v10x->position_0x4C_76.y - a1x->position_0x4C_76.y)
						* (signed __int16)(v10x->position_0x4C_76.y - a1x->position_0x4C_76.y)
						+ v12 * v12;
					//v35 = jx;
					if (v35 <= v31 && !(v10x->flags_0xc_12_15.byte[0] & 0x20))
					{
						v13 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
						v14 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v10x->position_0x4C_76);
						//jx = (unsigned __int16)AngleDifference_582B0(a1x->word_0x1C_28, v14);
						if (AngleDifference_582B0(a1x->yaw_0x1C_28, v14) < v13)
						{
							//LOBYTE(jx) = v35;
							if (v35 < v32)
							{
								v11x = v10x;
								v32 = v35;
							}
						}
					}
					v10x = v10x->next_0;
				}
				v15x = v11x;
				if (v11x)
				{
					if (v11x <= Entities_EA3E4[0]
						|| (v16 = v11x->model_0x40_64) != 0 && v16 != 1
						|| /*(jx = v11x->playerData_0xA4_164x, */!v11x->playerData_0xA4_164x->attackedTimer_0x248_584)
					{
						v15x = 0;
					}
				}
				if (!v15x)
				{
					v17 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					v33 = v17 * v17;
					v18 = -1;
					v19x = 0;
					for (jz = gameState_D41A4.creatureLists_38403x[36 / 4]; jz > Entities_EA3E4[0]; jz = jz->next_0)
					{
						v20 = (signed __int16)(jz->position_0x4C_76.x - a1x->position_0x4C_76.x);
						v21 = v20 * v20
							+ (signed __int16)(jz->position_0x4C_76.y - a1x->position_0x4C_76.y)
							* (signed __int16)(jz->position_0x4C_76.y - a1x->position_0x4C_76.y);
						if (v21 <= v33 && v21 < v18)
						{
							v19x = jz;
							v18 = v21;
						}
					}
					v15x = v19x;
				}
				if (v15x)
				{
					if (v15x->class_0x3F_63 == 10 && v15x->model_0x40_64 == 45)
						goto LABEL_68;
					a1x->targetId_0x96_150 = v15x - world_D41A0.entities_0x6E8E;
					jk = 34;
				LABEL_60:
					a1x->actionIndex_0x45_69 = jk;
					goto LABEL_68;
				}
				v22 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				v23x = 0;
				v30 = v22 * v22;
				//jx = a1x->byte_0x40_64;
				v34 = -1;
				for (kx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; kx > Entities_EA3E4[0]; kx = kx->next_0)
				{
					if (!kx->leaderId_0x32_50 && kx != a1x)
					{
						v25 = (signed __int16)(kx->position_0x4C_76.x - a1x->position_0x4C_76.x);
						v36 = (signed __int16)(kx->position_0x4C_76.y - a1x->position_0x4C_76.y)
							* (signed __int16)(kx->position_0x4C_76.y - a1x->position_0x4C_76.y)
							+ v25 * v25;
						//v36 = jx;
						if (v36 <= v30)
						{
							v26 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
							v27 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &kx->position_0x4C_76);
							//jx = (unsigned __int16)AngleDifference_582B0(a1x->word_0x1C_28, v27);
							if (AngleDifference_582B0(a1x->yaw_0x1C_28, v27) < v26)
							{
								//LOBYTE(jx) = v36;
								if (v36 < v34)
								{
									v23x = kx;
									v34 = v36;
								}
							}
						}
					}
				}
				if (v23x)
				{
					a1x->leaderId_0x32_50 = v23x - world_D41A0.entities_0x6E8E;
					jk = 35;
					goto LABEL_60;
				}
			}
		}
	}
LABEL_68:
	if (a1x->actionIndex_0x45_69 == 34)
		/*LOBYTE(jx) = */ArcherStartShooting_20060(a1x);
	//return jx;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0001FF40) --------------------------------------------------------
void AddArcher0504_1FF40(type_entity_0x6E8E* event)//200f40
{
	CreatureAttack_1C310(event, 32, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureShootArrow_1CCE0);
	if (event->actionIndex_0x45_69 != 34)
	{
		ArcherResetAnimation_200F0(event);
		return;
	}
	if (!(event->instanceIndex_0x3E_62 % event->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
	{
		type_entity_0x6E8E* event2 = Entities_EA3E4[event->targetId_0x96_150];
		if (event2 > Entities_EA3E4[0])
		{
			if (!event2->model_0x40_64 || event2->model_0x40_64 == 1)
			{
				Entities_EA3E4[event->targetId_0x96_150]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
			}
		}
	}
}

//----- (0001FFE0) --------------------------------------------------------
void ArcherFollow_1FFE0(type_entity_0x6E8E* a1x)//200fe0
{
	//char result; // al

	CreatureFollowLeader_1C560(a1x, 0x20u);
	if (a1x->actionIndex_0x45_69 == 34)
		ArcherStartShooting_20060(a1x);
	//return result;
}

//----- (00020010) --------------------------------------------------------
void HitArcher_20010(type_entity_0x6E8E* a1x)//201010
{
	//int result; // eax

	if (a1x->param_0x10_16)
		DisableEntityDrawing04_57F10(a1x);
	else
		PreKillEntity_1C890(a1x, 32);
	//return result;
}

//----- (00020040) --------------------------------------------------------
void KillArcher_20040(type_entity_0x6E8E* a1x)//201040
{
	KillEntity_1C930(a1x);
}

//----- (00020060) --------------------------------------------------------
void ArcherStartShooting_20060(type_entity_0x6E8E* entity)//201060 //tex 121,11
{
	entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
	entity->actSpeed_0x82_130 = 0;
	if (entity->rand_0x14_20 % 0x14u <= 10)
		SetEntityIndexAndRot_49CD0(entity, 206);
	else
		SetEntityIndexAndRot_49CD0(entity, 1);
	SetEntityShiftRot_49EA0(entity, 128, 256);
	entity->xtype_0x41_65 = Entities_EA3E4[entity->targetId_0x96_150]->class_0x3F_63;
	entity->xsubtype_0x42_66 = Entities_EA3E4[entity->targetId_0x96_150]->model_0x40_64;
}

//----- (000200F0) --------------------------------------------------------
void ArcherResetAnimation_200F0(type_entity_0x6E8E* entity)//2010f0 //tex 8
{
	entity->actSpeed_0x82_130 = entity->minSpeed_0x84_132;
	SetEntityIndexAndRot_49CD0(entity, 0);
	SetEntityShiftRot_49EA0(entity, 128, 256);
	entity->xtype_0x41_65 = 3;
	entity->xsubtype_0x42_66 = -1;
}

//----- (00020140) --------------------------------------------------------
void ArcherRunScript_20140(type_entity_0x6E8E* entity)//201140
{
	entity->param_0x10_16 = 0;
	CreatureRunScript_1D5D0(entity, 32);
	if (entity->actionIndex_0x45_69 == 34)
		ArcherStartShooting_20060(entity);
}

//----- (00020370) --------------------------------------------------------
void SkeletonArcherIdle_20370(type_entity_0x6E8E* a1x)//201370
{
	int result; // eax
	int v2; // edx

	result = a1x->param_0x10_16;
	v2 = result - 1;
	a1x->param_0x10_16 = result - 1;
	if (result)
	{
		if (v2 < 16)
		{
			result = v2 / 2;
			if (!(v2 % 2))
				/*result = */AdvanceAnimationFrame_585A0(a1x);
		}
	}
	else
	{
		SkeletonArcherReset_20F20(a1x);
		a1x->actionIndex_0x45_69 = 73;
		a1x->param_0x10_16 = 400;
		a1x->levelPar3_0x46_70 = 0;
		/*result = */AdvanceEntityStage_122A0(a1x);
	}
	//return result;
}

//----- (000203D0) --------------------------------------------------------
void SkeletonArcherWander_203D0(type_entity_0x6E8E* a1x)//2013d0
{
	int v1; // edx
	char v2; // al
	unsigned int v3; // esi
	__int16 v4; // ax
	int i; // eax
	int v6; // edx
	type_entity_0x6E8E* v7x; // eax
	unsigned int v8; // edx
	type_entity_0x6E8E* v9x; // edi
	type_entity_0x6E8E* jx; // esi
	int v11; // eax
	unsigned int v12; // eax
	unsigned int v13; // ST18_4
	unsigned __int16 v14; // ax
	int v15; // eax
	type_entity_0x6E8E* v16x; // edi
	type_entity_0x6E8E* v17x; // esi
	int v18; // eax
	int v19; // edx
	int v20; // eax
	signed int v21; // ST18_4
	__int16 v22; // ax
	//uint8_t* v23; // edx
	//type_str_0x6E8E* v23x; // edx
	//int v24; // esi
	unsigned int v25; // edx
	unsigned int v26; // edi
	type_entity_0x6E8E* v27x; // esi
	type_entity_0x6E8E* kx; // eax
	int v29; // edx
	unsigned int v30; // edx
	unsigned int v31; // edi
	type_entity_0x6E8E* lx; // eax
	int v33; // edx
	unsigned int v34; // edx
	unsigned int v35; // edi
	type_entity_0x6E8E* mx; // eax
	int v37; // edx
	unsigned int v38; // edx
	type_entity_0x6E8E* v39x; // eax
	unsigned int v40; // [esp+0h] [ebp-24h]
	type_entity_0x6E8E* v41x; // [esp+4h] [ebp-20h]
	unsigned int v42; // [esp+8h] [ebp-1Ch]
	unsigned int v43; // [esp+10h] [ebp-14h]
	unsigned int v44; // [esp+14h] [ebp-10h]
	unsigned int v45; // [esp+18h] [ebp-Ch]
	unsigned int v46; // [esp+1Ch] [ebp-8h]

	v1 = a1x->param_0x10_16;
	if (v1 > 0)
	{
		a1x->param_0x10_16 = v1 - 1;
		if (v1 == 1)
			SkeletonArcherStartAnimation_20F60(a1x);
	}
	v2 = a1x->levelPar3_0x46_70;
	if (v2)
	{
		if (v2 == 1)
			SkeletonArcherUpdate_20940(a1x);
		goto LABEL_85;
	}
	if (a1x->timer_0x39_57)
		a1x->param_0x10_16 = 400;
	v3 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v4 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v3 = 1;
		a1x->hitById_0x26_38 = v4;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v7x->nextLinkedId_0x34_52)
		{
			v7x = Entities_EA3E4[i];
			if (v7x == Entities_EA3E4[0])
				break;
			v6 = v7x->life_0x8;
			if (v6 < a1x->life_0x8)
			{
				a1x->life_0x8 = v6;
				v3 = 1;
				a1x->hitById_0x26_38 = v7x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v3 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v3 >= 1)
	{
		if (v3 <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			a1x->actionIndex_0x45_69 = 74;
		}
		else if (v3 == 2)
		{
			a1x->actionIndex_0x45_69 = 76;
		}
		goto LABEL_85;
	}
	if (!v3)
	{
		MoveCreature_1B8C0(a1x);
		if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
		{
			v8 = -1;
			v9x = 0;
			for (jx = gameState_D41A4.class3List_38519; jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				if (jx->model_0x40_64 == 2 && jx->id_0x1A_26 != a1x->id_0x1A_26)
				{
					v11 = (signed __int16)(jx->position_0x4C_76.x - a1x->position_0x4C_76.x);
					v12 = v11 * v11
						+ (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
						* (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y);
					if (v12 < v8)
					{
						v9x = jx;
						v8 = v12;
					}
				}
			}
			v41x = v9x;
			if (v9x)
			{
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v9x->position_0x4C_76);
				v13 = v9x->params_0x52_82.pitch + a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Distance3D_583F0(&a1x->position_0x4C_76, &v9x->position_0x4C_76) > v13)
				{
					v41x = 0;
				}
				else
				{
					a1x->targetId_0x96_150 = v9x - world_D41A0.entities_0x6E8E;
					a1x->actionIndex_0x45_69 = 74;
				}
			}
			else
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v14 = a1x->rand_0x14_20;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v14 % 0x9Du) / 79) - 1);
				a1x->roll_0x20_32 &= 0x7ff;
			}
			if (!v41x)
			{
				if (a1x->timer_0x39_57)
				{
					v15 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					v16x = 0;
					v44 = -1;
					v17x = gameState_D41A4.class3List_38519;
					v42 = v15 * v15;
					while (v17x > Entities_EA3E4[0])
					{
						if (v17x->id_0x1A_26 != a1x->id_0x1A_26)
						{
							v18 = (signed __int16)(v17x->position_0x4C_76.x - a1x->position_0x4C_76.x);
							v19 = v18 * v18;
							v20 = (signed __int16)(v17x->position_0x4C_76.y - a1x->position_0x4C_76.y);
							v43 = v20 * v20 + v19;
							if (v43 <= v42 && !(v17x->flags_0xc_12_15.byte[0] & 0x20))
							{
								v21 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
								v22 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v17x->position_0x4C_76);
								if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v22) < v21 && v43 < v44)
								{
									v16x = v17x;
									v44 = v43;
								}
							}
						}
						v17x = v17x->next_0;
					}
					v41x = v16x;
					if (v16x)
					{
						a1x->targetId_0x96_150 = v16x - world_D41A0.entities_0x6E8E;
						a1x->actionIndex_0x45_69 = 74;
					}
				}
				//v23x = v41x;
				if (!v41x)
				{
					/*LOBYTE(v23) = a1x->instanceIndex_0x3E_62;
					//v24 = a1x->dword_0xA0_160;
					v25 = (int)v23 / a1x->typeParams_0xA0_160x->word_160_0x1a_26 % 3;*/
					v25 = (a1x->instanceIndex_0x3E_62 / a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26) % 3;
					if (v25 < 1)
					{
						if (v25)
						{
						LABEL_79:
							if (v41x)
							{
								if (Distance3D_583F0(&a1x->position_0x4C_76, &v41x->position_0x4C_76) <= 0x600)
								{
									predictedAxis_EB398ar = v41x->position_0x4C_76;
									DisableEntityDrawing04_57F10(v41x);
									v39x = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 9);
									if (v39x)
									{
										if (Entities_EA3E4[a1x->id_0x1A_26]->class_0x3F_63 == 3)
											v39x->id_0x1A_26 = a1x->id_0x1A_26;
									}
								}
							}
							goto LABEL_85;
						}
						v45 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
						v26 = -1;
						v27x = 0;
						for (kx = gameState_D41A4.creatureLists_38403x[16 / 4]; kx > Entities_EA3E4[0]; kx = kx->next_0)
						{
							v29 = (signed __int16)(kx->position_0x4C_76.x - a1x->position_0x4C_76.x);
							v30 = v29 * v29
								+ (signed __int16)(kx->position_0x4C_76.y - a1x->position_0x4C_76.y)
								* (signed __int16)(kx->position_0x4C_76.y - a1x->position_0x4C_76.y);
							if (v30 <= v45 && v30 < v26)
							{
								v27x = kx;
								v26 = v30;
							}
						}
					}
					else if (v25 <= 1)
					{
						v46 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
						v31 = -1;
						v27x = 0;
						for (lx = gameState_D41A4.creatureLists_38403x[48 / 4]; lx > Entities_EA3E4[0]; lx = lx->next_0)
						{
							v33 = (signed __int16)(lx->position_0x4C_76.x - a1x->position_0x4C_76.x);
							v34 = v33 * v33
								+ (signed __int16)(lx->position_0x4C_76.y - a1x->position_0x4C_76.y)
								* (signed __int16)(lx->position_0x4C_76.y - a1x->position_0x4C_76.y);
							if (v34 <= v46 && v34 < v31)
							{
								v27x = lx;
								v31 = v34;
							}
						}
					}
					else
					{
						if (v25 != 2)
							goto LABEL_79;
						v40 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
						v35 = -1;
						v27x = 0;
						for (mx = gameState_D41A4.creatureLists_38403x[52 / 4]; mx > Entities_EA3E4[0]; mx = mx->next_0)
						{
							v37 = (signed __int16)(mx->position_0x4C_76.x - a1x->position_0x4C_76.x);
							v38 = v37 * v37
								+ (signed __int16)(mx->position_0x4C_76.y - a1x->position_0x4C_76.y)
								* (signed __int16)(mx->position_0x4C_76.y - a1x->position_0x4C_76.y);
							if (v38 <= v40 && v38 < v35)
							{
								v27x = mx;
								v35 = v38;
							}
						}
					}
					v41x = v27x;
					goto LABEL_79;
				}
			}
		}
	}
LABEL_85:
	if (a1x->actionIndex_0x45_69 == 74)
		SkeletonArcherCheckTarget_20EC0(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00020940) --------------------------------------------------------
void SkeletonArcherUpdate_20940(type_entity_0x6E8E* a1x)//201940
{
	signed int v1; // ecx
	__int16 v2; // ax
	int i; // eax
	int v4; // edx
	type_entity_0x6E8E* v5x; // eax
	__int16 v6; // ax
	int v7; // edx
	//int v8; // esi
	int v9; // edx
	type_entity_0x6E8E* v9x; // edx
	unsigned int v10; // edi
	type_entity_0x6E8E* jx; // ecx
	int v12; // eax
	unsigned int v13; // eax
	unsigned int v14; // edi
	type_entity_0x6E8E* kx; // ecx
	int v16; // eax
	unsigned int v17; // eax
	unsigned int v18; // edi
	type_entity_0x6E8E* lx; // ecx
	int v20; // eax
	unsigned int v21; // eax
	int v22; // eax
	//uint8_t* v23; // eax
	type_entity_0x6E8E* v23x; // eax
	type_entity_0x6E8E* v24x; // [esp+10h] [ebp-4h]

	//fix
	v9x = 0;
	//fix

	v1 = 0;
	v24x = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v1 = 1;
		a1x->hitById_0x26_38 = v2;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				v1 = 1;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v1 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if ((unsigned int)v1 >= 1)
	{
		if ((unsigned int)v1 <= 1)
		{
			v6 = a1x->hitById_0x26_38;
			a1x->actionIndex_0x45_69 = 74;
			a1x->targetId_0x96_150 = v6;
		}
		else if (v1 == 2)
		{
			a1x->actionIndex_0x45_69 = 76;
		}
		return;
	}
	if (v1)
		return;
	v7 = a1x->param_0x10_16;
	if (v7 < 0)
	{
		a1x->param_0x10_16 = v7 + 1;
		if (v7 == -1)
			SkeletonArcherSetCooldown_20F80(a1x);
		return;
	}
	if (a1x->timer_0x39_57)
	{
		a1x->param_0x10_16 = -50;
		return;
	}
	LOBYTE(v1) = a1x->instanceIndex_0x3E_62;
	//v8 = a1x->dword_0xA0_160;
	if (!(v1 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
	{
		v9 = (v1 / a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 % 3);
		if (v9 < 1)
		{
			if (v9)
			{
			LABEL_50:
				if (v24x)
				{
					v22 = (signed __int16)(v24x->position_0x4C_76.x - a1x->position_0x4C_76.x);//0x201BC1 16-bit sub + cwde
					if (Maths::IntegerSqrt_7277A(
						(signed __int16)(v24x->position_0x4C_76.y - a1x->position_0x4C_76.y) * (signed __int16)(v24x->position_0x4C_76.y - a1x->position_0x4C_76.y)
						+ v22 * v22
						+ (signed __int16)(v24x->position_0x4C_76.z - a1x->position_0x4C_76.z) * (signed __int16)(v24x->position_0x4C_76.z - a1x->position_0x4C_76.z)) <= 0x600)
					{
						predictedAxis_EB398ar = v24x->position_0x4C_76;
						DisableEntityDrawing04_57F10(v24x);
						v23x = CreateEntity_4A190(&predictedAxis_EB398ar, 5, 9);
						if (v23x)
							v23x->id_0x1A_26 = a1x->id_0x1A_26;
					}
				}
				return;
			}
			v10 = -1;
			for (jx = gameState_D41A4.creatureLists_38403x[16 / 4]; jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				v12 = (signed __int16)(jx->position_0x4C_76.x - a1x->position_0x4C_76.x);//0x201ABE 16-bit sub + cwde
				v13 = v12 * v12
					+ (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
					* (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y);
				if (v13 <= a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28 && v13 < v10)
				{
					v9x = jx;
					v10 = v13;
				}
			}
		}
		else if (v9 <= 1)
		{
			v14 = -1;
			v9 = 0;
			for (kx = gameState_D41A4.creatureLists_38403x[48 / 4]; kx > Entities_EA3E4[0]; kx = kx->next_0)
			{
				v16 = (signed __int16)(kx->position_0x4C_76.x - a1x->position_0x4C_76.x);//0x201B24 16-bit sub + cwde
				v17 = v16 * v16
					+ (signed __int16)(kx->position_0x4C_76.y - a1x->position_0x4C_76.y)
					* (signed __int16)(kx->position_0x4C_76.y - a1x->position_0x4C_76.y);
				if (v17 <= a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28 && v17 < v14)
				{
					v9x = kx;
					v14 = v17;
				}
			}
		}
		else
		{
			if (v9 != 2)
				goto LABEL_50;
			v18 = -1;
			v9 = 0;
			for (lx = gameState_D41A4.creatureLists_38403x[52 / 4]; lx > Entities_EA3E4[0]; lx = lx->next_0)
			{
				v20 = (signed __int16)(lx->position_0x4C_76.x - a1x->position_0x4C_76.x);//0x201B7C 16-bit sub + cwde
				v21 = v20 * v20
					+ (signed __int16)(lx->position_0x4C_76.y - a1x->position_0x4C_76.y)
					* (signed __int16)(lx->position_0x4C_76.y - a1x->position_0x4C_76.y);
				if (v21 <= a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28 && v21 < v18)
				{
					v9x = lx;
					v18 = v21;
				}
			}
		}
		v24x = v9x;
		goto LABEL_50;
	}
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00020C50) --------------------------------------------------------
void SkeletonArcherAttack_20C50(type_entity_0x6E8E* a1x)//201c50
{
	unsigned int v1; // esi
	__int16 v2; // ax
	int i; // eax
	int v4; // eax
	type_entity_0x6E8E* v5x; // ecx
	type_entity_0x6E8E* v6x; // eax
	type_entity_0x6E8E* v7x; // esi
	//int v8; // edi
	unsigned int v9; // edi

	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v1 = 1;
		a1x->hitById_0x26_38 = v2;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				v1 = 1;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v1 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v1 >= 1)
	{
		if (v1 <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
		}
		else if (v1 == 2)
		{
			a1x->actionIndex_0x45_69 = 76;
		}
		goto LABEL_32;
	}
	if (v1)
		goto LABEL_32;
	MoveCreature_1B8C0(a1x);
	v6x = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
	v7x = v6x;
	if (v6x <= Entities_EA3E4[0] || v6x->life_0x8 < 0 || v6x->flags_0xc_12_15.byte[1] & 4)
	{
	LABEL_31:
		a1x->actionIndex_0x45_69 = 73;
		goto LABEL_32;
	}
	if (!(a1x->instanceIndex_0x3E_62 % 10))
		a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v6x->position_0x4C_76);
	//v8 = a1x->dword_0xA0_160;
	if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
	{
		v9 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
		if (v7x->class_0x3F_63 == 3 && v7x->model_0x40_64 == 2)
			v9 += v7x->params_0x52_82.pitch;
		if (Distance3D_583F0(&a1x->position_0x4C_76, &v7x->position_0x4C_76) < v9)
		{
			CreatureShootArrow2_1CDA0(a1x, v7x);
			goto LABEL_32;
		}
		goto LABEL_31;
	}
LABEL_32:
	if (a1x->actionIndex_0x45_69 != 74)
		SkeletonArcherReset_20F20(a1x);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00020E50) --------------------------------------------------------
void SkeletonArcherFollow_20E50(type_entity_0x6E8E* a1x)//201e50
{
	//char result; // al

	/*result = */CreatureFollowLeader_1C560(a1x, 0x48u);
	if (a1x->actionIndex_0x45_69 == 74)
		/*result = */SkeletonArcherCheckTarget_20EC0(a1x);
	//return result;
}

//----- (00020E80) --------------------------------------------------------
void SkeletonArcherPreKill_20E80(type_entity_0x6E8E* a1x)//201e80
{
	PreKillEntity_1C890(a1x, 72);
}

//----- (00020EA0) --------------------------------------------------------
void SkeletonArcherKill_20EA0(type_entity_0x6E8E* a1x)//201ea0
{
	KillEntity_1C930(a1x);
}

//----- (00020EC0) --------------------------------------------------------
void SkeletonArcherCheckTarget_20EC0(type_entity_0x6E8E* a1x)//201ec0 //tex 113
{
	type_entity_0x6E8E* v1x; // esi
	//char result; // al

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (a1x->id_0x1A_26 == v1x->id_0x1A_26)
	{
		a1x->actionIndex_0x45_69 = 73;
	}
	else
	{
		a1x->actSpeed_0x82_130 = 0;
		SetEntityIndexAndRot_49CD0(a1x, 202);
		a1x->xtype_0x41_65 = v1x->class_0x3F_63;
		//result = v1x->byte_0x40_64;
		a1x->xsubtype_0x42_66 = v1x->model_0x40_64;
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00020F20) --------------------------------------------------------
void SkeletonArcherReset_20F20(type_entity_0x6E8E* a1x)//201f20 //tex 110
{
	//int result; // eax

	a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	/*result = */SetEntityIndexAndRot_49CD0(a1x, 201);
	a1x->xtype_0x41_65 = 3;
	a1x->xsubtype_0x42_66 = -1;
	a1x->param_0x10_16 = 50;
	a1x->levelPar3_0x46_70 = 0;
	//return result;
}

//----- (00020F60) --------------------------------------------------------
void SkeletonArcherStartAnimation_20F60(type_entity_0x6E8E* a1x)//201f60 //tex 110
{
	//int result; // eax

	/*result = */SetEntityIndexAndRot_49CD0(a1x, 201);
	a1x->levelPar3_0x46_70 = 1;
	//return result;
}

//----- (00020F80) --------------------------------------------------------
void SkeletonArcherSetCooldown_20F80(type_entity_0x6E8E* a1x)//201f80 //tex 110
{
	a1x->levelPar3_0x46_70 = 0;
	a1x->param_0x10_16 = 400;
	return SetEntityIndexAndRot_49CD0(a1x, 201);
}

//----- (00020FC0) --------------------------------------------------------
void SkeletonArcherScripted_20FC0(type_entity_0x6E8E* a1x)//201fc0
{
	CreatureRunScript_1D5D0(a1x, 72);
	if (a1x->actionIndex_0x45_69 == 74)
		SkeletonArcherCheckTarget_20EC0(a1x);
}

//----- (00021030) --------------------------------------------------------
void EmuIdle_21030(type_entity_0x6E8E* a1x)//202030
{
	type_entity_0x6E8E* v1x; // esi
	unsigned __int8 v2; // al
	//int v3; // eax
	char v4; // dh
	int v5; // edx
	int v6; // ecx
	int v7; // edi
	int v8; // eax
	//int v9; // edi
	int v10; // edx
	//int v11; // esi
	int v12; // eax
	type_entity_0x6E8E* v13x; // eax
	__int16 v14; // dx
	int v15; // ecx
	int v16; // edi
	char v17; // ah
	type_entity_0x6E8E* ix; // eax
	char v19; // dl
	int v20; // ecx
	type_entity_0x6E8E* v21x; // eax
	__int16 v22; // dx
	char v24; // [esp+0h] [ebp-4h]

	v24 = 0;
	CountWizards_223E0();
	v1x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	if (EmuCheckNearbyEntities_21F60(a1x))
		a1x->subSpellIndex_0x2A_42 |= 1u;
	v2 = a1x->levelPar3_0x46_70;
	if (v2 > 1u && (v2 < 0xCu || v2 > 0xFu) && a1x->life_0x8 >= 10)
		EmuCheckDamage_22190(a1x);
	switch (a1x->levelPar3_0x46_70)
	{
	case 0:
		//v3 = (int)x_D41A0_BYTEARRAY_0;
		world_D41A0.word_0x36548 = 1;
		a1x->levelPar3_0x46_70 = 1;
		a1x->subSpellIndex_0x2A_42 = 8;
		a1x->param_0x10_16 = 15;
		//LOWORD(v3) = *(x_WORD *)(2124 * *(signed __int16 *)(x_D41A0_BYTEARRAY_0 + 12) + x_D41A0_BYTEARRAY_0 + 11240);
		a1x->param2_0x2C_44 = 22;
		a1x->targetId_0x96_150 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240;
		//*(int16_t*)& x_DWORD_D9F50ar[0x800+0x7a] = 60;
		*(int16_t*)&dataBlock_D9F50[0x87a] = 60;
		ScanTilesAround_22490(a1x);
		goto LABEL_10;
	case 1:
	LABEL_10:
		if (EmuUpdateState_21490(a1x))
		{
			v4 = a1x->subSpellIndex_0x2A_42;
			a1x->levelPar3_0x46_70 = 4;
			a1x->subSpellIndex_0x2A_42 = v4 | 0x80;
		}
		goto LABEL_48;
	case 2:
		v5 = 26 * a1x->life_0x8;
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v6 = a1x->maxLife_0x4;
		v7 = a1x->rand_0x14_20 & 7;
		a1x->levelPar3_0x46_70 = 3;
		v8 = v5 / v6 - v7;
		a1x->param_0x10_16 = v8;
		if (v8 < 3)
			a1x->param_0x10_16 = 3;
		if (a1x->param_0x10_16 > 26)
			a1x->param_0x10_16 = 26;
		a1x->levelPar2_0x44_68 = 0;
		a1x->param2_0x2C_44 = 22;
		SetEntitySpriteAndLoad_221F0(a1x, 341);
		goto LABEL_17;
	case 3:
	LABEL_17:
		if (a1x->life_0x8 < 10)
		{
			a1x->levelPar3_0x46_70 = 12;
			goto LABEL_48;
		}
		if (a1x->subSpellIndex_0x2A_42 & 1)
			goto LABEL_26;
		a1x->param_0x10_16--;
		if ((signed int)Maths::EuclideanDistXYZ_58490(&v1x->position_0x4C_76, &a1x->position_0x4C_76) < 0x2000 && a1x->param_0x10_16 <= 0)
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			if ((signed int)(a1x->rand_0x14_20 % 0xCu) < 9)
			{
				a1x->levelPar3_0x46_70 = 4;
				goto LABEL_48;
			}
		LABEL_26:
			a1x->levelPar3_0x46_70 = 6;
		}
	LABEL_48:
		if (v24 && !(a1x->instanceIndex_0x3E_62 & 3))
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 63);
		EmuSnapToTerrain_22270(a1x);
		return;
	case 4:
		a1x->levelPar3_0x46_70 = 5;
		a1x->param_0x10_16 = 6;
		a1x->levelPar2_0x44_68 = 2;
		a1x->param2_0x2C_44 = 113;
		goto LABEL_25;
	case 5:
	LABEL_25:
		//v9 = a1x->dword_0x10_16 - 1;
		a1x->param_0x10_16--;// = v9;
		if (a1x->param_0x10_16 <= 0)
			goto LABEL_26;
		goto LABEL_48;
	case 6:
		a1x->levelPar3_0x46_70 = 7;
		a1x->param_0x10_16 = 16;
		a1x->levelPar2_0x44_68 = 0;
		a1x->param2_0x2C_44 = 113;
		SetEntitySpriteAndLoad_221F0(a1x, 343);
		goto LABEL_28;
	case 7:
	LABEL_28:
		v10 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v10;
		if (v10 <= 0)
			a1x->levelPar3_0x46_70 = 8;
		goto LABEL_48;
	case 8:
		a1x->levelPar3_0x46_70 = 9;
		a1x->param_0x10_16 = 0;
		a1x->levelPar2_0x44_68 = 3;
		a1x->param2_0x2C_44 = 22;
		SetEntitySpriteAndLoad_221F0(a1x, 342);
		EmuUpdateMovement_21850(a1x);
		goto LABEL_31;
	case 9:
	LABEL_31:
		EmuFindTarget_21AB0(a1x);
		//v11 = a1x->dword_0x10_16 - 1;
		a1x->param_0x10_16--;// = v11;
		if (a1x->param_0x10_16 <= 0)
			a1x->levelPar3_0x46_70 = 10;
		goto LABEL_48;
	case 0xA:
		a1x->levelPar3_0x46_70 = 11;
		a1x->param_0x10_16 = 16;
		a1x->param2_0x2C_44 = 22;
		SetEntitySpriteAndLoad_221F0(a1x, 344);
		goto LABEL_34;
	case 0xB:
	LABEL_34:
		v12 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v12;
		if (v12 <= 0)
			a1x->levelPar3_0x46_70 = 2;
		goto LABEL_48;
	case 0xC:
		a1x->levelPar3_0x46_70 = 13;
		a1x->param_0x10_16 = 32;
		v13x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 17);
		if (v13x)
		{
			v14 = v1x->id_0x1A_26;
			v13x->position_0x4C_76.z = 0;
			v13x->maxLife_0x4 = 70;
			v13x->id_0x1A_26 = v14;
			v13x->life_0x8 = v13x->maxLife_0x4;
		}
		goto LABEL_38;
	case 0xD:
	LABEL_38:
		v24 = 1;
		v15 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v15;
		if (v15 <= 0)
		{
			a1x->levelPar3_0x46_70 = 14;
			a1x->param_0x10_16 = 32;
			SetEntitySpriteAndLoad_221F0(a1x, 345);
		}
		goto LABEL_48;
	case 0xE:
		v24 = 1;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
		v16 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v16;
		if (v16 <= 0)
		{
			a1x->levelPar3_0x46_70 = 15;
			a1x->param_0x10_16 = 60;
			v17 = a1x->flags_0xc_12_15.byte[0];
			a1x->life_0x8 = -1;
			a1x->flags_0xc_12_15.byte[0] = v17 | 1;
			KillAllCreatures_1B5F0();
			for (ix = gameState_D41A4.manaSphereList_38523; ix > Entities_EA3E4[0]; ix = ix->next_0)
			{
				v19 = ix->flags_0xc_12_15.byte[1];
				ix->maxLife_0x4 = 140;
				ix->flags_0xc_12_15.byte[1] = v19 | 0x20;
				ix->life_0x8 = ix->maxLife_0x4;
			}
		}
		goto LABEL_48;
	case 0xF:
		KillAllCreatures_1B5F0();
		v20 = a1x->param_0x10_16 - 1;
		v24 = 1;
		a1x->param_0x10_16 = v20;
		if (v20 <= 0)
		{
			v21x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 9);
			if (v21x)
			{
				v22 = v1x->id_0x1A_26;
				v21x->life_0x8 = 32;
				v21x->maxLife_0x4 = 11;
				v21x->id_0x1A_26 = v22;
				world_D41A0.altSpawnMode_0x36E03 = 1;
			}
			world_D41A0.word_0x36548 = 0;
			DisableEntityDrawing04_57F10(a1x);
		}
		goto LABEL_48;
	default:
		goto LABEL_48;
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// DA7CA: using guessed type __int16 x_WORD_DA7CA;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00021490) --------------------------------------------------------
char EmuUpdateState_21490(type_entity_0x6E8E* a1x)//202490
{
	char v1; // cl
	int v2; // edx
	int v3; // esi
	int v4; // esi
	unsigned __int16 v5; // ax
	char v6; // ah
	unsigned int v7; // esi
	char v8; // dh
	char v9; // al
	char v10; // dl
	char v11; // cl
	int v12; // edx
	char v13; // ah
	type_entity_0x6E8E* ix; // esi
	int v15; // eax
	type_entity_0x6E8E* v16x; // eax
	axis_3d v18x; // [esp+0h] [ebp-30h]
	//__int16 v19; // [esp+4h] [ebp-2Ch]
	int v20; // [esp+8h] [ebp-28h]
	int v21; // [esp+Ch] [ebp-24h]
	int v22; // [esp+10h] [ebp-20h]
	int v23; // [esp+14h] [ebp-1Ch]
	int v24; // [esp+18h] [ebp-18h]
	int v25; // [esp+1Ch] [ebp-14h]
	char v26; // [esp+20h] [ebp-10h]
	char v27; // [esp+24h] [ebp-Ch]
	char v28; // [esp+28h] [ebp-8h]
	unsigned __int8 v29; // [esp+2Ch] [ebp-4h]

	v1 = a1x->subSpellIndex_0x2A_42;
	v27 = 1;
	v29 = 0;
	v26 = 0;
	if (v1 & 8)
	{
		v25 = (a1x->position_0x4C_76.x + 128) >> 8;
		v2 = a1x->param_0x10_16;
		v22 = (a1x->position_0x4C_76.y + 128) >> 8;
		if (v2 < 0)
		{
			v28 = 1;
			v4 = AddE7EE0x_10080(0, 7);
			if (v4)
			{
				while (GetNextSearchOffset_10130(v4, &v21, &v20) == 1 && v28)
				{
					LOBYTE(v5) = v21 + v25;
					HIBYTE(v5) = v20 + v22;
					if (mapTerrainType_10B4E0[v5])
						v28 = 0;
				}
				ResetEvent08_10100(v4);
			}
			if (v28)
			{
				v6 = a1x->subSpellIndex_0x2A_42 | 4;
				a1x->param_0x10_16 = 70;
				a1x->subSpellIndex_0x2A_42 = v6 & 0xF7;
			}
			else
			{
				a1x->param_0x10_16 = 15;
			}
		}
		else
		{
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
			v3 = AddE7EE0x_10080(0, 15 - a1x->param_0x10_16);
			if (v3)
			{
				while (GetNextSearchOffset_10130(v3, &v21, &v20) == 1)
					FindFreeTileNear_56F10(v21 + v25, v20 + v22, -1, 0);
				ResetEvent08_10100(v3);
			}
			a1x->param_0x10_16--;
		}
	}
	else if (v1 & 4)
	{
		KillAllCreatures_1B5F0();
		v7 = a1x->param_0x10_16;
		a1x->param_0x10_16 = v7 - 1;
		if (v7 < 0x11)
		{
			if (v7)
			{
				if (v7 == 1)
					v29 = 3;
			}
			else
			{
				a1x->subSpellIndex_0x2A_42 = (a1x->subSpellIndex_0x2A_42 | 0x10) & 0xFB;
				v8 = a1x->flags_0xc_12_15.byte[0];
				a1x->param_0x10_16 = 1;
				a1x->flags_0xc_12_15.byte[0] = v8 & 0xFE;
			}
		}
		else if (v7 <= 0x11)
		{
			v29 = 2;
		}
		else if (v7 >= 0x23)
		{
			if (v7 <= 0x23)
			{
				v29 = 1;
			}
			else if (v7 == 70)
			{
				world_D41A0.countStageVars_0x36E00 = 0;
			}
		}
	}
	else if (v1 & 0x10)
	{
		if (a1x->param_0x10_16 == 1)
		{
			v11 = a1x->subSpellIndex_0x2A_42;
			a1x->param_0x10_16 = 0;
			a1x->subSpellIndex_0x2A_42 = v11 & 0xBF;
		}
		else if (v1 & 0x40)
		{
			if (Maths::EuclideanDistXYZ_58490(&Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]->position_0x4C_76,
				&a1x->position_0x4C_76) >= 0xA00)
			{
				a1x->subSpellIndex_0x2A_42 &= 0xBFu;
			}
			else
			{
				v9 = a1x->subSpellIndex_0x2A_42;
				v10 = a1x->flags_0xc_12_15.byte[2];
				a1x->param_0x10_16 = 30;
				v9 |= 0x20u;
				a1x->subSpellIndex_0x2A_42 = v9;
				a1x->flags_0xc_12_15.byte[2] = v10 & 0x7F;
				a1x->subSpellIndex_0x2A_42 = v9 & 0xEF;
			}
		}
	}
	else if (v1 & 0x20)
	{
		if (a1x->param_0x10_16 >= 600)
			v27 = 0;
		v12 = a1x->param_0x10_16 + 30;
		a1x->param_0x10_16 = v12;
		if (v12 >= 1200)
		{
			v13 = a1x->subSpellIndex_0x2A_42;
			a1x->param_0x10_16 = 1200;
			a1x->subSpellIndex_0x2A_42 = v13 & 0xDF;
		}
		*(int16_t*)&dataBlock_D9F50[0x87a] = a1x->param_0x10_16;
	}
	else
	{
		v26 = 1;
	}
	if (v29)
	{
		for (ix = gameState_D41A4.manaSphereList_38523; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (v29 < 2u)
			{
				if (v29 == 1)
				{
					ix->flags_0xc_12_15.byte[3] |= 1u;
					ix->flags_0xc_12_15.byte[2] &= 0x7Fu;
				}
			}
			else if (v29 <= 2u)
			{
				ix->flags_0xc_12_15.byte[2] |= 0x80u;//20278D or byte ptr [esi+0Eh],80h
				ix->flags_0xc_12_15.byte[3] &= 0xFEu;//202791 and byte ptr [esi+0Fh],0FEh
			}
			else if (v29 == 3)
			{
				DisableEntityDrawing04_57F10(ix);
			}
		}
	}
	if (v27)
	{
		a1x->parentId_0x28_40 += 96;
		a1x->parentId_0x28_40 &= 0x7FF;
		v24 = 0;
		v15 = a1x->parentId_0x28_40;
		while (1)
		{
			v23 = v15;
			if (v24 >= 4)
				break;
			v18x = a1x->position_0x4C_76;
			MoveEntity_57FA0(&v18x, v23, 0, 192);
			v16x = CreateEntity_4A190(&v18x, 10, 14);
			if (v16x)
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v16x->life_0x8 = (a1x->rand_0x14_20 & 7) + 8;
			}
			v15 = ((x_WORD)v23 + 512) & 0x7FF;
			v24++;
		}
	}
	return v26;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// DA7CA: using guessed type __int16 x_WORD_DA7CA;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00021850) --------------------------------------------------------
void EmuUpdateMovement_21850(type_entity_0x6E8E* a1x)//202850
{
	signed int v1; // ecx
	char v2; // ah
	signed int v3; // esi
	signed int v4; // edx
	char v5; // al
	//int v6; // eax
	//int v7; // eax
	//int v8; // eax
	//int v9; // eax
	signed int v10; // edx

	//fix
	//v6 = 0;
	//v7 = 0;
	//v8 = 0;
	//v9 = 0;
	//fix

	v1 = 0;
	v2 = a1x->subSpellIndex_0x2A_42 & 0xFD;
	v3 = 0;
	a1x->subSpellIndex_0x2A_42 = v2;
	if (v2 & 1)
	{
		a1x->subSpellIndex_0x2A_42 = v2 & 0xFE;
		if (a1x->timer_0x39_57)
		{
			v3 = 1;
			a1x->subSpellIndex_0x2A_42 = v2 & 0xFE | 2;
		}
	}
	else
	{
		a1x->subSpellIndex_0x2A_42 = v2 | 2;
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v4 = a1x->rand_0x14_20 % 0x46u;
		v5 = a1x->subSpellIndex_0x2A_42;
		a1x->rand_0x14_20 += gameState_D41A4.setting_30;
		if (v5 < 0)
		{
			v4 = 0;
			a1x->subSpellIndex_0x2A_42 = v5 & 0x7F;
		}
		if (v4 < 40)
		{
			if (v4 >= 3 && v4 <= 6)
			{
				v3 = 1;
				goto LABEL_22;
			}
		}
		else if (v4 > 48)
		{
			if (v4 > 58)
			{
				if (v4 > 68)
				{
					a1x->param_0x10_16 = 8;
					a1x->ownerPlayerId_0x24_36 = 3;
					//v9 = (int)x_D41A0_BYTEARRAY_0;
					a1x->nextStage_0x4A_74 = 682;

					//if (*(x_WORD*)(0x3653D + v9 + 3) < 12)
					if (world_D41A0.dragonCount2_0x36540 < 12)
					{
						a1x->levelPar1_0x43_67 = 4;
						goto LABEL_22;
					}
				}
				else
				{
					a1x->param_0x10_16 = 8;
					a1x->ownerPlayerId_0x24_36 = 3;
					//v8 = (int)x_D41A0_BYTEARRAY_0;
					a1x->nextStage_0x4A_74 = 682;
					//if (*(x_WORD *)(0x36541 + v8 + 1) < 6)
					if (world_D41A0.cymmerianCount_0x36542 < 6)
					{
						a1x->levelPar1_0x43_67 = 5;
						goto LABEL_22;
					}
				}
			}
			else
			{
				a1x->param_0x10_16 = 8;
				a1x->ownerPlayerId_0x24_36 = 3;
				//v7 = (int)x_D41A0_BYTEARRAY_0;
				a1x->nextStage_0x4A_74 = 682;
				//if (*(x_WORD *)(0x3653D + v7 + 1) < 4)
				if (world_D41A0.dragonCount1_0x3653E < 4)
				{
					a1x->levelPar1_0x43_67 = 3;
					goto LABEL_22;
				}
			}
		}
		else
		{
			a1x->param_0x10_16 = 8;
			a1x->ownerPlayerId_0x24_36 = 8;
			//v6 = (int)x_D41A0_BYTEARRAY_0;
			a1x->nextStage_0x4A_74 = 256;
			//if (*(x_WORD *)(0x36543 + v6 + 1) < 28)
			if (world_D41A0.dragonCount3_0x36544 < 28)
			{
				a1x->levelPar1_0x43_67 = 6;
				goto LABEL_22;
			}
		}
		v1 = 1;
	}
LABEL_22:
	if (v1)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v10 = a1x->rand_0x14_20 % 0x1Du;
		a1x->rand_0x14_20 += gameState_D41A4.setting_30;
		if (v10 > 7)
		{
			if (v10 > 17)
			{
				if (v10 > 25)
				{
					if (v10 > 27)
					{
						v3 = 1;
					}
					else
					{
						a1x->levelPar1_0x43_67 = 8;
						a1x->ownerPlayerId_0x24_36 = 1;
						a1x->param_0x10_16 = 5;
					}
				}
				else
				{
					a1x->levelPar1_0x43_67 = 9;
					a1x->ownerPlayerId_0x24_36 = 1;
					a1x->param_0x10_16 = 5;
				}
			}
			else
			{
				a1x->levelPar1_0x43_67 = 2;
				a1x->ownerPlayerId_0x24_36 = 8;
				a1x->param_0x10_16 = 8;
			}
		}
		else
		{
			a1x->levelPar1_0x43_67 = 1;
			a1x->ownerPlayerId_0x24_36 = 10;
			a1x->param_0x10_16 = 10;
		}
	}
	if (v3)
	{
		a1x->levelPar1_0x43_67 = 7;
		a1x->ownerPlayerId_0x24_36 = 24;
		a1x->param_0x10_16 = 32;
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00021AB0) --------------------------------------------------------
void EmuFindTarget_21AB0(type_entity_0x6E8E* a1x)//202ab0
{
	//int v1; // edx
	type_entity_0x6E8E* v2x; // eax
	__int16 v3; // dx
	__int16 v4; // ax
	//x_WORD* v5; // eax
	type_entity_0x6E8E* v5x; // ebx
	type_entity_0x6E8E* v6x; // eax
	type_entity_0x6E8E* v7x; // eax
	type_entity_0x6E8E* v8x; // eax
	__int16 v9; // ax
	type_entity_0x6E8E* v10x; // eax
	//uint8_t* v11; // eax
	type_entity_0x6E8E* v12x; // esi
	type_entity_0x6E8E* v13x; // edx
	char v14; // cl
	//int v15; // eax
	//__int16 v16; // si
	axis_3d* v17; // ST0C_4
	unsigned __int16 v18; // ax
	type_entity_0x6E8E* v19x; // edi
	int v20; // esi
	__int16 v21; // ax
	axis_3d* v22; // esi
	axis_3d* v23; // ST0C_4
	__int16 v24; // ax
	__int16 v25; // ax
	type_entity_0x6E8E* v26x; // edx
	type_entity_0x6E8E* v27x; // ST0C_4
	axis_3d v28x; // [esp+0h] [ebp-20h]
	//__int16 v29; // [esp+4h] [ebp-1Ch]
	int v30; // [esp+8h] [ebp-18h]
	type_entity_0x6E8E* v31x; // [esp+Ch] [ebp-14h]
	int v32; // [esp+10h] [ebp-10h]
	type_entity_0x6E8E* v33x; // [esp+14h] [ebp-Ch]
	int v34; // [esp+18h] [ebp-8h]
	type_entity_0x6E8E* v35x; // [esp+1Ch] [ebp-4h]

	v33x = 0;
	v35x = 0;
	//v1 = 2124 * world_D41A0.LevelIndex_0xc;
	v34 = -1;
	v2x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v31x = v2x;
	if (v2x > Entities_EA3E4[0] && v2x->life_0x8 >= 0 && !(v2x->flags_0xc_12_15.byte[1] & 4))
	{
		v3 = a1x->ownerPlayerId_0x24_36;
		if (v3)
		{
			a1x->ownerPlayerId_0x24_36 = v3 - 1;
			v28x = a1x->position_0x4C_76;
			MoveEntity_57FA0(&v28x, a1x->yaw_0x1C_28, 0, 640);
			v4 = a1x->position_0x4C_76.z;
			HIBYTE(v4) += 3;
			v28x.z = v4;
			switch (a1x->levelPar1_0x43_67)
			{
			case 1:
				v5x = CreateEntity_4A190(&v28x, 9, 0);
				v33x = v5x;
				if (v5x)
				{
					v5x->levelPar1_0x43_67 = 10;
					v5x->levelPar2_0x44_68 = 0;
					v5x->typeParams_0xA0_160x = &str_D7BD6[62];//(x_DWORD)&unk_D7BD6[0x83c];
					v5x->subSpellIndex_0x2A_42 = 800;
					//*((x_BYTE*)v5 + 67) = 10;
					//*((x_BYTE*)v5 + 68) = 0;
					//*((x_DWORD*)v5 + 40) = &str_D7BD6[62];//(x_DWORD)&unk_D7BD6[0x83c];
					//v5[21] = 800;
					v34 = 15;
				}
				break;
			case 2:
				v6x = CreateEntity_4A190(&v28x, 9, 9);
				v33x = v6x;
				if (v6x)
				{
					v6x->levelPar1_0x43_67 = 10;
					v6x->levelPar2_0x44_68 = 23;
					v6x->typeParams_0xA0_160x = &str_D7BD6[62];//(x_DWORD)&unk_D7BD6[0x83c];
					v6x->subSpellIndex_0x2A_42 = 800;
					/* *((x_BYTE*)v6 + 67) = 10;
					*((x_BYTE*)v6 + 68) = 23;
					*((x_DWORD*)v6 + 40) = (x_DWORD)&unk_D7BD6[0x83c];
					v6[21] = 800;*/
					v34 = 23;
				}
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				v32 = (a1x->nextStage_0x4A_74 * a1x->ownerPlayerId_0x24_36 + a1x->yaw_0x1C_28) & 0x7FF;
				MoveEntity_57FA0(&v28x, v32, 0, 1792);
				v9 = a1x->position_0x4C_76.z;
				HIBYTE(v9) += 3;
				v28x.z = v9;
				switch (a1x->levelPar1_0x43_67)
				{
				case 3:
					v10x = AddDragon_4B240(&v28x);
					goto LABEL_19;
				case 4:
					v10x = AddDevil_4C8F0(&v28x);
					goto LABEL_19;
				case 5:
					v10x = AddCymmerian_4CE00(&v28x);
					goto LABEL_19;
				case 6:
					v10x = AddFirefly_4C6B0(&v28x);
				LABEL_19:
					v35x = v10x;
					break;
				default:
					break;
				}
				if (v35x)
				{
					world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2++;
					switch (a1x->levelPar1_0x43_67)
					{
					case 3:
						v30 = 7;
						v34 = 8;
						break;
					case 4:
						v30 = 175;
						v34 = 42;
						break;
					case 5:
						v30 = 207;
						v34 = 37;
						break;
					case 6:
						v30 = 159;
						v34 = 44;
						break;
					default:
						break;
					}
					//v11 = x_D41A0_BYTEARRAY_0;
					v12x = v35x;
					v35x->targetId_0x96_150 = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240;
					v12x->id_0x1A_26 = a1x->id_0x1A_26;
					v13x = v35x;
					v35x->StageVar2_0x49_73 = 17;
					v13x->stage_0x2E_46 = 250;
					v13x->actSpeed_0x82_130 = 320;
					v13x->parentId_0x28_40 = a1x - world_D41A0.entities_0x6E8E;
					//LOWORD(v11) = v32;
					v13x->yaw_0x1C_28 = v32;
					v13x->roll_0x20_32 = v32;
					v13x->actionIndex_0x45_69 = v30;
				}
				break;
			case 7:
				if (a1x->subSpellIndex_0x2A_42 & 2)
				{
					SetLocalPaletteSubMode_5C800(v31x, 6);
					world_D41A0.emuTargetDistance_0x36546 = 1024;
					v14 = a1x->subSpellIndex_0x2A_42;
					v34 = 19;
					a1x->subSpellIndex_0x2A_42 = v14 & 0xFD;
				}
				//v15 = (int)x_D41A0_BYTEARRAY_0;
				world_D41A0.emuTargetDistance_0x36546 -= 80;
				if (world_D41A0.emuTargetDistance_0x36546 < 10)
					world_D41A0.emuTargetDistance_0x36546 = 10;
				if (world_D41A0.emuTargetDistance_0x36546 > 1024)
					world_D41A0.emuTargetDistance_0x36546 = 1024;
				v17 = &v31x->position_0x4C_76;
				predictedAxis_EB398ar = v31x->position_0x4C_76;
				v18 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, v17);
				v32 = v18;
				MoveEntity_57FA0(&predictedAxis_EB398ar, v18, 0, world_D41A0.emuTargetDistance_0x36546);
				v19x = v31x;
				if (MoveTest_5D0A0(v31x))
				{
					v20 = v19x->typeParams_0xA0_160x->minHeight_160_0xc_12;
					v21 = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					if (predictedAxis_EB398ar.z < v21 + v20)
						predictedAxis_EB398ar.z = v19x->typeParams_0xA0_160x->minHeight_160_0xc_12 + v21;
					CopyEntityPosition_57CF0(v31x, &predictedAxis_EB398ar);
				}
				break;
			case 8:
				v8x = CreateEntity_4A190(&v28x, 9, 26);
				v33x = v8x;
				if (v8x)
				{
					v8x->levelPar1_0x43_67 = 10;
					v8x->levelPar2_0x44_68 = 22;
					v8x->subSpellIndex_0x2A_42 = 20;
					v8x->levelPar3_0x46_70 = 3;
					/**((x_BYTE*)v8 + 67) = 10;
					*((x_BYTE*)v8 + 68) = 22;
					v8[21] = 20;
					*((x_BYTE*)v8 + 70) = 3;*/
					v34 = 15;
				}
				break;
			case 9:
				v7x = CreateEntity_4A190(&v28x, 9, 3);
				v33x = v7x;
				if (v7x)
				{
					v7x->levelPar1_0x43_67 = 10;
					v7x->levelPar2_0x44_68 = 17;
					v7x->subSpellIndex_0x2A_42 = 6000;
					v7x->levelPar3_0x46_70 = 10;
					/**((x_BYTE*)v7 + 67) = 10;
					*((x_BYTE*)v7 + 68) = 17;
					v7[21] = 6000;
					*((x_BYTE*)v7 + 70) = 10;*/
					v34 = 15;
				}
				break;
			default:
				break;
			}
			if (v33x)
			{
				v22 = &v31x->position_0x4C_76;
				v23 = &v31x->position_0x4C_76;
				v33x->id_0x1A_26 = a1x->id_0x1A_26;
				v24 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, v23);
				v33x->yaw_0x1C_28 = v24;
				v25 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, v22);
				v26x = v33x;
				v33x->pitch_0x1E_30 = v25;
				v26x->xsubtype_0x42_66 = v31x->model_0x40_64;
				v27x = v31x;
				v26x->xtype_0x41_65 = v31x->class_0x3F_63;
				SetWizardHitTimer_5EF70(v27x);
			}
		}
		if (v34 >= 0)
			PrepareEventSound_6E450(a1x->id_0x1A_26, -1, v34);
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D8412: using guessed type void *off_D8412;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00021F60) --------------------------------------------------------
char EmuCheckNearbyEntities_21F60(type_entity_0x6E8E* a1x)//202f60
{
	type_entity_0x6E8E* ix; // ebx
	unsigned __int8 v2; // al
	bool v3; // eax
	unsigned __int16 v4; // si
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // eax
	__int16 v7; // di
	axis_4d v9x; // [esp+0h] [ebp-28h]
	//uint32_t v10; // [esp+4h] [ebp-24h]
	axis_4d v11x; // [esp+8h] [ebp-20h]
	//__int16 v12; // [esp+Ah] [ebp-1Eh]
	//__int16 v13; // [esp+Ch] [ebp-1Ch]
	axis_3d v14x; // [esp+10h] [ebp-18h]
	//__int16 v15; // [esp+14h] [ebp-14h]
	type_entity_0x6E8E* v16x; // [esp+18h] [ebp-10h]
	bool v17; // [esp+1Ch] [ebp-Ch]
	char v18; // [esp+20h] [ebp-8h]
	char v19; // [esp+24h] [ebp-4h]

	v19 = 0;
	v16x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v17 = a1x->class_0x3F_63 != 5 || a1x->model_0x40_64 != 10;
	for (ix = gameState_D41A4.projectileList_38531; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		v2 = ix->model_0x40_64;
		v18 = 0;
		if (v2 < 0xAu)
		{
			if (v2 < 2u || v2 > 2u && (v2 < 4u || v2 > 5u))
				goto LABEL_31;
			goto LABEL_19;
		}
		if (v2 > 0xAu)
		{
			if (v2 < 0x19u)
			{
				if (v2 < 0x16u || v2 > 0x17u)
					goto LABEL_31;
			}
			else if (v2 > 0x19u && v2 != 30)
			{
				goto LABEL_31;
			}
		LABEL_19:
			if (!v17)
			{
				if (Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &ix->position_0x4C_76) <= 0xC00)
				{
					v19 = 1;
					v18 = 1;
				}
				goto LABEL_31;
			}
			v11x.pitch = 5120;
			v11x.roll = 5120;
			v3 = CompareAxisWithShift_106F0(&a1x->position_0x4C_76, &a1x->params_0x52_82, &ix->position_0x4C_76, &v11x);
			goto LABEL_29;
		}
		v11x.pitch = 5120;
		v11x.roll = 5120;
		v9x.pitch = 5120;
		v9x.roll = 5120;
		v14x = ix->position_0x4C_76;
		v4 = v16x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
		if (v4)
		{
			v5x = Entities_EA3E4[v4];
			v14x = v5x->position_0x4C_76;

			v9x = v5x->params_0x52_82;
		}
		if (v17)
		{
			v3 = CompareAxisWithShift_106F0(&a1x->position_0x4C_76, &a1x->params_0x52_82, &v14x, &v9x);
		LABEL_29:
			if (v3)
				v18 = 1;
			goto LABEL_31;
		}
		if (CompareAxisWithShift_106F0(&a1x->position_0x4C_76, &v11x, &v14x, &v9x))
		{
			v19 = 1;
			v18 = 1;
		}
	LABEL_31:
		if (v18)
		{
			if (ix->model_0x40_64 == 10)
				Entities_EA3E4[v16x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2]]->stage_0x2E_46 = 0;
			v6x = CreateEntity_4A190(&ix->position_0x4C_76, 10, 0);
			if (v6x)
				v6x->id_0x1A_26 = a1x->id_0x1A_26;
			DisableEntityDrawing04_57F10(ix);
		}
	}
	v7 = v16x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[8];
	if (v7 && Entities_EA3E4[v7]->stage_0x2E_46 > 0)
		v19 = 1;
	return v19;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00022190) --------------------------------------------------------
char EmuCheckDamage_22190(type_entity_0x6E8E* a1x)//203190
{
	char v1; // dl
	signed int v2; // edx
	__int16 v3; // dx

	v1 = 0;
	if (a1x->timer_0x39_57)
	{
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			v2 = a1x->mail_0x5E_94.channel[0].amount.dword;
			if (v2 < 1)
				v2 = 1;
			if (v2 > 300)
				v2 = 300;
			a1x->life_0x8 -= v2;
			v3 = a1x->mail_0x5E_94.channel[0].source;
			a1x->mail_0x5E_94.channel[0].source = 0;
			a1x->hitById_0x26_38 = v3;
			v1 = 1;
		}
		else
		{
			a1x->hitById_0x26_38 = 0;
		}
	}
	if (a1x->life_0x8 < 10)
	{
		v1 = 2;
		a1x->life_0x8 = 8;
	}
	return v1;
}

//----- (00022270) --------------------------------------------------------
void EmuSnapToTerrain_22270(type_entity_0x6E8E* a1x)//203270
{
	__int16 result; // ax
	signed int v2; // edx

	result = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v2 = a1x->life_0x8;
	a1x->position_0x4C_76.z = result;
	if (v2 >= 10)
		/*result = */EmuFacePlayer_222B0(a1x);
	//return result;
}

//----- (000222B0) --------------------------------------------------------
void EmuFacePlayer_222B0(type_entity_0x6E8E* a1x)//2032b0
{
	type_entity_0x6E8E* v1x; // eax
	unsigned int v2; // edx
	signed int v3; // esi
	signed __int16 v4; // dx
	__int16 v5; // ax
	//char v6; // ch

	v1x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	v2 = (((a1x->yaw_0x1C_28 - v1x->yaw_0x1C_28) >> 3) & 0xF0) >> 4;
	v3 = 0;
	if (v2 <= 2)
	{
		//LOWORD(v1) = *(x_WORD *)(v1 + 28) + 384;
		a1x->yaw_0x1C_28 = v1x->yaw_0x1C_28 + 384;
	}
	else if (v2 >= 0xD && v2 <= 0xF)
	{
		//LOWORD(v1) = *(x_WORD *)(v1 + 28);
		v5 = v1x->yaw_0x1C_28;
		HIBYTE(v5) += 6;//0x203320 add ah, 6
		a1x->yaw_0x1C_28 = v5;
	}
	else
	{
		switch (a1x->levelPar2_0x44_68)
		{
		case 0:
			v3 = 1;
			//LOWORD(v1) = Maths::GetYawAngle_581E0((uint16_t*)(a1 + 76), (uint16_t*)(v1 + 76));
			//goto LABEL_15;
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v1x->position_0x4C_76);
			break;
		case 1:
			//goto LABEL_13;
			v3 = 1;
			break;
		case 2:
			if (gameState_D41A4.FrameTimingIndex_26 & 1)
				v4 = 512;
			else
				v4 = -512;
			v5 = v1x->yaw_0x1C_28;
			HIBYTE(v5) += 4;
			//LOWORD(v1) = v4 + v5;
			a1x->roll_0x20_32 = v4 + v5;
			//v6 = a1x->byte_0x21_33;
			a1x->levelPar2_0x44_68 = 1;
			a1x->roll_0x20_32 &= 0x7ff;
			//LABEL_13:
			v3 = 1;
			break;
		case 3:
			//LOWORD(v1) = *(x_WORD *)(a1 + 28);
		//LABEL_15:
			a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
			break;
		default:
			break;
		}
		if (v3)
		{
			/*LOWORD(v1) = TurnTowardsAngle_58350(
				*(x_WORD *)(a1 + 28),
				*(x_WORD *)(a1 + 32),
				*(unsigned __int16 *)(*(x_DWORD *)(a1 + 160) + 4),
				*(x_WORD *)(a1 + 44));*/
			a1x->yaw_0x1C_28 += TurnTowardsAngle_58350(
				a1x->yaw_0x1C_28,
				a1x->roll_0x20_32,
				a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
				a1x->param2_0x2C_44);
		}
	}
	a1x->yaw_0x1C_28 &= 0x7FF;
	return;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00022490) --------------------------------------------------------
void ScanTilesAround_22490(type_entity_0x6E8E* a1x)//203490
{
	uaxis_2d v1x; // ax
	signed int v2; // esi
	uaxis_2d v3x; // bx
	signed int v4; // edi
	uaxis_2d v6x; // [esp+8h] [ebp-4h]

	v1x._axis_2d.x = ((unsigned __int16)(a1x->position_0x4C_76.x + 128) >> 8) - 19;
	v1x._axis_2d.y = ((unsigned __int16)(a1x->position_0x4C_76.y + 128) >> 8) - 19;
	v2 = 0;
	v6x.word = v1x.word;
	do
	{
		v3x.word = v6x.word;
		v4 = 0;
		while (v4 < 38)
		{
			v4++;
			/*LOBYTE(v1) = */DamageSceneryOnTile_57390(v3x, a1x->id_0x1A_26);
			v3x._axis_2d.x++;
		}
		v2++;
		v6x._axis_2d.y++;
	} while (v2 < 38);
	//return v1;
}

//----- (00022530) --------------------------------------------------------
type_entity_0x6E8E* EmuWander_22530(type_entity_0x6E8E* a1x)//203530
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	a1x->actionIndex_0x45_69 = 80;
	return resultx;
}

//----- (00022540) --------------------------------------------------------
type_entity_0x6E8E* EmuAttack_22540(type_entity_0x6E8E* a1x)//203540
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	a1x->actionIndex_0x45_69 = 80;
	return resultx;
}

//----- (00022550) --------------------------------------------------------
type_entity_0x6E8E* EmuFollow_22550(type_entity_0x6E8E* a1x)//203550
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	a1x->actionIndex_0x45_69 = 80;
	return resultx;
}

//----- (00022560) --------------------------------------------------------
void EmuPreKill_22560(type_entity_0x6E8E* a1x)//203560
{
	PreKillEntity_1C890(a1x, 80);
}

//----- (00022580) --------------------------------------------------------
void EmuKill_22580(type_entity_0x6E8E* a1x)//203580
{
	KillEntity_1C930(a1x);
}

//----- (000225A0) --------------------------------------------------------
type_entity_0x6E8E* EmuHit_225A0(type_entity_0x6E8E* a1x)//2035a0
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	a1x->actionIndex_0x45_69 = 80;
	return resultx;
}

//----- (000225B0) --------------------------------------------------------
type_entity_0x6E8E* EmuScripted_225B0(type_entity_0x6E8E* a1x)//2035b0
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	a1x->actionIndex_0x45_69 = 80;
	return resultx;
}

//----- (00022640) --------------------------------------------------------
bool CanPlaceBuilding_22640(axis_3d* a1, unsigned __int16 a2, unsigned __int16 a3, unsigned __int16 a4)//203640
{
	unsigned __int8 v4; // bl
	unsigned __int8 v5; // bh

	v4 = (a1->x >> 8) - ((signed int)a2 >> 1);
	v5 = (a1->y >> 8) - ((signed int)a3 >> 1);
	if ((v4 + v5) % 2)
		v4++;
	return (signed __int16)GetAreaHeightRange_48990(v4, v5, a3, a2) < (signed int)a4;
}

//----- (000226D0) --------------------------------------------------------
void GetBuildingBitmapSize_226D0(unsigned __int16 a1, int16_t* exwidth, int16_t* expheight)//2036d0
{
	unsigned __int16 v3; // cx
	unsigned __int16 v4; // bx
	//x_WORD *result; // eax
	/*v3 = *(unsigned __int8 *)(**filearray_2aa18c[24] + 6 * a1 + 5);
	v4 = *(unsigned __int8 *)(**filearray_2aa18c[24] + 6 * a1 + 4);*/
	// BUILD00 table (dword_EA3BC): 0x2036E3
	v3 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1].height_5;
	v4 = (*filearray_2aa18c[filearrayindex_BUILD00DATTAB].posistruct)[a1].width_4;
	if (videoMode_180660 == 1)
	{
		v3 >>= 1;
		v4 >>= 1;
	}
	*expheight = (v3 << 8) / 2 + 768;
	//result = a2;
	*exwidth = (v4 << 8) / 2 + 768;
	//return result;
}
// EA3BC: using guessed type int **filearray_2aa18c[24];
// 180660: using guessed type __int16 videoMode_180660;

//----- (00022760) --------------------------------------------------------
void BuilderIdle_22760(type_entity_0x6E8E* a1x)//203760
{
	type_entity_0x6E8E* v1x; // eax
	signed int v2; // eax
	int v3; // eax
	__int16 v4; // ax
	__int16 v5; // dx
	__int16 v6; // ax
	__int16 v7; // di
	int v8z; // eax
	type_entity_0x6E8E* v8y; // eax
	char v9; // cl
	type_entity_0x6E8E* ix; // esi
	type_entity_0x6E8E* jx; // esi
	type_entity_0x6E8E* kx; // esi
	type_entity_0x6E8E* lx; // esi
	type_entity_0x6E8E* v14x; // esi
	//char v15; // al
	type_entity_0x6E8E* v17x; // [esp+0h] [ebp-1Ch]
	int16_t exheight; // [esp+8h] [ebp-14h]
	int16_t exwidth; // [esp+Ch] [ebp-10h]
	int16_t v20; // [esp+10h] [ebp-Ch]
	int v21; // [esp+14h] [ebp-8h]
	unsigned __int8 v22; // [esp+18h] [ebp-4h]

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v20 = 96;
	v17x = v1x;
	if (!v1x->class_0x3F_63 || v1x->model_0x40_64 != 45)
	{
		a1x->param_0x10_16 = 5;
	LABEL_51:
		//v15 = v20;
		a1x->targetId_0x96_150 = 0;
		//LOBYTE(v8z) = v15 + 1;
		a1x->actionIndex_0x45_69 = v20 + 1;
		return;
	}
	predictedAxis_EB398ar = v1x->position_0x4C_76;
	v2 = a1x->param_0x10_16;
	a1x->param_0x10_16 = v2 + 1;
	if (v2 >= 4)
	{
		a1x->param_0x10_16 = 1;
		goto LABEL_51;
	}
	v22 = ChooseRandomBuilding_232C0();
	GetBuildingBitmapSize_226D0(v22, &exwidth, &exheight);
	v3 = a1x->param_0x10_16 - 1;
	switch (a1x->param_0x10_16)
	{
	case 1:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v4 = 9377 * a1x->rand_0x14_20;
		predictedAxis_EB398ar.x += (a1x->rand_0x14_20 % 3u << 8) + v17x->params_0x52_82.pitch + exwidth + 256;
		a1x->rand_0x14_20 = v4 + 9439;
		v3 = (a1x->rand_0x14_20 % 3u << 8) - 1280;
		v5 = v3 + predictedAxis_EB398ar.y;
		goto LABEL_10;
	case 2:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		predictedAxis_EB398ar.x -= (a1x->rand_0x14_20 % 3u << 8) + v17x->params_0x52_82.pitch + exwidth + 256;
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v3 = (a1x->rand_0x14_20 % 3u << 8) - 1280;
		v5 = v3 + predictedAxis_EB398ar.y;
		goto LABEL_10;
	case 3:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v6 = 9377 * a1x->rand_0x14_20;
		predictedAxis_EB398ar.x += (a1x->rand_0x14_20 % 3u << 8) - 1280;
		a1x->rand_0x14_20 = v6 + 9439;
		v3 = (a1x->rand_0x14_20 % 3u << 8) + v17x->params_0x52_82.roll + exheight + 256;
		v5 = v3 + predictedAxis_EB398ar.y;
		goto LABEL_10;
	case 4:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		predictedAxis_EB398ar.x += (a1x->rand_0x14_20 % 3u << 8) - 1280;
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v3 = v17x->params_0x52_82.roll + exheight;
		v5 = predictedAxis_EB398ar.y - (v3 + (a1x->rand_0x14_20 % 3u << 8) + 256);
	LABEL_10:
		predictedAxis_EB398ar.y = v5;
		break;
	default:
		break;
	}
	LOWORD(v3) = predictedAxis_EB398ar.x;
	v7 = predictedAxis_EB398ar.y;
	v21 = v3;
	if (TerrainTileIsWater_104D0(&predictedAxis_EB398ar) == 1)
	{
		a1x->param_0x10_16 = 2;
		goto LABEL_51;
	}
	LOBYTE(v8z) = CanPlaceBuilding_22640(
		&predictedAxis_EB398ar,
		(signed int)exwidth >> 8,
		(signed int)exheight >> 8,
		(((signed int)exwidth >> 7) + ((signed int)exheight >> 7) > 4) + 15);
	if ((x_BYTE)v8z)
	{
		v9 = 0;
		for (ix = gameState_D41A4.buildingList_38527; !v9 && ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			v8z = abs((signed __int16)(ix->position_0x4C_76.x - v21));
			if (v8z <= ix->params_0x52_82.pitch + exwidth)
			{
				v8z = abs((signed __int16)(ix->position_0x4C_76.y - v7));
				if (v8z <= exheight + ix->params_0x52_82.roll)
					v9 = 1;
			}
		}
		if (!v9)
		{
			for (jx = gameState_D41A4.class3List_38519; !v9 && jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				if (jx->model_0x40_64 == 2)
				{
					v8z = abs((signed __int16)(jx->position_0x4C_76.x - v21));
					if (v8z <= jx->params_0x52_82.pitch + exwidth)
					{
						v8z = abs((signed __int16)(jx->position_0x4C_76.y - v7));
						if (v8z <= exheight + jx->params_0x52_82.roll)
							v9 = 1;
					}
				}
			}
		}
		if (!v9)
		{
			for (kx = gameState_D41A4.miscList_38535; !v9 && kx > Entities_EA3E4[0]; kx = kx->next_0)//0x203B57 mov esi, [esi+9687h]
			{
				if (kx->model_0x40_64 == 67)
				{
					v8z = abs((signed __int16)(kx->position_0x4C_76.x - v21));
					if (v8z <= kx->params_0x52_82.pitch + exwidth)
					{
						v8z = abs((signed __int16)(kx->position_0x4C_76.y - v7));
						if (v8z <= exheight + kx->params_0x52_82.roll)
							v9 = 1;
					}
				}
			}
		}
		if (!v9)
		{
			for (lx = gameState_D41A4.creatureLists_38403x[48 / 4]; !v9 && lx > Entities_EA3E4[0]; lx = lx->next_0)
			{
				if (lx->fontTypeIndex_0x3D_61)
					v9 = 1;
			}
		}
		if (!v9)
		{
			v8y = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 45);
			v14x = v8y;
			if (v8y)
			{
				PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 10);
				/*LOBYTE(v8) =*/ InitBuildingEntity_49A30(v14x, v22);
				v14x->actionIndex_0x45_69 = 51;
			}
			a1x->targetId_0x96_150 = 0;
			a1x->fontTypeIndex_0x3D_61 = 1;
			a1x->actionIndex_0x45_69 = 105;
		}
	}
	//return v8z;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];

//----- (00022C80) --------------------------------------------------------
void BuilderWander_22C80(type_entity_0x6E8E* a1x)//203c80
{
	unsigned int v1; // eax
	__int16 v2; // ax
	int i; // edx
	int v4; // ecx
	type_entity_0x6E8E* v5x; // edx
	type_entity_0x6E8E* v6x; // edx
	unsigned __int16 v7; // ax

	a1x->fontTypeIndex_0x3D_61 = 0;
	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				v1 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1 = 2;
	}
	if (v1 >= 1)
	{
		if (v1 <= 1)
		{
			v6x = Entities_EA3E4[a1x->hitById_0x26_38];
			if (v6x > Entities_EA3E4[0] && (!v6x->model_0x40_64 || v6x->model_0x40_64 == 1))
				Entities_EA3E4[a1x->hitById_0x26_38]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			LOBYTE(v1) = 102;
		}
		else
		{
			if (v1 != 2)
				goto LABEL_28;
			LOBYTE(v1) = 100;
		}
	LABEL_27:
		a1x->actionIndex_0x45_69 = v1;
		goto LABEL_28;
	}
	if (!v1)
	{
		MoveCreature_1B8C0(a1x);
		LOWORD(v1) = (signed __int16)a1x->instanceIndex_0x3E_62 / a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
		if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v7 = a1x->rand_0x14_20;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v7 % 0x9Du) / 79) - 1);
			v1 = a1x->param_0x10_16;
			a1x->roll_0x20_32 &= 0x7ff;
			a1x->param_0x10_16 = v1 - 1;
			if (!v1)
			{
				LOBYTE(v1) = 99;
				a1x->param_0x10_16 = 1;
				goto LABEL_27;
			}
		}
	}
LABEL_28:
	if (a1x->actionIndex_0x45_69 == 102)
	{
		//LOWORD(v1) = a1x->word_0x84_132;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	}
	//return v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00022E60) --------------------------------------------------------
void BuilderAttack_22E60(type_entity_0x6E8E* a1x)//203e60
{
	unsigned int v1; // eax
	__int16 v2; // ax
	int i; // edx
	int v4; // esi
	type_entity_0x6E8E* v5x; // edx
	type_entity_0x6E8E* v6x; // edx
	int v7; // edx
	type_entity_0x6E8E* v8x; // eax
	axis_3d* v9; // esi

	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x203e64, CommandLineParams.DoDebugafterload(),0x2e);
	}

	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				v1 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1 = 2;
	}
	if (v1 >= 1)
	{
		if (v1 <= 1)
		{
			v6x = Entities_EA3E4[a1x->hitById_0x26_38];
			if (v6x > Entities_EA3E4[0] && (!v6x->model_0x40_64 || v6x->model_0x40_64 == 1))
				Entities_EA3E4[a1x->hitById_0x26_38]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			LOBYTE(v1) = 102;
		}
		else
		{
			if (v1 != 2)
				goto LABEL_31;
			LOBYTE(v1) = 100;
		}
	LABEL_30:
		a1x->actionIndex_0x45_69 = v1;
		goto LABEL_31;
	}
	if (!v1)
	{
		MoveCreature_1B8C0(a1x);
		v1 = a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 / 2;
		if (!v1)
		{
			v7 = a1x->param_0x10_16;
			v8x = Entities_EA3E4[a1x->targetId_0x96_150];
			a1x->param_0x10_16 = v7 - 1;
			if (!v7 || !v8x->class_0x3F_63)
			{
				a1x->param_0x10_16 = 5;
				a1x->actionIndex_0x45_69 = 97;
			}
			v9 = &v8x->position_0x4C_76;
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v8x->position_0x4C_76);
			v1 = Distance3D_583F0(&a1x->position_0x4C_76, v9);
			if (v1 < 0xA00)
			{
				LOBYTE(v1) = 96;
				a1x->param_0x10_16 = 0;
				goto LABEL_30;
			}
		}
	}
LABEL_31:
	if (a1x->actionIndex_0x45_69 == 102)
	{
		//LOWORD(v1) = a1x->word_0x84_132;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	}
	//return v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00023020) --------------------------------------------------------
void BuilderFollow_23020(type_entity_0x6E8E* a1x)//204020
{
	unsigned int v1; // eax
	__int16 v2; // ax
	int i; // edx
	int v4; // esi
	type_entity_0x6E8E* v5x; // edx
	type_entity_0x6E8E* v6x; // edx
	type_entity_0x6E8E* v7x; // edi
	type_entity_0x6E8E* jx; // esi
	unsigned int v9; // eax
	unsigned int v11; // [esp+0h] [ebp-8h]

	v1 = 0;
	v11 = -1;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				v1 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1 = 2;
	}
	if (v1 >= 1)
	{
		if (v1 <= 1)
		{
			v6x = Entities_EA3E4[a1x->hitById_0x26_38];
			if (v6x > Entities_EA3E4[0] && (!v6x->model_0x40_64 || v6x->model_0x40_64 == 1))
				Entities_EA3E4[a1x->hitById_0x26_38]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			LOBYTE(v1) = 102;
		}
		else
		{
			if (v1 != 2)
				goto LABEL_34;
			LOBYTE(v1) = 100;
		}
	LABEL_33:
		a1x->actionIndex_0x45_69 = v1;
		goto LABEL_34;
	}
	if (!v1)
	{
		v7x = 0;
		for (jx = gameState_D41A4.buildingList_38527; jx > Entities_EA3E4[0]; jx = jx->next_0)
		{
			v9 = Distance3D_583F0(&a1x->position_0x4C_76, &jx->position_0x4C_76);
			if (v9 && v9 < v11)
			{
				v7x = jx;
				v11 = v9;
			}
		}
		if (v7x)
		{
			a1x->targetId_0x96_150 = v7x - world_D41A0.entities_0x6E8E;
			a1x->param_0x10_16 = 10;
			LOBYTE(v1) = 98;
		}
		else
		{
			a1x->param_0x10_16 = 5;
			LOBYTE(v1) = 97;
		}
		goto LABEL_33;
	}
LABEL_34:
	if (a1x->actionIndex_0x45_69 == 102)
	{
		//LOWORD(v1) = a1x->word_0x84_132;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	}
	//return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000231E0) --------------------------------------------------------
void BuilderPreKill_231E0(type_entity_0x6E8E* a1x)//2041e0
{
	PreKillEntity_1C890(a1x, 96);
}

//----- (00023200) --------------------------------------------------------
void BuilderKill_23200(type_entity_0x6E8E* a1x)//204200
{
	type_entity_0x6E8E* v1x; // edx

	v1x = Entities_EA3E4[a1x->ownerPlayerId_0x24_36];
	if (v1x > Entities_EA3E4[0] && (!v1x->model_0x40_64 || v1x->model_0x40_64 == 1))
		Entities_EA3E4[a1x->ownerPlayerId_0x24_36]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
	KillEntity_1C930(a1x);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00023260) --------------------------------------------------------
void BuilderHit_23260(type_entity_0x6E8E* a1x)//204260
{
	//__int16 result; // ax

	/*LOBYTE(result) = */CreatureFlee_1C980(a1x, 96);
	//HIBYTE(result) = a1x->byte_0x45_69;
	if (a1x->actionIndex_0x45_69 != 102)
	{
		a1x->param_0x10_16 = 5;
		a1x->targetId_0x96_150 = 0;
		//result = a1x->word_0x86_134;
		a1x->actionIndex_0x45_69 = 97;
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
	}
	//return result;
}

//----- (000232A0) --------------------------------------------------------
void BuilderScripted_232A0(type_entity_0x6E8E* a1x)//2042a0
{
	a1x->param_0x10_16 = 5;
	a1x->actionIndex_0x45_69 = 97;
	/*return */BuilderWander_22C80(a1x);
}

//----- (000232C0) --------------------------------------------------------
char ChooseRandomBuilding_232C0()//2042c0
{
	//int v0; // eax
	unsigned __int8 v1; // bl
	unsigned int v2; // eax

	//v0 = (int)x_D41A0_BYTEARRAY_0;
	//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) + 9439;
	world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
	v1 = 0;
	v2 = world_D41A0.rand_0x8 % 0x3Cu + 17;
	while (!(buildingParams_D93C0.at(v2).flags_2 & 2) && v1 < 0x4Du)
	{
		LOBYTE(v2) = v2 + 1;
		v1++;
		if ((unsigned __int8)v2 >= 0x4Cu)
			LOBYTE(v2) = 17;
	}
	if (v1 >= 0x4Du)
		LOBYTE(v2) = 17;
	return v2;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00023320) --------------------------------------------------------
void TownieIdle_23320(type_entity_0x6E8E* a1x)//204320
{
	a1x->actionIndex_0x45_69 = 105;
	return TownieWander_23340(a1x);
}

//----- (00023340) --------------------------------------------------------
void TownieWander_23340(type_entity_0x6E8E* a1x)//204340
{
	int jy;
	type_entity_0x6E8E* jx; // eax
	__int16 v2; // ax
	int i; // edx
	int v4; // edx
	type_entity_0x6E8E* v5x; // esi
	type_entity_0x6E8E* v6x; // edx
	unsigned __int16 v7; // ax
	unsigned int v8; // edi
	type_entity_0x6E8E* v9x; // esi
	int v10; // edx
	unsigned int v11; // edx
	type_entity_0x6E8E* v12x; // esi

	jy = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		jy = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				jy = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		jy = 2;
	}
	if (jy >= 1)
	{
		if (jy <= 1)
		{
			v6x = Entities_EA3E4[a1x->hitById_0x26_38];
			if (v6x > Entities_EA3E4[0] && (!v6x->model_0x40_64 || v6x->model_0x40_64 == 1))
				Entities_EA3E4[a1x->hitById_0x26_38]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			//jy = 110;
			a1x->actionIndex_0x45_69 = 110;
		}
		else if (jy == 2)
		{
			//jy = 108;
			a1x->actionIndex_0x45_69 = 108;
		}
		goto LABEL_43;
	}
	if (jy)
		goto LABEL_43;
	MoveCreature_1B8C0(a1x);
	//LOWORD(j) = (signed __int16)*(unsigned __int8 *)(a1 + 62) / *(x_WORD *)(*(x_DWORD *)(a1 + 160) + 26);
	if (a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26)
		goto LABEL_43;
	if (a1x->targetId_0x96_150)
	{
		v12x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v12x->class_0x3F_63 == 10 && v12x->model_0x40_64 == 45)
		{
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v12x->position_0x4C_76) > 0x800)
			{
				//LOWORD(j) = Maths::GetYawAngle_581E0((uint16_t*)(a1 + 76), (uint16_t*)(v12 + 76));
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v12x->position_0x4C_76);
				goto LABEL_43;
			}
			if ((signed int)v12x->minSpeed_0x84_132 > v12x->param_0x10_16)
			{
				a1x->param_0x10_16 = 1;
				//LOBYTE(j) = 108;
				a1x->actionIndex_0x45_69 = 108;
				v12x->param_0x10_16++;
				goto LABEL_43;
			}
		}
		a1x->targetId_0x96_150 = 0;
		//LOWORD(j) = *(x_WORD *)(a1 + 134);
		//goto LABEL_42;
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
		goto LABEL_43;
	}
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	v7 = a1x->rand_0x14_20;
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	a1x->roll_0x20_32 += (2 * ((signed int)(v7 % 0x9Du) / 79) - 1) * ((a1x->rand_0x14_20 & 0xFF) + 85);
	a1x->roll_0x20_32 &= 0x7ff;
	//LOWORD(j) = (signed __int16)*(unsigned __int8 *)(a1 + 62) / *(x_WORD *)(*(x_DWORD *)(a1 + 160) + 26);
	if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
	{
		v8 = -1;
		v9x = 0;
		for (jx = gameState_D41A4.buildingList_38527; jx > Entities_EA3E4[0]; jx = jx->next_0)
		{
			if (buildingParams_D93C0[jx->levelPar3_0x46_70].flags_2 & 1)
			{
				v10 = (signed __int16)(jx->position_0x4C_76.x - a1x->position_0x4C_76.x);//0x204531 16-bit sub + movsx
				v11 = v10 * v10
					+ (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y)
					* (signed __int16)(jx->position_0x4C_76.y - a1x->position_0x4C_76.y);//0x204546 16-bit sub + movsx
				if (v11 < v8)
				{
					v9x = jx;
					v8 = v11;
				}
			}
		}
		if (v9x)
		{
			a1x->targetId_0x96_150 = v9x - world_D41A0.entities_0x6E8E;
			//LOWORD(j) = *(x_WORD *)(a1 + 134) + 12;
		//LABEL_42:
			a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134 + 12;
			goto LABEL_43;
		}
	}
LABEL_43:
	if (a1x->actionIndex_0x45_69 == 110)
	{
		//LOWORD(j) = *(x_WORD *)(a1 + 132);
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	}
	//return j;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00023640) --------------------------------------------------------
void TownieAttack_23640(type_entity_0x6E8E* a1x)//204640
{
	a1x->actionIndex_0x45_69 = 105;
	TownieWander_23340(a1x);
}

//----- (00023660) --------------------------------------------------------
void TownieFollow_23660(type_entity_0x6E8E* a1x)//204660
{
	a1x->actionIndex_0x45_69 = 105;
	TownieWander_23340(a1x);
}

//----- (00023680) --------------------------------------------------------
void KillTownie_23680(type_entity_0x6E8E* a1x)//204680
{
	type_entity_0x6E8E* v2x; // ebx

	if (a1x->param_0x10_16)
	{
		DisableEntityDrawing04_57F10(a1x);
		return;
	}
	v2x = Entities_EA3E4[a1x->ownerPlayerId_0x24_36];
	if (v2x > Entities_EA3E4[0] && (!v2x->model_0x40_64 || v2x->model_0x40_64 == 1))
		Entities_EA3E4[a1x->ownerPlayerId_0x24_36]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
	PreKillEntity_1C890(a1x, 104);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000236F0) --------------------------------------------------------
void TownieKill_236F0(type_entity_0x6E8E* a1x)//2046f0
{
	KillEntity_1C930(a1x);
}

//----- (00023710) --------------------------------------------------------
void HitTownie_23710(type_entity_0x6E8E* a1x)//204710
{
	//__int16 result; // ax

	/*LOBYTE(result) = */CreatureFlee_1C980(a1x, 104);
	//HIBYTE(result) = a1x->byte_0x45_69;
	if (a1x->actionIndex_0x45_69 != 110)
	{
		//result = a1x->word_0x86_134;
		a1x->targetId_0x96_150 = 0;
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
	}
	//return result;
}

//----- (00023750) --------------------------------------------------------
void TownieRunScript_23750(type_entity_0x6E8E* a1x)//204750
{
	//__int16 result; // ax

	CreatureRunScript_1D5D0(a1x, 104);
	if (a1x->actionIndex_0x45_69 == 110)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	else
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
	//a1x->word_0x82_130 = result;
	//return result;
}

//----- (00023790) --------------------------------------------------------
void TraderIdle_23790(type_entity_0x6E8E* a1x)//204790
{
	a1x->actionIndex_0x45_69 = 113;
	/*return */TraderWander_237B0(a1x);
}

//----- (000237B0) --------------------------------------------------------
void TraderWander_237B0(type_entity_0x6E8E* a1x)//2047b0
{
	int v1y;
	type_entity_0x6E8E* v1x; // eax
	__int16 v2; // ax
	int i; // edx
	int v4; // edx
	type_entity_0x6E8E* v5x; // esi
	type_entity_0x6E8E* v6x; // edx
	unsigned __int16 v7; // di
	unsigned __int16 v8; // ax
	unsigned int v9; // edi
	type_entity_0x6E8E* v10x; // esi
	int v11; // edx
	unsigned int v12; // edx
	type_entity_0x6E8E* v13x; // esi

	//fix
	v1x = 0;
	//fix

	v1y = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1y = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				v1y = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1y = 2;
	}
	if (v1y >= 1)
	{
		if (v1y <= 1)
		{
			v6x = Entities_EA3E4[a1x->hitById_0x26_38];
			if (v6x > Entities_EA3E4[0] && (!v6x->model_0x40_64 || v6x->model_0x40_64 == 1))
				Entities_EA3E4[a1x->hitById_0x26_38]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			//LOBYTE(v1) = 118;
			a1x->actionIndex_0x45_69 = 118;
		}
		else if (v1y == 2)
		{
			//LOBYTE(v1) = 116;
			a1x->actionIndex_0x45_69 = 116;
		}
		goto LABEL_44;
	}
	if (v1x)
		goto LABEL_44;
	MoveCreature_1B8C0(a1x);
	//LOWORD(v1) = (signed __int16)*(unsigned __int8 *)(a1 + 62) / *(x_WORD *)(*(x_DWORD *)(a1 + 160) + 26);
	if (a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26)
		goto LABEL_44;
	v7 = a1x->targetId_0x96_150;
	if (v7)
	{
		v13x = Entities_EA3E4[v7];
		if (v13x->class_0x3F_63 == 10 && v13x->model_0x40_64 == 45)
		{
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v13x->position_0x4C_76) > 0x800)
			{
				//LOWORD(v1) = Maths::GetYawAngle_581E0((uint16_t*)(a1 + 76), (uint16_t*)(v13 + 76));
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v13x->position_0x4C_76);
				goto LABEL_44;
			}
			if (v13x->minSpeed_0x84_132 > v13x->param_0x10_16)
			{
				a1x->param_0x10_16 = 1;
				//LOBYTE(v1) = 116;
				a1x->actionIndex_0x45_69 = 116;
				v13x->param_0x10_16++;
				goto LABEL_44;
			}
		}
		a1x->targetId_0x96_150 = 0;
		//LOWORD(v1) = *(x_WORD *)(a1 + 134);
		//goto LABEL_43;
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
		goto LABEL_44;
	}
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	v8 = a1x->rand_0x14_20;
	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	a1x->roll_0x20_32 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v8 % 0x9Du) / 79) - 1);
	a1x->roll_0x20_32 &= 0x7ff;
	//LOWORD(v1) = (signed __int16)*(unsigned __int8 *)(a1 + 62) / *(x_WORD *)(*(x_DWORD *)(a1 + 160) + 26);
	if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
	{
		v9 = -1;
		v1x = gameState_D41A4.buildingList_38527;
		v10x = 0;
		while (v1x > Entities_EA3E4[0])
		{
			if (buildingParams_D93C0[v1x->levelPar3_0x46_70].flags_2 & 1)
			{
				v11 = (signed __int16)(v1x->position_0x4C_76.x - a1x->position_0x4C_76.x);
				v12 = v11 * v11
					+ (signed __int16)(v1x->position_0x4C_76.y - a1x->position_0x4C_76.y)
					* (signed __int16)(v1x->position_0x4C_76.y - a1x->position_0x4C_76.y);
				if (v12 < v9 && v12 > 0xE100000)
				{
					v10x = v1x;
					v9 = v12;
				}
			}
			v1x = v1x->next_0;
		}
		if (v10x)
		{
			a1x->targetId_0x96_150 = v10x - world_D41A0.entities_0x6E8E;
			//LOWORD(v1) = *(x_WORD *)(a1 + 134) + 12;
		//LABEL_43:
			a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134 + 12;
			goto LABEL_44;
		}
	}
LABEL_44:
	if (a1x->actionIndex_0x45_69 == 118)
	{
		//LOWORD(v1) = *(x_WORD *)(a1 + 132);
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	}
	//return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00023AC0) --------------------------------------------------------
void TraderAttack_23AC0(type_entity_0x6E8E* a1x)//204ac0
{
	a1x->actionIndex_0x45_69 = 113;
	TraderWander_237B0(a1x);
}

//----- (00023AE0) --------------------------------------------------------
void TraderFollow_23AE0(type_entity_0x6E8E* a1x)//204ae0
{
	a1x->actionIndex_0x45_69 = 113;
	TraderWander_237B0(a1x);
}

//----- (00023B00) --------------------------------------------------------
void TraderPreKill_23B00(type_entity_0x6E8E* a1x)//204b00
{
	//int result; // eax

	if (a1x->param_0x10_16)
		DisableEntityDrawing04_57F10(a1x);
	else
		PreKillEntity_1C890(a1x, 112);
	//return result;
}

//----- (00023B30) --------------------------------------------------------
void TraderKill_23B30(type_entity_0x6E8E* a1x)//204b30
{
	type_entity_0x6E8E* v1x; // edx

	v1x = Entities_EA3E4[a1x->ownerPlayerId_0x24_36];
	if (v1x > Entities_EA3E4[0] && (!v1x->model_0x40_64 || v1x->model_0x40_64 == 1))
		Entities_EA3E4[a1x->ownerPlayerId_0x24_36]->playerData_0xA4_164x->attackedTimer_0x248_584 = 200;
	KillEntity_1C930(a1x);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00023B90) --------------------------------------------------------
void TraderHit_23B90(type_entity_0x6E8E* a1x)//204b90
{
	//__int16 result; // ax

	/*LOBYTE(result) = */CreatureFlee_1C980(a1x, 112);
	//HIBYTE(result) = a1x->byte_0x45_69;
	if (a1x->actionIndex_0x45_69 != 118)
	{
		//result = a1x->word_0x86_134;
		a1x->targetId_0x96_150 = 0;
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
	}
	//return result;
}

//----- (00023BD0) --------------------------------------------------------
void TraderScripted_23BD0(type_entity_0x6E8E* a1x)//204bd0
{
	//__int16 result; // ax

	CreatureRunScript_1D5D0(a1x, 112);
	if (a1x->actionIndex_0x45_69 == 118)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	else
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
	//a1x->word_0x82_130 = result;
	//return result;
}

//----- (00023C20) --------------------------------------------------------
void ExplodedHumanIdle_23C20(type_entity_0x6E8E* a1x)//204c20
{
	CreatureIdle_1BD90(a1x, 120);
}

//----- (00023C40) --------------------------------------------------------
void ExplodedHumanWander_23C40(type_entity_0x6E8E* a1x)//204c40
{
	unsigned int v1; // esi
	__int16 v2; // ax
	int i; // eax
	int v4; // edx
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // eax
	__int16 v7; // ax
	//int v8; // esi
	int v9; // eax
	type_entity_0x6E8E* v10x; // esi
	int v11; // eax
	unsigned int v12; // edi
	signed int v13; // ST0C_4
	__int16 v14; // ax
	//int v15; // eax
	unsigned int v16; // [esp+0h] [ebp-10h]
	unsigned int v17; // [esp+8h] [ebp-8h]
	type_entity_0x6E8E* v18x; // [esp+Ch] [ebp-4h]

	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v1 = 1;
		a1x->hitById_0x26_38 = v2;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				v1 = 1;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v1 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v1 < 1)
	{
		if (!v1)
		{
			ExplodedHumanExplode_24190(a1x);
			//v8 = a1x->dword_0xA0_160;
			if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
			{
				if (a1x->timer_0x39_57)
				{
					v9 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					v16 = -1;
					v18x = 0;
					v10x = gameState_D41A4.class3List_38519;
					v17 = v9;
					while (v10x > Entities_EA3E4[0])
					{
						if (v10x->id_0x1A_26 != a1x->id_0x1A_26)
						{
							v11 = (signed __int16)(v10x->position_0x4C_76.x - a1x->position_0x4C_76.x);
							v12 = (signed __int16)(v10x->position_0x4C_76.y - a1x->position_0x4C_76.y)
								* (signed __int16)(v10x->position_0x4C_76.y - a1x->position_0x4C_76.y)
								+ v11 * v11;
							if (v12 <= v17 && !(v10x->flags_0xc_12_15.byte[0] & 0x20))
							{
								v13 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
								v14 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v10x->position_0x4C_76);
								if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v14) < v13 && v12 < v16)
								{
									v18x = v10x;
									v16 = v12;
								}
							}
						}
						v10x = v10x->next_0;
					}
					if (v18x)
					{
						//v15 = v18x - D41A0_BYTESTR_0.struct_0x6E8E;
						a1x->actionIndex_0x45_69 = 122;
						a1x->targetId_0x96_150 = v18x - world_D41A0.entities_0x6E8E;
					}
				}
			}
		}
	}
	else if (v1 <= 1)
	{
		v6x = Entities_EA3E4[a1x->hitById_0x26_38];
		if (v6x->class_0x3F_63 == 3 && v6x->id_0x1A_26 != a1x->id_0x1A_26)
		{
			v7 = a1x->hitById_0x26_38;
			a1x->actionIndex_0x45_69 = 122;
			a1x->targetId_0x96_150 = v7;
		}
		KeepCreatureAboveTerrain_1EEE0(a1x);
	}
	else if (v1 == 2)
	{
		a1x->actionIndex_0x45_69 = 124;
	}
	if (a1x->actionIndex_0x45_69 == 122)
		ExplodedHumanStartAttack_24100(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00023E60) --------------------------------------------------------
void ExplodedHumanAttack_23E60(type_entity_0x6E8E* a1x)//204e60
{
	unsigned int v1; // esi
	//__int16 v2; // ax
	int i; // eax
	int v4; // edx
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // edi
	//int v7; // esi
	unsigned int v8; // esi
	type_entity_0x6E8E* v9x; // eax
	type_entity_0x6E8E* v10x; // esi
	axis_3d* v11; // [esp+4h] [ebp-8h]
	axis_3d* v12; // [esp+8h] [ebp-4h]

	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		a1x->hitById_0x26_38 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v1 = 1;
		//a1x->hitById_0x26_38 = v2;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				v1 = 1;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v1 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v1 <= 1)
	{
		v6x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (!(a1x->instanceIndex_0x3E_62 & 3))
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v6x->position_0x4C_76);
		if (v6x->life_0x8 < 0 || v6x->flags_0xc_12_15.byte[1] & 4)
		{
			a1x->actionIndex_0x45_69 = 121;
			goto LABEL_26;
		}
		//v7 = a1x->dword_0xA0_160;
		if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
		{
			v11 = &v6x->position_0x4C_76;
			v8 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			v12 = &a1x->position_0x4C_76;
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v6x->position_0x4C_76) >= v8)
			{
				a1x->actionIndex_0x45_69 = 121;
				goto LABEL_26;
			}
			v9x = CreateEntity_4A190(v12, 9, 13);
			v10x = v9x;
			if (v9x)
			{
				v9x->id_0x1A_26 = a1x->id_0x1A_26;
				v9x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v12, v11);
				v10x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v12, v11);
				v10x->position_0x4C_76.z += a1x->params_0x52_82.fov / 2;
				v10x->targetId_0x96_150 = a1x->targetId_0x96_150;
				v10x->xsubtype_0x42_66 = a1x->xsubtype_0x42_66;
				v10x->xtype_0x41_65 = a1x->xtype_0x41_65;
				SetWizardHitTimer_5EF70(v6x);
			}
		}
		KeepCreatureAboveTerrain_1EEE0(a1x);
	}
	else if (v1 == 2)
	{
		a1x->actionIndex_0x45_69 = 124;
	}
LABEL_26:
	if (a1x->actionIndex_0x45_69 != 122)
		ExplodedHumanResetAnimation_24150(a1x);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000240A0) --------------------------------------------------------
void ExplodedHumanFollow_240A0(type_entity_0x6E8E* a1x)//2050a0
{
	/*return */CreatureFollowLeader_1C560(a1x, 0x78u);
}

//----- (000240C0) --------------------------------------------------------
void ExplodedHumanPreKill_240C0(type_entity_0x6E8E* a1x)//2050c0
{
	PreKillEntity_1C890(a1x, 120);
}

//----- (000240E0) --------------------------------------------------------
void ExplodedHumanKill_240E0(type_entity_0x6E8E* a1x)//2050e0
{
	KillEntity_1C930(a1x);
}

//----- (00024100) --------------------------------------------------------
void ExplodedHumanStartAttack_24100(type_entity_0x6E8E* a1x)//205100 //tex 121,11
{
	signed int v1; // edx
	//int result; // eax

	a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
	v1 = a1x->rand_0x14_20 % 0x14u;
	a1x->actSpeed_0x82_130 = 0;
	if (v1 <= 10)
		/*result = */SetEntityIndexAndRot_49CD0(a1x, 206);
	else
		/*result = */SetEntityIndexAndRot_49CD0(a1x, 1);
	//return result;
}

//----- (00024150) --------------------------------------------------------
void ExplodedHumanResetAnimation_24150(type_entity_0x6E8E* a1x)//205150 //tex 8
{
	a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	return SetEntityIndexAndRot_49CD0(a1x, 0);
}

//----- (00024190) --------------------------------------------------------
void ExplodedHumanExplode_24190(type_entity_0x6E8E* a1x)//205190
{
	int v1; // esi
	uint32_t resulta; // eax
	uint16_t resultb;
	unsigned __int16 v3; // ST04_2
	unsigned __int16 v4; // dx
	unsigned int v5; // esi
	int v6; // eax
	type_entity_0x6E8E* ix; // ecx
	uint16_t v8y[4]; // [esp+0h] [ebp-18h]
	//int v9; // [esp+4h] [ebp-14h]
	int v10; // [esp+8h] [ebp-10h]
	//int v11; // [esp+Ch] [ebp-Ch]
	int v12; // [esp+10h] [ebp-8h]
	//char v13; // [esp+14h] [ebp-4h]

	v10 = 0;
	//v8 = *(x_DWORD *)"X\x1BX\x1B\n";//581b581b a000581b
	//v9 = *(x_DWORD *)"\n";//a0 00
	v8y[0] = 0x1b58;
	v8y[1] = 0x1b58;
	v8y[2] = 0x000a;
	v8y[3] = 0x1b58;

	v12 = 1;
	if (!(a1x->instanceIndex_0x3E_62 % 8))
	{
		v1 = ~a1x->typeParams_0xA0_160x->dword_160_0x14_20;
		resulta = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
		if (resulta & v1)
		{
			a1x->actionIndex_0x45_69 = 124;
			return;// resulta;
		}
		resultb = a1x->yaw_0x1C_28;
		while (1)
		{
			//v11 = resultb;
			v3 = resultb;
			if ((unsigned __int16)v10 >= 4u)
				break;
			//v3 = v11;
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			MoveEntity_57FA0(&predictedAxis_EB398ar, v3, 0, 256);
			v4 = v10;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v5 = a1x->rand_0x14_20 % v8y[v4];
			v6 = (v5 + 2) * (CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1) == 0);
			if ((unsigned __int16)v6 > (unsigned __int16)v12)
			{
				v12 = v6;
				//a1x->word_0x1C_28 = v11;
				a1x->yaw_0x1C_28 = v3;
			}
			//result = v11;
			resultb = (resultb + 0x200) & 0x7ff;
			//BYTE1(result) = (BYTE1(result) + 2) & 7;
			v10++;
		}
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	if (!(a1x->instanceIndex_0x3E_62 % 16))
	{
		switch (((a1x->yaw_0x1C_28 - 256) >> 9) & 3)
		{
		case 0:
		case 2:
			predictedAxis_EB398ar.y = (predictedAxis_EB398ar.y >> 8 << 8) + 128;
			break;
		case 1:
		case 3:
			predictedAxis_EB398ar.x = (predictedAxis_EB398ar.x >> 8 << 8) + 128;
			break;
		default:
			break;
		}
	}
	//v13 = 0;
	for (ix = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->id_0x1A_26 != a1x->id_0x1A_26
			&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)ix->position_0x4C_76.x) < 256
			&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)ix->position_0x4C_76.y) < 256)
		{
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&ix->position_0x4C_76, &a1x->position_0x4C_76);
			//v13 = 1;
			break;
		}
	}
	if (a1x->roll_0x20_32 == a1x->yaw_0x1C_28
		|| (a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439,
			(signed int)(a1x->rand_0x14_20 % 0x14u) <= 10))
	{
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	}
	KeepCreatureAboveTerrain_1EEE0(a1x);
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];

//----- (00024400) --------------------------------------------------------
void ExplodedHumanScripted_24400(type_entity_0x6E8E* a1x)//205400
{
	CreatureRunScript_1D5D0(a1x, 120);
}

//----- (00024420) --------------------------------------------------------
void WyvernIdle_24420(type_entity_0x6E8E* a1x)//205420
{
	CreatureIdle_1BD90(a1x, 128);
}

//----- (00024440) --------------------------------------------------------
void WyvernWander_24440(type_entity_0x6E8E* a1x)//205440
{
	//int v1; // ecx
	type_entity_0x6E8E* v2x; // edx
	unsigned int v3; // esi
	type_entity_0x6E8E* ix; // eax
	int v5; // ecx
	unsigned int v6; // ecx
	int v7; // eax
	unsigned int v8; // [esp+4h] [ebp-4h]

	//fix
	v2x = 0;
	//fix

	CreatureWander_1BF90(a1x, 128);
	if (a1x->actionIndex_0x45_69 == 129)
	{
		//v1 = a1x->dword_0xA0_160;
		//v2 = *(unsigned __int8 *)(a1 + 62) % (*(signed __int16 *)(v1 + 26) + 1);
		if (!(a1x->instanceIndex_0x3E_62 % (a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 + 1)))
		{
			v8 = a1x->typeParams_0xA0_160x->range_160_0x1c_28 * a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			v3 = -1;
			for (ix = gameState_D41A4.buildingList_38527; ix > Entities_EA3E4[0]; ix = ix->next_0)
			{
				v5 = (signed __int16)(ix->position_0x4C_76.x - a1x->position_0x4C_76.x);
				v6 = v5 * v5
					+ (signed __int16)(ix->position_0x4C_76.y - a1x->position_0x4C_76.y)
					* (signed __int16)(ix->position_0x4C_76.y - a1x->position_0x4C_76.y);
				if (v6 <= v8 && v6 < v3)
				{
					v2x = ix;
					v3 = v6;
				}
			}
			if (v2x)
			{
				v7 = v2x - world_D41A0.entities_0x6E8E;
				a1x->actionIndex_0x45_69 = 130;
				a1x->targetId_0x96_150 = v7;
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00024510) --------------------------------------------------------
void WyvernAttack_24510(type_entity_0x6E8E* a1x)//205510
{
	int v1; // eax
	type_entity_0x6E8E* iz; // eax
	int iy; // eax
	unsigned int v3; // esi
	type_entity_0x6E8E* v4x; // edi
	type_entity_0x6E8E* v5x; // edi
	int v6; // eax
	type_entity_0x6E8E* v7x; // eax
	type_entity_0x6E8E* v8x; // esi
	__int16 v9; // ax
	int v10; // eax
	__int16 v11; // ax
	unsigned int v13; // [esp+4h] [ebp-8h]
	axis_3d* v14; // [esp+8h] [ebp-4h]

	v1 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
	iy = v1 * v1;
	v3 = 0;
	v13 = iy;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		LOWORD(iy) = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v3 = 1;
		a1x->hitById_0x26_38 = iy;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (iy = a1x->nextLinkedId_0x34_52; ; iy = v4x->nextLinkedId_0x34_52)
		{
			v4x = Entities_EA3E4[iy];
			if (v4x == Entities_EA3E4[0])
				break;
			iy = v4x->life_0x8;
			if ((signed int)iy < a1x->life_0x8)
			{
				a1x->life_0x8 = (x_DWORD)iy;
				LOWORD(iy) = v4x->hitById_0x26_38;
				v3 = 1;
				a1x->hitById_0x26_38 = iy;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		//LOWORD(i) = *(x_WORD *)(a1 + 38);
		v3 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v3 < 1)
	{
		if (!v3)
		{
			MoveCreature_1B8C0(a1x);
			iz = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
			v5x = iz;
			if (iz > Entities_EA3E4[0])
			{
				if (!(a1x->instanceIndex_0x3E_62 & 7))
				{
					if (iz->class_0x3F_63 == 3 || (iy = Distance3D_583F0(&a1x->position_0x4C_76, &iz->position_0x4C_76), iy >= 0x200))//0x205616 cmp byte ptr [eax+3Fh], 3 (eax = target)
					{
						//LOWORD(i) = Maths::GetYawAngle_581E0((uint16_t*)(a1 + 76), (uint16_t*)(v5 + 76));
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v5x->position_0x4C_76);
					}
				}
				if (v5x->life_0x8 < 0 || v5x->flags_0xc_12_15.byte[1] & 4)
				{
					a1x->actionIndex_0x45_69 = 129;
				}
				else
				{
					v6 = a1x->param_0x10_16;
					if (v6)
					{
						a1x->param_0x10_16 = v6 - 1;
						v14 = &a1x->position_0x4C_76;
						v7x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 0);
						v8x = v7x;
						if (v7x)
						{
							v7x->levelPar1_0x43_67 = 10;
							v7x->levelPar2_0x44_68 = 0;
							v7x->typeParams_0xA0_160x = &str_D7BD6[61];// (type_str_160*)&unk_D7BD6[0x81a];
							v7x->xsubtype_0x42_66 = v5x->model_0x40_64;
							v7x->xtype_0x41_65 = v5x->class_0x3F_63;
							v7x->id_0x1A_26 = a1x->id_0x1A_26;
							v7x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(v14, &v5x->position_0x4C_76);
							v8x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(v14, &v5x->position_0x4C_76);
							v8x->position_0x4C_76.z += 6 * a1x->params_0x52_82.fov;
							v9 = a1x->targetId_0x96_150;
							v8x->subSpellIndex_0x2A_42 = 1600;
							v8x->mana_0x90_144 = 50000;
							v8x->targetId_0x96_150 = v9;
						}
					}
					//LOWORD(iy) = (signed __int16)a1x->instanceIndex_0x3E_62 / *(x_WORD *)(a1x->dword_0xA0_160 + 26);
					if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
					{
						v10 = (signed __int16)(v5x->position_0x4C_76.x - a1x->position_0x4C_76.x);
						iy = v10 * v10
							+ (signed __int16)(v5x->position_0x4C_76.y - a1x->position_0x4C_76.y)
							* (signed __int16)(v5x->position_0x4C_76.y - a1x->position_0x4C_76.y);
						if (iy < v13)
						{
							if (!(a1x->instanceIndex_0x3E_62 % (2 * a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26)))
								PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 39);
							v11 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v5x->position_0x4C_76);
							LOWORD(iy) = AngleDifference_582B0(a1x->yaw_0x1C_28, v11);
							if ((unsigned __int16)iy < 0xE3u)
							{
								a1x->param_0x10_16 = 15;
								/*LOWORD(i) = */SetWizardHitTimer_5EF70(v5x);
							}
						}
						else
						{
							a1x->actionIndex_0x45_69 = 129;
						}
					}
				}
			}
		}
	}
	else if (v3 <= 1)
	{
		LOWORD(iy) = a1x->hitById_0x26_38;
		a1x->targetId_0x96_150 = iy;
	}
	else if (v3 == 2)
	{
		a1x->actionIndex_0x45_69 = 132;
	}
	//return iy;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000247D0) --------------------------------------------------------
void WyvernFollow_247D0(type_entity_0x6E8E* a1x)//2057d0
{
	/*return */CreatureFollowLeader_1C560(a1x, 0x80u);
}

//----- (000247F0) --------------------------------------------------------
void WyvernPreKill_247F0(type_entity_0x6E8E* a1x)//2057f0
{
	PreKillEntity_1C890(a1x, 128);
}

//----- (00024810) --------------------------------------------------------
void WyvernKill_24810(type_entity_0x6E8E* a1x)//205810
{
	KillEntity_1C930(a1x);
}

//----- (00024840) --------------------------------------------------------
void WyvernScripted_24840(type_entity_0x6E8E* a1x)//205840
{
	CreatureRunScript_1D5D0(a1x, 128);
}

//----- (00024860) --------------------------------------------------------
void ManticoreIdle_24860(type_entity_0x6E8E* a1x)//205860
{
	type_entity_0x6E8E* resultx; // eax
	char v2; // dh

	/*result = */CreatureIdle_1BD90(a1x, 136);
	//BYTE1(result) = *(x_BYTE *)(a1 + 69);
	if (a1x->actionIndex_0x45_69 == 138)
	{
		resultx = Entities_EA3E4[a1x->targetId_0x96_150];
		if (resultx <= Entities_EA3E4[0] || resultx->class_0x3F_63 != 3 || (v2 = resultx->model_0x40_64) != 0 && v2 != 1)
			a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000248C0) --------------------------------------------------------
void ManticoreWander_248C0(type_entity_0x6E8E* a1x)//2058c0
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dh

	CreatureWander_1BF90(a1x, 136);
	if (a1x->actionIndex_0x45_69 == 138)
	{
		v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v1x <= Entities_EA3E4[0] || v1x->class_0x3F_63 != 3 || (v2 = v1x->model_0x40_64) != 0 && v2 != 1)
			a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00024930) --------------------------------------------------------
void ManticoreAttack_24930(type_entity_0x6E8E* a1x)//205930
{
	unsigned int v1; // eax
	__int16 v2; // ax
	type_entity_0x6E8E* ix; // ecx
	int v4; // edx
	type_entity_0x6E8E* v5x; // eax
	char v6; // al
	type_entity_0x6E8E* jx; // ecx
	signed int v8; // esi
	signed int v9; // eax
	__int16 v10; // ax
	int v11; // esi
	signed int v13; // [esp+0h] [ebp-10h]
	signed int v14; // [esp+4h] [ebp-Ch]
	type_entity_0x6E8E* v15x; // [esp+8h] [ebp-8h]

	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 58);
	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (ix = Entities_EA3E4[a1x->nextLinkedId_0x34_52];
			ix != Entities_EA3E4[0];
			ix = Entities_EA3E4[ix->nextLinkedId_0x34_52])
		{
			v4 = ix->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = ix->hitById_0x26_38;
				v1 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1 = 2;
	}
	if (v1 < 1)
	{
		if (v1)
			return;// v1;
	}
	else
	{
		if (v1 > 1)
		{
			if (v1 == 2)
				a1x->actionIndex_0x45_69 = 140;
			return;// v1;
		}
		a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
	}
	v13 = MoveCreature_1B8C0(a1x);
	v5x = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
	v15x = v5x;
	if (v5x <= Entities_EA3E4[0] || v5x->life_0x8 < 0 || v5x->flags_0xc_12_15.byte[1] & 4)
	{
		a1x->typeParams_0xA0_160x = &str_D7BD6[85]; //(type_str_160*)&unk_D7BD6[0xb4a];
		a1x->targetId_0x96_150 = 0;
		LOWORD(v1) = a1x->minSpeed_0x84_132;
		a1x->actionIndex_0x45_69 = 137;
		a1x->actSpeed_0x82_130 = v1;
	}
	else
	{
		if (!(a1x->instanceIndex_0x3E_62 & 3))
		{
			v6 = a1x->levelPar3_0x46_70;
			if (!v6 || v6 == 4)
			{
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v15x->position_0x4C_76);
				for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
				{
					if (jx->id_0x1A_26 != a1x->id_0x1A_26
						&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch
						&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
					{
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
						break;
					}
				}
			}
		}
		LOWORD(v1) = a1x->levelPar3_0x46_70;
		switch ((x_BYTE)v1)
		{
		case 0:
			LOWORD(v1) = a1x->instanceIndex_0x3E_62 / a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
			if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
			{
				v1 = Distance3D_583F0(&a1x->position_0x4C_76, &v15x->position_0x4C_76);
				if (v1 >= a1x->typeParams_0xA0_160x->range_160_0x1c_28)
				{
					a1x->actionIndex_0x45_69 = 137;
				}
				else if (v1 >= 0x700)
				{
					LOWORD(v1) = CreatureShootProjectile20_1D0E0(a1x, v15x);
				}
				else
				{
					a1x->levelPar3_0x46_70 = 1;
				}
			}
			return;// v1;
		case 1:
			LOWORD(v1) = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v15x->position_0x4C_76);
			a1x->roll_0x20_32 = v1;
			a1x->yaw_0x1C_28 = v1;
			LOWORD(v1) = 3 * a1x->minSpeed_0x84_132;
			a1x->typeParams_0xA0_160x = &str_D7BD6[87]; //(type_str_160*)&unk_D7BD6[0xb8e];
			a1x->param_0x10_16 = 0;
			a1x->levelPar3_0x46_70 = 2;
			a1x->actSpeed_0x82_130 = v1;
			return;// v1;
		case 2:
		case 3:
			if (v13 != 3)
				a1x->yaw_0x1C_28 = a1x->roll_0x20_32;
			v8 = a1x->param_0x10_16;
			if (v8 <= 4)
			{
				v9 = 192 >> a1x->param_0x10_16;
			}
			else
			{
				if (4 - (v8 - 4) <= 0)
				{
					v14 = -192;
					goto LABEL_49;
				}
				v9 = -(192 >> (4 - (v8 - 4)));
			}
			v14 = v9;
		LABEL_49:
			a1x->param_0x10_16++;
			v10 = a1x->actSpeed_0x82_130;
			if (v10 > a1x->maxSpeed_0x86_134)
				a1x->actSpeed_0x82_130 = v10 - 8;
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			v11 = v14 + predictedAxis_EB398ar.z;
			LOWORD(v1) = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
			if (v11 <= (signed __int16)v1)
			{
				a1x->levelPar3_0x46_70 = 4;
				a1x->param_0x10_16 = 18;
			}
			else
			{
				LOBYTE(v1) = v14;
				BYTE1(v1) = a1x->levelPar3_0x46_70;
				a1x->position_0x4C_76.z += v14;
				if (BYTE1(v1) != 3)
				{
					LOWORD(v1) = CreatureMeleeAttack768_1CED0(a1x, v15x);
					if ((x_WORD)v1)
						a1x->levelPar3_0x46_70 = 3;
				}
			}
			break;
		case 4:
			v1 = a1x->param_0x10_16;
			a1x->param_0x10_16 = v1 - 1;
			if (v1)
			{
				if (v1 == 18)
				{
					//LOWORD(v1) = a1x->word_0x86_134;
					a1x->typeParams_0xA0_160x = &str_D7BD6[85]; //(type_str_160*)&unk_D7BD6[0xb4a];
					a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
				}
			}
			else
			{
				a1x->levelPar3_0x46_70 = 0;
				//LOWORD(v1) = a1x->word_0x84_132;
				a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
			}
			return;// v1;
		default:
			return;// v1;
		}
	}
	//return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00024D40) --------------------------------------------------------
void ManticoreFollow_24D40(type_entity_0x6E8E* a1x)//205d40
{
	//type_str_0x6E8E* v1x; // eax
	//char v2; // dh

	/*LOBYTE(v1x) = */CreatureFollowLeader_1C560(a1x, 0x88u);
	if (a1x->actionIndex_0x45_69 == 138)
	{
		//v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (Entities_EA3E4[a1x->targetId_0x96_150] <= Entities_EA3E4[0] || Entities_EA3E4[a1x->targetId_0x96_150]->class_0x3F_63 != 3 /*|| (v2 = v1x->byte_0x40_64) != 0*/ && Entities_EA3E4[a1x->targetId_0x96_150]->model_0x40_64 != 1)
			a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
	//return v1x;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00024DA0) --------------------------------------------------------
void ManticorePreKill_24DA0(type_entity_0x6E8E* a1x)//205da0
{
	PreKillEntity_1C890(a1x, 136);
}

//----- (00024DC0) --------------------------------------------------------
void ManticoreKill_24DC0(type_entity_0x6E8E* a1x)//205dc0
{
	KillEntity_1C930(a1x);
}

//----- (00024DF0) --------------------------------------------------------
void ManticoreScripted_24DF0(type_entity_0x6E8E* a1x)//205df0
{
	CreatureRunScript_1D5D0(a1x, 136);
	if (a1x->actionIndex_0x45_69 == 138)
		a1x->levelPar3_0x46_70 = 0;
}

//----- (00024E20) --------------------------------------------------------
void SentinelIdle_24E20(type_entity_0x6E8E* a1x)//205e20
{
	int result; // eax
	int v2; // edx
	int v3; // eax
	type_entity_0x6E8E* v4x; // edi
	type_entity_0x6E8E* v5x; // esi
	int v6; // eax
	int v7; // ST10_4
	__int16 v8; // ax
	int v9; // eax
	type_entity_0x6E8E* v10x; // esi
	int v11; // eax
	unsigned int v12; // [esp+0h] [ebp-10h]
	unsigned int v13; // [esp+8h] [ebp-8h]
	unsigned int v14; // [esp+Ch] [ebp-4h]

	result = SentinelCheckDamage_252E0(a1x);
	if (result)
	{
		if (result == 1)
			/*result = */SentinelSetState_253B0(/*1, */a1x, 2u, 0);
	}
	else
	{
		result = a1x->levelPar3_0x46_70;
		if ((x_BYTE)result)
		{
			if ((x_BYTE)result == 1 && a1x->targetId_0x96_150)
			{
				v10x = Entities_EA3E4[a1x->targetId_0x96_150];
				v11 = Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v10x->position_0x4C_76);
				if (v11 < a1x->typeParams_0xA0_160x->range_160_0x1c_28)
				{
					TurnTowardsEntity_254E0(a1x, v10x, 4u);
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					result = a1x->rand_0x14_20 / 0x31u;
					if (!(a1x->rand_0x14_20 % 0x31u))
						/*result = */SentinelSetState_253B0(/*result, */a1x, 2u, 0);
				}
				else
				{
					a1x->targetId_0x96_150 = 0;
					/*result = */SentinelSetState_253B0(/*v11, */a1x, 0, 0);
				}
			}
		}
		else
		{
			v2 = a1x->param_0x10_16 - 1;
			a1x->param_0x10_16 = v2;
			if (v2)
			{
				if (a1x->timer_0x39_57)
				{
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					result = a1x->rand_0x14_20 & 1;
					if (!result)
					{
						v3 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
						result = v3 * v3;
						v13 = -1;
						v4x = 0;
						v5x = gameState_D41A4.class3List_38519;
						v12 = result;
						while (v5x > Entities_EA3E4[0])
						{
							v6 = (signed __int16)(v5x->position_0x4C_76.x - a1x->position_0x4C_76.x);
							result = (signed __int16)(v5x->position_0x4C_76.y - a1x->position_0x4C_76.y)
								* (signed __int16)(v5x->position_0x4C_76.y - a1x->position_0x4C_76.y)
								+ v6 * v6;
							v14 = result;
							if (result <= v12 && !(v5x->flags_0xc_12_15.byte[0] & 0x20))
							{
								v7 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
								v8 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v5x->position_0x4C_76);
								result = (unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v8);
								if (result < v7)
								{
									result = v14;
									if (v14 < v13)
									{
										v4x = v5x;
										v13 = v14;
									}
								}
							}
							v5x = v5x->next_0;
						}
						if (v4x && v4x->class_0x3F_63 == 3)
						{
							v9 = v4x - world_D41A0.entities_0x6E8E;
							a1x->targetId_0x96_150 = v9;
							/*result = */SentinelSetState_253B0(/*v9, */a1x, 0, 1u);
						}
					}
				}
			}
			else
			{
				/*result = */SentinelSetState_253B0(/*result, */a1x, 1u, 0);
			}
		}
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00025050) --------------------------------------------------------
void SentinelWander_25050(type_entity_0x6E8E* a1x)//206050
{
	int result; // eax
	int v2; // edx

	result = SentinelCheckDamage_252E0(a1x);
	if (result)
	{
		if (result == 1)
			/*result = */SentinelSetState_253B0(/*1, */a1x, 0, 1u);
	}
	else
	{
		a1x->targetId_0x96_150 = 0;
		result = MoveCreature_1B8C0(a1x);
		v2 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v2;
		if (v2 <= 0)
			/*result = */SentinelSetState_253B0(/*result, */a1x, 0, 0);
	}
	//return result;
}

//----- (000250B0) --------------------------------------------------------
void SentinelAttack_250B0(type_entity_0x6E8E* a1x)//2060b0
{
	int v1; // eax
	int v2; // esi
	int v3; // edx
	//int v4; // esi
	//int v5y; // eax
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // esi
	//int v7; // ecx

	v1 = SentinelCheckDamage_252E0(a1x);
	v2 = v1;
	if ((unsigned int)v1 <= 1)
	{
		v1 = a1x->levelPar3_0x46_70;
		switch ((x_BYTE)v1)
		{
		case 0:
			/*v1 = */TurnTowardsEntity_254E0(a1x, Entities_EA3E4[a1x->targetId_0x96_150], 4u);
			if (v2 == 1)
			{
				v3 = a1x->param_0x10_16 - 47;
				a1x->param_0x10_16 = v3;
				if (v3 < 0)
					/*LOWORD(v1) = */SentinelSetState_253B0(/*v1, */a1x, 2u, 1u);
			}
			else
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v1 = a1x->rand_0x14_20 / 0x29u;
				if (a1x->rand_0x14_20 % 0x29u)
				{
					//v4 = a1x->dword_0x10_16 - 1;
					a1x->param_0x10_16--;// = v4;
					if (a1x->param_0x10_16 < 0)
						/*LOWORD(v1) = */SentinelSetState_253B0(/*v1, */a1x, 2u, 2u);
				}
				else
				{
					/*LOWORD(v1) = */SentinelSetState_253B0(/*v1, */a1x, 2u, 1u);
				}
			}
			return;// v1;
		case 1:
			//v5y = (a1x->dword_0x10_16 - 1);
			a1x->param_0x10_16--;// = (uint32_t)v5y;
			if (a1x->param_0x10_16 <= 0)
				goto LABEL_17;
			//LOWORD(v1) = a1x->instanceIndex_0x3E_62 / *(x_WORD *)(a1x->dword_0xA0_160 + 26);
			if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
			{
				v5x = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
				v6x = v5x;
				if (v5x <= Entities_EA3E4[0] || v5x->life_0x8 < 0 || v5x->flags_0xc_12_15.byte[1] & 4)
				{
				LABEL_17:
					/*LOWORD(v1) = */SentinelSetState_253B0(/*v5x, */a1x, 2u, 2u);
				}
				else
				{
					a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v5x->position_0x4C_76);
					a1x->yaw_0x1C_28 += TurnTowardsAngle_58350(a1x->yaw_0x1C_28, a1x->roll_0x20_32, 5, 0x400u);
					a1x->yaw_0x1C_28 &= 0x7ff;
					/*LOWORD(v1) = */CreatureShootSpread_1D460(a1x, v6x);
				}
			}
			break;
		case 2:
			//v1 = a1x->dword_0x10_16 - 1;
			a1x->param_0x10_16--;// = v1;
			if (a1x->param_0x10_16 <= 0)
				/*LOWORD(v1) = */SentinelSetState_253B0(/*a1x->dword_0x10_16, */a1x, 2u, 3u);
			break;
		case 3:
			//v7 = a1x->dword_0x10_16 - 1;
			a1x->param_0x10_16--;// = v7;
			if (a1x->param_0x10_16 < 0)
			{
				/*LOWORD(v1) = */SentinelSetState_253B0(/*v1, */a1x, 1u, 0);
			}
			else if (a1x->param_0x10_16 >= 8)
			{
				a1x->yaw_0x1C_28 += 170;
				a1x->yaw_0x1C_28 &= 0x7ff;
				//LOWORD(v1) = a1x->word_0x1C_28;
				a1x->roll_0x20_32 = a1x->yaw_0x1C_28;
			}
			break;
		default:
			return;// v1;
		}
	}
	//return v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00025280) --------------------------------------------------------
void SentinelFollow_25280(/*int a1, */type_entity_0x6E8E* a2x)//206280
{
	/*return */SentinelSetState_253B0(/*a1, */a2x, 0, 0);
}

//----- (000252A0) --------------------------------------------------------
void SentinelPreKill_252A0(type_entity_0x6E8E* a1x)//2062a0
{
	PreKillEntity_1C890(a1x, 144);
}

//----- (000252C0) --------------------------------------------------------
void SentinelKill_252C0(type_entity_0x6E8E* a1x)//2062c0
{
	KillEntity_1C930(a1x);
}

//----- (000252E0) --------------------------------------------------------
unsigned int SentinelCheckDamage_252E0(type_entity_0x6E8E* a1x)//2062e0
{
	__int16 v1; // ax
	__int16 v2; // dx
	unsigned int result; // eax
	__int16 v4; // ax
	int i; // edx
	int v6; // ecx
	type_entity_0x6E8E* v7x; // edx

	v1 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v2 = a1x->mail_0x5E_94.channel[0].source;
	a1x->position_0x4C_76.z = v1;
	result = 0;
	if (v2)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v4 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v4;
		result = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v7x->nextLinkedId_0x34_52)
		{
			v7x = Entities_EA3E4[i];
			if (v7x == Entities_EA3E4[0])
				break;
			v6 = v7x->life_0x8;
			if (v6 < a1x->life_0x8)
			{
				a1x->life_0x8 = v6;
				a1x->hitById_0x26_38 = v7x->hitById_0x26_38;
				result = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		result = 2;
	}
	if (result >= 1)
	{
		if (result <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
		}
		else if (result == 2)
		{
			a1x->actionIndex_0x45_69 = 148;
		}
	}
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000253B0) --------------------------------------------------------
void SentinelSetState_253B0(/*int result, */type_entity_0x6E8E* a2x, unsigned __int8 a3, unsigned __int8 a4)//2063b0
{
	signed int v4; // esi
	unsigned __int64 v5; // rax
	unsigned __int64 v6; // rt2

	//int result = 0;

	if (a3 < 1u)
	{
		if (!a3)
		{
			if (a4)
			{
				if (a4 != 1)
					goto LABEL_19;
				HIDWORD(v5) = 0;
				a2x->rand_0x14_20 = 9377 * a2x->rand_0x14_20 + 9439;
				WORD1(v5) = 0;
				v4 = 60;
			}
			else
			{
				v4 = 400;
				a2x->rand_0x14_20 = 9377 * a2x->rand_0x14_20 + 9439;
				v5 = 0;
			}
			LOWORD(v5) = a2x->rand_0x14_20;
			v6 = v5 % (unsigned int)v4;
			//result = v5 / (unsigned int)v4;
			a2x->param_0x10_16 = v4 + v6;
		}
	LABEL_19:
		a2x->levelPar3_0x46_70 = a4;
		a2x->actionIndex_0x45_69 = a3 - 112;
		return;// result;
	}
	if (a3 <= 1u)
	{
		a2x->rand_0x14_20 = 9377 * a2x->rand_0x14_20 + 9439;
		//result = a2x->rand_0x14_20 / 0x190u;
		a2x->param_0x10_16 = a2x->rand_0x14_20 % 0x190u + 400;
		a2x->levelPar3_0x46_70 = a4;
		a2x->actionIndex_0x45_69 = a3 - 112;
		return;// result;
	}
	if (a3 != 2 || a4 > 3u)
		goto LABEL_19;
	//result = a4;
	switch (a4)
	{
	case 0u:
		a2x->rand_0x14_20 = 9377 * a2x->rand_0x14_20 + 9439;
		//result = a2x->rand_0x14_20 / 200;
		a2x->param_0x10_16 = a2x->rand_0x14_20 % 200 + 200;
		a2x->levelPar3_0x46_70 = a4;
		a2x->actionIndex_0x45_69 = 146;
		break;
	case 1u:
		a2x->param_0x10_16 = 10;
		a2x->levelPar3_0x46_70 = a4;
		a2x->actionIndex_0x45_69 = 146;
		break;
	case 2u:
		a2x->param_0x10_16 = 12;
		a2x->levelPar3_0x46_70 = a4;
		a2x->actionIndex_0x45_69 = 146;
		break;
	case 3u:
		a2x->param_0x10_16 = 14;
		goto LABEL_19;
	}
	//return result;
}

//----- (00025550) --------------------------------------------------------
void SentinelScripted_25550(type_entity_0x6E8E* a1x)//206550
{
	//int result; // eax

	CreatureRunScript_1D5D0(a1x, 144);
	//result = sub_10C40_getTerrainAlt(&a1x->array_0x4C_76);
	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	//BYTE1(result) = a1x->byte_0x45_69;
	if (a1x->actionIndex_0x45_69 == 146)
		/*result = */SentinelSetState_253B0(/*result, */a1x, 2u, 0);
	//return result;
}

//----- (00025590) --------------------------------------------------------
void FireflyIdle_25590(type_entity_0x6E8E* a1x)//206590
{
	//signed int result; // eax

	/*result = */CreatureIdle_1BD90(a1x, 152);
	//BYTE1(result) = *(x_BYTE *)(a1 + 69);
	if (a1x->actionIndex_0x45_69 == 154)
		a1x->levelPar3_0x46_70 = 0;
	//return result;
}

//----- (000255C0) --------------------------------------------------------
void FireflyWander_255C0(type_entity_0x6E8E* a1x)//2065c0
{
	CreatureWander_1BF90(a1x, 152);
	if (a1x->actionIndex_0x45_69 == 154)
		a1x->levelPar3_0x46_70 = 0;
}

//----- (00025610) --------------------------------------------------------
void HitFirefly_25610(type_entity_0x6E8E* a1x)//206610
{
	unsigned int v1; // eax
	__int16 v2; // ax
	int i; // edx
	int v4; // edx
	type_entity_0x6E8E* v5x; // ecx
	__int16 v6; // ax
	type_entity_0x6E8E* v7x; // eax
	__int16 v8; // ax
	__int16 v9; // ax
	char v10; // dh
	type_entity_0x6E8E* jx; // ecx
	__int16 v12; // cx
	int v13; // eax
	int v14; // edx
	type_entity_0x6E8E* v15x; // ecx
	unsigned int v16; // edx
	signed __int16 v17; // dx
	__int16 v18; // dx
	__int16 v19; // ST0C_2
	__int16 v20; // ax
	int v21; // edi
	type_entity_0x6E8E* v22x; // ecx
	int v23; // ecx
	char v24; // dh
	//int v25; // ecx
	unsigned int v26; // esi
	__int16 v27; // ax
	__int16 v28; // ax
	//int v29; // edx
	type_entity_0x6E8E* v30x; // ecx
	//int v31; // ecx
	unsigned int v32; // esi
	__int16 v33; // ax
	type_entity_0x6E8E* v34x; // [esp+0h] [ebp-14h]

	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				v1 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1 = 2;
	}
	if (v1 < 1)
	{
		if (!v1)
		{
			MoveCreature_1B8C0(a1x);
			v7x = GetCreatureTarget_1ED30(a1x, Entities_EA3E4[a1x->targetId_0x96_150]);
			v34x = v7x;
			if (v7x <= Entities_EA3E4[0] || v7x->life_0x8 < 0 || v7x->flags_0xc_12_15.byte[1] & 4)
			{
			LABEL_92:
				v33 = a1x->minSpeed_0x84_132;
				a1x->actionIndex_0x45_69 = 153;
				a1x->actSpeed_0x82_130 = v33;
			}
			else
			{
				switch (a1x->levelPar3_0x46_70)
				{
				case 0:
					v8 = a1x->minSpeed_0x84_132;
					a1x->levelPar3_0x46_70 = 1;
					a1x->actSpeed_0x82_130 = v8;
					goto LABEL_25;
				case 1:
				LABEL_25:
					predictedAxis_EB398ar = v34x->position_0x4C_76;
					a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
					MoveEntity_57FA0(
						&predictedAxis_EB398ar,
						(v34x->yaw_0x1C_28 - 256 + (signed int)(a1x->rand_0x14_20 % 0x5Au << 11) / 360) & 0x7FF,
						0,
						2048);
					if (Distance3D_583F0(&a1x->position_0x4C_76, &predictedAxis_EB398ar) <= 0x500)
					{
						a1x->levelPar3_0x46_70 = 2;
					}
					else
					{
						v9 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
						v10 = a1x->instanceIndex_0x3E_62;
						a1x->roll_0x20_32 = v9;
						if (v10 & 3)
							return;
						for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
						{
							if (jx->id_0x1A_26 != a1x->id_0x1A_26
								&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch//20680F movsx edx, word ptr [ebx+4Ch]
								&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
							{
								a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
								return;
							}
						}
					}
					return;
				case 2:
					v12 = a1x->rand_0x14_20;
					a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
					a1x->rand_0x14_20 = 9377 * v12 + 9439;
					v13 = v7x->position_0x4C_76.z;
					v14 = a1x->rand_0x14_20 & 0x3FF;
					a1x->levelPar3_0x46_70 = 3;
					a1x->param_0x10_16 = v14 + v13;
					goto LABEL_37;
				case 3:
				LABEL_37:
					a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v34x->position_0x4C_76);
					if (a1x->instanceIndex_0x3E_62 & 3)
						return;
					v15x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
					while (2)
					{
						if (v15x > Entities_EA3E4[0])
						{
							if (v15x->id_0x1A_26 == a1x->id_0x1A_26
								|| abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v15x->position_0x4C_76.x) >= a1x->params_0x52_82.pitch//2068FC movsx edx, word ptr [ebx+4Ch]
								|| abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v15x->position_0x4C_76.y) >= a1x->params_0x52_82.pitch)
							{
								v15x = v15x->next_0;
								continue;
							}
							a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v15x->position_0x4C_76, &a1x->position_0x4C_76);
						}
						break;
					}
					predictedAxis_EB398ar = v34x->position_0x4C_76;
					MoveEntity_57FA0(&predictedAxis_EB398ar, v34x->yaw_0x1C_28, 0, 2048);
					if (Distance3D_583F0(&a1x->position_0x4C_76, &predictedAxis_EB398ar) > 0x500)
					{
					LABEL_57:
						a1x->levelPar3_0x46_70 = 0;
					}
					else
					{
						a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
						v16 = a1x->rand_0x14_20 % 0x11Fu;
						if (!(v16 & 0x3F))
							a1x->levelPar3_0x46_70 = 6;
						if (!(v16 & 0x1F))
							a1x->levelPar3_0x46_70 = 7;
						if (!v16)
							a1x->levelPar3_0x46_70 = 4;
						if (!(v16 & 3))
						{
							if (a1x->position_0x4C_76.z <= a1x->param_0x10_16)
								v17 = 64;
							else
								v17 = -64;
							a1x->position_0x4C_76.z += v17;
						}
					}
					return;
				case 4:
					v27 = a1x->minSpeed_0x84_132;
					a1x->levelPar3_0x46_70 = 5;
					a1x->actSpeed_0x82_130 = v27;
					goto LABEL_81;
				case 5:
				LABEL_81:
					if (a1x->instanceIndex_0x3E_62 & 3)
						goto LABEL_89;
					v28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v34x->position_0x4C_76);
					//v29 = (int)x_D41A0_BYTEARRAY_4;
					a1x->roll_0x20_32 = v28;
					v30x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
					while (2)
					{
						if (v30x > Entities_EA3E4[0])
						{
							if (v30x->id_0x1A_26 == a1x->id_0x1A_26
								|| abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v30x->position_0x4C_76.x) >= a1x->params_0x52_82.pitch//206AC6 movsx edx, word ptr [ebx+4Ch]
								|| abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v30x->position_0x4C_76.y) >= a1x->params_0x52_82.pitch)
							{
								v30x = v30x->next_0;
								continue;
							}
							a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v30x->position_0x4C_76, &a1x->position_0x4C_76);
						}
						break;
					}
				LABEL_89:
					//v31 = a1x->dword_0xA0_160;
					if (a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26)
						return;
					v32 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					if (Distance3D_583F0(&a1x->position_0x4C_76, &v34x->position_0x4C_76) >= v32)
						goto LABEL_79;
					CreatureShootFireball_1CC20(a1x, v34x);
					return;
				case 6:
					goto LABEL_92;
				case 7:
					v18 = a1x->rand_0x14_20;
					a1x->actSpeed_0x82_130 = 3 * a1x->minSpeed_0x84_132;
					a1x->rand_0x14_20 = 9377 * v18 + 9439;
					v19 = (a1x->rand_0x14_20 & 1) + 43;
					v20 = a1x - world_D41A0.entities_0x6E8E;
					a1x->param_0x10_16 = 24;
					a1x->levelPar3_0x46_70 = 8;
					PrepareEventSound_6E450(v20, -1, v19);
					goto LABEL_59;
				case 8:
				case 9:
				LABEL_59:
					v21 = a1x->param_0x10_16 - 1;
					a1x->param_0x10_16 = v21;
					if (!v21)
						goto LABEL_57;
					if (a1x->instanceIndex_0x3E_62 & 3)
						goto LABEL_70;
					if (v21 > 16)
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v34x->position_0x4C_76);
					v22x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
					break;
				default:
					return;
				}
				while (v22x > Entities_EA3E4[0])
				{
					if (v22x->id_0x1A_26 != a1x->id_0x1A_26
						&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v22x->position_0x4C_76.x) < a1x->params_0x52_82.pitch//206C0F movsx edx, word ptr [ebx+4Ch]
						&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v22x->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
					{
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v22x->position_0x4C_76, &a1x->position_0x4C_76);
						break;
					}
					v22x = v22x->next_0;
				}
			LABEL_70:
				v23 = v34x->position_0x4C_76.z - a1x->position_0x4C_76.z;
				if (abs(v23) > 64)
				{
					if (v23 <= 0)
						LOWORD(v23) = -64;
					else
						LOWORD(v23) = 64;
				}
				v24 = a1x->levelPar3_0x46_70;
				a1x->position_0x4C_76.z += v23;
				if (v24 == 8 && (unsigned __int16)CreatureMeleeAttack768_1CED0(a1x, v34x))
					a1x->levelPar3_0x46_70 = 9;
				//v25 = a1x->dword_0xA0_160;
				if (!(a1x->instanceIndex_0x3E_62 % a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26))
				{
					v26 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
					if (Distance3D_583F0(&a1x->position_0x4C_76, &v34x->position_0x4C_76) >= v26)
						LABEL_79:
					a1x->levelPar3_0x46_70 = 6;
				}
			}
		}
	}
	else if (v1 <= 1)
	{
		v6 = a1x->hitById_0x26_38;
		a1x->levelPar3_0x46_70 = 7;
		a1x->targetId_0x96_150 = v6;
	}
	else if (v1 == 2)
	{
		a1x->actionIndex_0x45_69 = 156;
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00025CD0) --------------------------------------------------------
void FireflyFollow_25CD0(type_entity_0x6E8E* a1x)//206cd0
{
	//char result; // al

	/*result = */CreatureFollowLeader_1C560(a1x, 0x98u);
	if (a1x->actionIndex_0x45_69 == 154)
		a1x->levelPar3_0x46_70 = 0;
	//return result;
}

//----- (00025D00) --------------------------------------------------------
void FireflyPreKill_25D00(type_entity_0x6E8E* a1x)//206d00
{
	PreKillEntity_1C890(a1x, 152);
}

//----- (00025D20) --------------------------------------------------------
void FireflyKill_25D20(type_entity_0x6E8E* a1x)//206d20
{
	KillEntity_1C930(a1x);
}

//----- (00025D50) --------------------------------------------------------
void FireflyRunScript_25D50(type_entity_0x6E8E* a1x)//206d50
{
	CreatureRunScript_1D5D0(a1x, 152);
	if (a1x->actionIndex_0x45_69 == 154)
		a1x->levelPar3_0x46_70 = 0;
}

//----- (00025D80) --------------------------------------------------------
void SpiderIdle_25D80(type_entity_0x6E8E* a1x)//206d80
{
	type_entity_0x6E8E* resultx; // eax
	char v2; // dh

	/*result = */CreatureIdle_1BD90(a1x, 160);
	//BYTE1(result) = *(x_BYTE *)(a1 + 69);
	if (a1x->actionIndex_0x45_69 == 162)
	{
		resultx = Entities_EA3E4[a1x->targetId_0x96_150];
		if (resultx <= Entities_EA3E4[0] || resultx->class_0x3F_63 != 3 || (v2 = resultx->model_0x40_64) != 0 && v2 != 1)
			a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00025DE0) --------------------------------------------------------
void SpiderWander_25DE0(type_entity_0x6E8E* a1x)//206de0
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dh

	CreatureWander_1BF90(a1x, 160);
	if (a1x->actionIndex_0x45_69 == 162)
	{
		v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v1x <= Entities_EA3E4[0] || v1x->class_0x3F_63 != 3 || (v2 = v1x->model_0x40_64) != 0 && v2 != 1)
			a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00025E40) --------------------------------------------------------
void SpiderAttack_25E40(type_entity_0x6E8E* a1x)//206e40
{
	type_entity_0x6E8E* v1x; // esi
	unsigned __int8 v2; // al
	char v3; // al
	bool v4; // zf
	//__int16 v5; // dx
	//int v6; // ecx
	//__int16 v7; // ax
	char v8; // [esp+0h] [ebp-4h]

	v1x = Entities_EA3E4[a1x->targetId_0x96_150];
	v8 = 0;
	if (v1x <= Entities_EA3E4[0])
	{
		a1x->actionIndex_0x45_69 = 161;
		goto LABEL_21;
	}
	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 32);
	v2 = a1x->levelPar3_0x46_70;
	if (v2 < 1u)
	{
		if (!v2)
		{
			v3 = CreatureAttack_1C310(a1x, 160, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureShootProjectile21_1D1A0);
			if (v1x->class_0x3F_63 != 3 || v1x->model_0x40_64)
				v4 = v3 == 0;
			else
				v4 = v1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334 == 0;
			if (!v4)
				a1x->levelPar3_0x46_70 = 1;
		}
	}
	else
	{
		if (v2 <= 1u)
		{
			a1x->levelPar3_0x46_70 = 2;
			//v5 = a1x->word_0x84_132;
			a1x->param_0x10_16 = 32;
			a1x->actSpeed_0x82_130 = 2 * a1x->minSpeed_0x84_132;
		}
		else if (v2 != 2)
		{
			goto LABEL_21;
		}
		if (CreatureAttack_1C310(a1x, 160, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureMeleeAttack1024_1CE80))
			v8 = 1;
		//v6 = a1x->dword_0x10_16 - 1;
		//a1x->dword_0x10_16 = v6;
		if (!(--a1x->param_0x10_16))
			v8 = 1;
		if (v8)
		{
			//v7 = a1x->word_0x84_132;
			a1x->levelPar3_0x46_70 = 0;
			a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
		}
	}
LABEL_21:
	if (a1x->actionIndex_0x45_69 != 162)
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00025F70) --------------------------------------------------------
void SpiderFollow_25F70(type_entity_0x6E8E* a1x)//206f70
{
	type_entity_0x6E8E* v1x; // eax
	char v2; // dh

	/*LOBYTE(v1) = */CreatureFollowLeader_1C560(a1x, 0xA0u);
	if (a1x->actionIndex_0x45_69 == 162)
	{
		v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v1x <= Entities_EA3E4[0] || v1x->class_0x3F_63 != 3 || (v2 = v1x->model_0x40_64) != 0 && v2 != 1)
			a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
	//return v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00025FD0) --------------------------------------------------------
void SpiderPreKill_25FD0(type_entity_0x6E8E* a1x)//206fd0
{
	PreKillEntity_1C890(a1x, 160);
}

//----- (00025FF0) --------------------------------------------------------
void SpiderKill_25FF0(type_entity_0x6E8E* a1x)//206ff0
{
	KillEntity_1C930(a1x);
}

//----- (00026020) --------------------------------------------------------
void SpiderScripted_26020(type_entity_0x6E8E* a1x)//207020
{
	CreatureRunScript_1D5D0(a1x, 160);
	if (a1x->actionIndex_0x45_69 == 162)
		a1x->levelPar3_0x46_70 = 0;
}

//----- (00026050) --------------------------------------------------------
type_entity_0x6E8E* DevilIdle_26050(type_entity_0x6E8E* a1x)//207050
{
	return DevilSetMode_268F0(a1x, 1u);
}

//----- (00026070) --------------------------------------------------------
char DevilWander_26070(type_entity_0x6E8E* a1x)//207070
{
	signed int v1; // eax
	unsigned __int16 v2; // ax
	int v3; // eax
	type_entity_0x6E8E* v4x; // edi
	type_entity_0x6E8E* v5x; // esi
	int v6; // eax
	int v7; // edx
	int v8; // eax
	signed int v9; // ST08_4
	__int16 v10; // ax
	unsigned int v12; // [esp+4h] [ebp-Ch]
	unsigned int v13; // [esp+8h] [ebp-8h]
	unsigned int v14; // [esp+Ch] [ebp-4h]

	v1 = DevilCheckDamage_26830(a1x);
	if ((unsigned int)v1 <= 1)
	{
		MoveCreature_1B8C0(a1x);
		DevilUpdateState_265A0(a1x);
		LOBYTE(v1) = DevilIsAttacking_26930(a1x);
		if ((x_BYTE)v1)
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v2 = a1x->rand_0x14_20;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->yaw_0x1C_28 += ((a1x->rand_0x14_20 & 0xFF) + 85) * (2 * ((signed int)(v2 % 0x9Du) / 79) - 1);
			a1x->yaw_0x1C_28 &= 0x7ff;
			LOWORD(v1) = a1x->yaw_0x1C_28;
			a1x->roll_0x20_32 = v1;
		}
		if (!(a1x->instanceIndex_0x3E_62 & 0x3F) && a1x->timer_0x39_57)
		{
			v3 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			v1 = v3 * v3;
			v4x = 0;
			v13 = -1;
			v5x = gameState_D41A4.class3List_38519;
			v12 = v1;
			while (v5x > Entities_EA3E4[0])
			{
				v6 = (signed __int16)(v5x->position_0x4C_76.x - a1x->position_0x4C_76.x);
				v7 = v6 * v6;
				v8 = (signed __int16)(v5x->position_0x4C_76.y - a1x->position_0x4C_76.y);
				v1 = v8 * v8;
				v14 = v1 + v7;
				if (v1 + v7 <= v12 && !(v5x->flags_0xc_12_15.byte[0] & 0x20))
				{
					v9 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
					v10 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v5x->position_0x4C_76);
					v1 = (unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v10);
					if (v1 < v9)
					{
						LOBYTE(v1) = v14;
						if (v14 < v13)
						{
							v4x = v5x;
							v13 = v14;
						}
					}
				}
				v5x = v5x->next_0;
			}
			if (v4x)
			{
				a1x->targetId_0x96_150 = v4x - world_D41A0.entities_0x6E8E;
				/*LOBYTE(v1) = */DevilSetMode_268F0(a1x, 2u);
			}
		}
	}
	return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00026220) --------------------------------------------------------
void DevilAttack_26220(/*type_str_0x6E8E* a1x, */type_entity_0x6E8E* a2x)//207220
{
	unsigned int result; // eax
	unsigned __int16 v3; // dx
	type_entity_0x6E8E* v4x; // eax
	type_entity_0x6E8E* ix; // ecx
	unsigned int v6; // edi
	char v7; // [esp+8h] [ebp-8h]
	char v8; // [esp+Ch] [ebp-4h]

	v4x = NULL;//fix

	v7 = 1;
	v8 = 0;
	result = DevilCheckDamage_26830(a2x);
	if (result <= 1)
	{
		v3 = a2x->targetId_0x96_150;
		if (v3)
		{
			v4x = GetCreatureTarget_1ED30(a2x, Entities_EA3E4[v3]);
			//a1x = v4x;
			if (v4x <= Entities_EA3E4[0] || v4x->life_0x8 < 0 || v4x->flags_0xc_12_15.byte[1] & 4)
				v7 = 0;
		}
		else
		{
			v7 = 0;
		}
		if (!v7)
			goto LABEL_26;
		if (DevilIsAttacking_26930(a2x))
		{
			a2x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a2x->position_0x4C_76, &v4x->position_0x4C_76);
			if (!(a2x->instanceIndex_0x3E_62 & 3))
			{
				for (ix = gameState_D41A4.creatureLists_38403x[a2x->model_0x40_64]; ix > Entities_EA3E4[0]; ix = ix->next_0)
				{
					if (ix->id_0x1A_26 != a2x->id_0x1A_26
						&& abs((int16_t)a2x->position_0x4C_76.x - (int16_t)ix->position_0x4C_76.x) < a2x->params_0x52_82.pitch
						&& abs((int16_t)a2x->position_0x4C_76.y - (int16_t)ix->position_0x4C_76.y) < a2x->params_0x52_82.pitch)
					{
						a2x->roll_0x20_32 = Maths::GetYawAngle_581E0(&ix->position_0x4C_76, &a2x->position_0x4C_76);
						break;
					}
				}
			}
		}
		if (a2x->instanceIndex_0x3E_62 & 0x1F)
			goto LABEL_22;
		v6 = a2x->typeParams_0xA0_160x->range_160_0x1c_28;
		if (Distance3D_583F0(&a2x->position_0x4C_76, &v4x->position_0x4C_76) < v6)
			CreatureShootFireball_1CC20(a2x, v4x);
		else
			LABEL_26:
		v8 = 1;
	LABEL_22:
		MoveCreature_1B8C0(a2x);
		/*result = */DevilUpdateState_265A0(a2x);
		if (v8)
			/*result = */DevilSetMode_268F0(a2x, 1u);
	}
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000263C0) --------------------------------------------------------
type_entity_0x6E8E* DevilFollow_263C0(type_entity_0x6E8E* a1x)//2073c0
{
	return DevilSetMode_268F0(a1x, 1u);
}

//----- (000263E0) --------------------------------------------------------
void DevilPreKill_263E0(type_entity_0x6E8E* a1x)//2073e0
{
	PreKillEntity_1C890(a1x, 168);
}

//----- (00026400) --------------------------------------------------------
void DevilKill_26400(type_entity_0x6E8E* a1x)//207400
{
	KillEntity_1C930(a1x);
}

int debug_sub_26470 = 0;
//----- (00026470) --------------------------------------------------------
void DevilScripted_26470(type_entity_0x6E8E* a1x)//207470
{
	//int result; // eax

	if (CommandLineParams.DoDebugSequences()) {
		if (debug_sub_26470 >= 0x153c)
		{
			debug_sub_26470++;
			debug_sub_26470--;
		}
		debug_sub_26470++;
	}

	CreatureRunScript_1D5D0(a1x, 168);
	//result = (unsigned __int8)(a1x->StageVar2_0x49_73 - 1);
	switch (a1x->StageVar2_0x49_73)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 0xA:
		/*result = */DevilUpdateState_265A0(a1x);
		break;
	case 0xD:
	case 0xE:
	case 0x10:
		a1x->levelPar1_0x43_67 = 0;
		/*result = */DevilUpdateState_265A0(a1x);
		break;
	default:
		break;
	}
	//HIBYTE(result) = a1x->byte_0x45_69;
	if (a1x->actionIndex_0x45_69 != 175)
		/*result = */DevilSetMode_268F0(a1x, a1x->actionIndex_0x45_69 + 88);
	//return result;
}

//----- (00026500) --------------------------------------------------------
void DevilSetSprite_26500(/*int result, */type_entity_0x6E8E* a2x)//207500
{
	int result;
	switch (a2x->levelPar3_0x46_70)
	{
	case 0:
		result = 311;
		break;
	case 1:
	case 2:
	case 3:
	case 9:
		result = 308;
		break;
	case 4:
		result = 309;
		break;
	case 5:
		result = 310;
		break;
	case 6:
		result = 305;
		break;
	case 7:
		result = 306;
		break;
	case 8:
		result = 307;
		break;
	case 0xA:
		result = 312;
		break;
	default:
		break;
	}
	if (a2x->spriteIndex_0x5A_90 != result)
		/*result = */SetEntityIndexAndRot_49CD0(a2x, result);
	//return result;
}

//----- (000265A0) --------------------------------------------------------
void DevilUpdateState_265A0(type_entity_0x6E8E* a1x)//2075a0
{
	char v1; // dh
	__int16 v2; // dx
	int v3; // esi
	unsigned int v4; // esi
	unsigned int v5; // edx
	signed int v6; // esi
	__int16 v7; // cx
	int v8; // edi
	int v9; // eax
	int v10; // eax
	char v12; // [esp+0h] [ebp-8h]
	char v13; // [esp+4h] [ebp-4h]

	v12 = 0;
	v13 = 1;
	switch (a1x->levelPar3_0x46_70)
	{
	case 0:
	case 1:
		v1 = a1x->levelPar2_0x44_68;
		if (v1)
			a1x->levelPar2_0x44_68 = v1 - 1;
		else
			a1x->levelPar3_0x46_70 = 2;
		v12 = 1;
		v13 = 0;
		break;
	case 2:
		v12 = 1;
		a1x->levelPar3_0x46_70 = 3;
		v13 = 0;
		break;
	case 3:
		a1x->levelPar3_0x46_70 = 4;
		break;
	case 4:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v2 = a1x->rand_0x14_20 % 0x64u;
		a1x->levelPar3_0x46_70 = 5;
		a1x->param2_0x2C_44 = v2 + 140;
		break;
	case 5:
		if (a1x->param2_0x2C_44 < 0)
			a1x->levelPar3_0x46_70 = 6;
		break;
	case 6:
		v3 = a1x->position_0x4C_76.z;
		if (v3 - (signed __int16)GetTerrainAlt_10C40(&a1x->position_0x4C_76) < 230)
			a1x->levelPar3_0x46_70 = 7;
		break;
	case 7:
		a1x->levelPar3_0x46_70 = 8;
		v13 = 0;
		break;
	case 8:
		v12 = 1;
		a1x->levelPar3_0x46_70 = 9;
		v13 = 0;
		break;
	case 9:
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		if (!(a1x->rand_0x14_20 % 0xBu))
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 42);
		if (a1x->levelPar1_0x43_67)
		{
			v4 = a1x->levelPar1_0x43_67;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v5 = a1x->rand_0x14_20 % v4;
			a1x->levelPar2_0x44_68 = v5;
			a1x->levelPar3_0x46_70 = (v5 & 1) == 0;
		}
		else
		{
			a1x->levelPar2_0x44_68 = 1;
			a1x->levelPar3_0x46_70 = 0;
		}
		v12 = 1;
		v13 = 0;
		break;
	case 0xA:
		v12 = 1;
		break;
	default:
		break;
	}
	v6 = (signed __int16)GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	if (v12)
	{
		a1x->position_0x4C_76.z -= (int16_t)42;
	}
	else
	{
		v7 = a1x->param2_0x2C_44 - (int16_t)42;
		a1x->position_0x4C_76.z += a1x->param2_0x2C_44;
		a1x->param2_0x2C_44 = v7;
	}
	if ((int16_t)a1x->position_0x4C_76.z < v6)
		a1x->position_0x4C_76.z = v6;
	if (isCaveLevel_D41B6)
	{
		v8 = a1x->params_0x52_82.fov;
		v9 = (signed __int16)GetCeilingAlt_10C60(&a1x->position_0x4C_76) - v8;
		if ((int16_t)a1x->position_0x4C_76.z > v9)
		{
			a1x->param2_0x2C_44 = 0;
			a1x->position_0x4C_76.z = v9;
		}
	}
	if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
	{
		if (a1x->levelPar3_0x46_70 == 10)
		{
			if ((int16_t)a1x->position_0x4C_76.z > v6)
				a1x->levelPar3_0x46_70 = 0;
		}
		else if (a1x->position_0x4C_76.z == v6)
		{
			a1x->levelPar3_0x46_70 = 10;
			CreateEntity_4A190(&predictedAxis_EB398ar, 10, 5);//this set 02 2077b2
		}
		if (a1x->actionIndex_0x45_69 == 170)
			v10 = 66;
		else
			v10 = 40;
	}
	else
	{
		if (a1x->levelPar3_0x46_70 == 10)
			a1x->levelPar3_0x46_70 = 0;
		if (a1x->actionIndex_0x45_69 == 170)
			v10 = 96;
		else
			v10 = 60;
	}
	a1x->actSpeed_0x82_130 = v10;
	DevilSetSprite_26500(a1x);
	if (!v13)
		a1x->flags_0xc_12_15.byte[1] |= 8u;
}

//----- (00026830) --------------------------------------------------------
signed int DevilCheckDamage_26830(type_entity_0x6E8E* a1x)//207830
{
	__int16 v1; // dx
	unsigned int v2; // ebx
	int i; // edx
	int v4; // ecx
	type_entity_0x6E8E* v5x; // edx

	v1 = a1x->mail_0x5E_94.channel[0].source;
	v2 = 0;
	if (v1)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v1 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v2 = 1;
	}
	a1x->hitById_0x26_38 = v1;
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				v2 = 1;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v2 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v2 >= 1)
	{
		if (v2 <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
			DevilSetMode_268F0(a1x, 2u);
		}
		else if (v2 == 2)
		{
			a1x->actionIndex_0x45_69 = 172;
			return 2;
		}
	}
	return v2;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000268F0) --------------------------------------------------------
type_entity_0x6E8E* DevilSetMode_268F0(type_entity_0x6E8E* a1x, unsigned __int8 a2)//2078f0
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	if (a2 >= 1u)
	{
		if (a2 <= 1u)
		{
			a1x->levelPar1_0x43_67 = 64;
			a1x->targetId_0x96_150 = 0;
		}
		else if (a2 == 2)
		{
			a1x->levelPar1_0x43_67 = 0;
		}
	}
	a1x->actionIndex_0x45_69 = a2 - 88;
	return resultx;
}

//----- (00026930) --------------------------------------------------------
char DevilIsAttacking_26930(type_entity_0x6E8E* a1x)//207930
{
	char result; // al
	unsigned __int8 v2; // ah

	result = 0;
	v2 = a1x->levelPar3_0x46_70;
	if (v2 >= 9u && (v2 <= 9u || v2 == 10 && !(a1x->instanceIndex_0x3E_62 & 7)))
		result = 1;
	return result;
}

//----- (00026960) --------------------------------------------------------
void FlyingManaIdle_26960(type_entity_0x6E8E* a1x)//207960
{
	FlyingManaUpdateSpeed_26FF0(a1x);
	FlyingManaUpdateAnimation_272C0(a1x);
	FlyingManaApplyDamage_26F10(a1x);
	FlyingManaCountdown_27880(a1x);
}

void FlyingManaWander_26990(type_entity_0x6E8E* a1x)//207990
{
	__int16 v1; // si
	signed __int16 v2; // di
	type_entity_0x6E8E* resultx; // eax
	int v4; // edx
	__int16 v5; // ax
	type_entity_0x6E8E* v6x; // [esp+0h] [ebp-1Ch]
	__int16 v7; // [esp+4h] [ebp-18h]
	int v8; // [esp+8h] [ebp-14h]
	__int16 v9; // [esp+10h] [ebp-Ch]
	char v10; // [esp+18h] [ebp-4h]

	v10 = 1;
	FlyingManaUpdateSpeed_26FF0(a1x);
	FlyingManaUpdateAnimation_272C0(a1x);
	v8 = a1x->param_0x10_16 >> 8;
	v1 = a1x->param_0x10_16 & 0xFF;
	v2 = 0;
	//result = (uint8_t*)sub_369F0((int)a1, *(x_WORD *)(a1 + 148));
	v9 = GetManaSphereColorIndexFromEntityId_369F0(/*a1x, */a1x->playerEntityIndex_0x94_148);
	while (v2 < (signed __int16)((v1 != 0) + 1))
	{
		if (v2)
			v4 = -v1;
		else
			LOWORD(v4) = v1;
		v7 = v4 + v8;
		//result = abs((signed __int16)(v4 + v8));
		if (abs((signed __int16)(v4 + v8)) <= a1x->levelPar3_0x46_70 / 2)
		{
			resultx = GetChainSegment_27470(a1x, v7);
			v6x = resultx;
			if (resultx)
			{
				v5 = GetManaSpriteIndex_278F0(v9, a1x->levelPar3_0x46_70, v7);
				/*result = */SetEntitySprite_49D50(v6x, v5);
				v10 = 0;
			}
		}
		v2++;
	}
	if (v10)
	{
		if (a1x->playerEntityIndex_0x94_148)
			a1x->actionIndex_0x45_69 = 178;
		else
			a1x->actionIndex_0x45_69 = 176;
	}
	else
	{
		//result = (signed __int16)(v1 + 1);
		a1x->param_0x10_16 = (signed __int16)(v1 + 1) | ((signed __int16)v8 << 8);
	}
	//return result;
}

//----- (00026AA0) --------------------------------------------------------
void FlyingManaAttack_26AA0(type_entity_0x6E8E* a1x)//207aa0
{
	unsigned __int16 v1; // dx
	type_entity_0x6E8E* v2x; // eax
	unsigned __int16 v3; // si
	type_entity_0x6E8E* v4x; // esi
	__int16 v5; // ax
	char v6; // ch
	char v7; // [esp+4h] [ebp-4h]

	v7 = 0;
	FlyingManaUpdateSpeed_26FF0(a1x);
	FlyingManaUpdateAnimation_272C0(a1x);
	FlyingManaApplyDamage_26F10(a1x);
	FlyingManaCountdown_27880(a1x);
	if (!(a1x->instanceIndex_0x3E_62 & 0x1F))
	{
		v1 = a1x->playerEntityIndex_0x94_148;
		if (!v1)
			goto LABEL_17;
		if (a1x->actSpeed_0x82_130 > a1x->maxSpeed_0x86_134)
			goto LABEL_13;
		v2x = Entities_EA3E4[v1];
		if (v2x->class_0x3F_63 != 3)
			goto LABEL_17;
		if (v2x->life_0x8 < 0)
			goto LABEL_17;
		if (v2x->flags_0xc_12_15.byte[1] & 4)
			goto LABEL_17;
		v3 = v2x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
		if (!v3)
			goto LABEL_17;
		v4x = Entities_EA3E4[v3];
		v5 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v4x->position_0x4C_76);
		v6 = a1x->instanceIndex_0x3E_62;
		a1x->roll_0x20_32 = v5;
		if (v6 & 3 || Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v4x->position_0x4C_76) > 0x100)
			goto LABEL_13;
		if (a1x->mana_0x90_144 + v4x->mana_0x90_144 < v4x->maxMana_0x8C_140)
		{
			a1x->param_0x10_16 = 128;
			a1x->actionIndex_0x45_69 = 179;
		}
		else
		{
		LABEL_17:
			v7 = 1;
		}
	LABEL_13:
		if (v7)
		{
			a1x->param_0x10_16 = 0;
			a1x->playerEntityIndex_0x94_148 = 0;
			a1x->actionIndex_0x45_69 = 177;
		}
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00026BD0) --------------------------------------------------------
void FlyingManaFollow_26BD0(type_entity_0x6E8E* a1x)//207bd0
{
	int v1; // edx
	char v2; // dl
	type_entity_0x6E8E* v3x; // eax
	unsigned __int16 v4; // dx
	type_entity_0x6E8E* v5x; // eax
	int v6; // edi
	int v7; // edx

	FlyingManaUpdateAnimation_272C0(a1x);
	v1 = a1x->param_0x10_16;
	if (v1)
	{
		a1x->param_0x10_16 = v1 - 1;
	}
	else if (!(a1x->instanceIndex_0x3E_62 & 1))
	{
		v2 = a1x->levelPar3_0x46_70;
		if (v2 > 1)
		{
			SetFlyingManaLevel_27720(a1x, v2 - 2);
		}
		else
		{
			v3x = Entities_EA3E4[a1x->playerEntityIndex_0x94_148];
			if (v3x->class_0x3F_63 == 3 && v3x->life_0x8 >= 0 && !(v3x->flags_0xc_12_15.byte[1] & 4))
			{
				v4 = v3x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
				if (v4)
				{
					v5x = Entities_EA3E4[v4];
					v6 = v5x->maxMana_0x8C_140;
					v7 = a1x->mana_0x90_144 + v5x->mana_0x90_144;
					if (v7 >= v6)
						v5x->mana_0x90_144 = v6;
					else
						v5x->mana_0x90_144 = v7;
				}
			}
			DisableEntityDrawing04_57F10(a1x);
		}
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00026CA0) --------------------------------------------------------
void FlyingManaPreKill_26CA0(type_entity_0x6E8E* a1x)//207ca0
{
	FlyingManaSteerToTarget_271D0(a1x);
	FlyingManaFollowTarget_26D20(a1x);
}

//----- (00026CC0) --------------------------------------------------------
void FlyingManaKill_26CC0(type_entity_0x6E8E* a1x)//207cc0
{
	int i; // ebx
	type_entity_0x6E8E* v2x; // ebx

	for (i = a1x->nextLinkedId_0x34_52; ; i = v2x->nextLinkedId_0x34_52)
	{
		v2x = Entities_EA3E4[i];
		if (v2x == Entities_EA3E4[0])
			break;
		TransformEntityToManaSphere_36BA0(v2x, false);
		DisableEntityDrawing04_57F10(v2x);
	}
	TransformEntityToManaSphere_36BA0(a1x, false);
	DisableEntityDrawing04_57F10(a1x);
	//return 1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00026D20) --------------------------------------------------------
void FlyingManaFollowTarget_26D20(type_entity_0x6E8E* a1x)//207d20
{
	type_entity_0x6E8E* v1x; // ebx
	unsigned __int8 v2; // al
	__int16 v3; // ax
	int v4; // eax
	int v5; // ecx
	int v6; // eax
	type_entity_0x6E8E* v7x; // eax
	__int16 v8; // cx
	__int16 v9; // ax
	__int16 v10; // ax
	int i; // eax
	type_entity_0x6E8E* v12x; // eax

	if (a1x->timer_0x39_57)
	{
		v1x = Entities_EA3E4[a1x->targetId_0x96_150];
		v2 = v1x->actionIndex_0x45_69;
		if (v2 >= 0xB0u && (v2 <= 0xB0u || v2 == 178))
		{
			if (a1x->mail_0x5E_94.channel[0].source)
			{
				v1x->actSpeed_0x82_130 = ((v1x->minSpeed_0x84_132 - v1x->maxSpeed_0x86_134) >> 2)
					+ v1x->maxSpeed_0x86_134;
				v3 = Maths::GetYawAngle_581E0(&Entities_EA3E4[a1x->mail_0x5E_94.channel[0].source]->position_0x4C_76, &a1x->position_0x4C_76);
				v1x->yaw_0x1C_28 = v3;
				v1x->roll_0x20_32 = v3;
				v4 = 56 * abs(a1x->levelPar3_0x46_70) / (v1x->levelPar3_0x46_70 >> 1);
				if (((v1x->yaw_0x1C_28 - a1x->param2_0x2C_44) & 0x7FF) >= 1024)
					v4 = -v4;
				v5 = v4 + v1x->param2_0x2C_44;
				if (abs(v5) < 11)
				{
					if (v5 <= 0)
						v5 = -11;
					else
						v5 = 11;
				}
				if (abs(v5) > 227)
				{
					if (v5 <= 0)
						LOWORD(v5) = -227;
					else
						LOWORD(v5) = 227;
				}
				v6 = v1x->nextLinkedId_0x34_52;
				v1x->param2_0x2C_44 = v5;
				while (1)
				{
					v7x = Entities_EA3E4[v6];
					if (v7x == Entities_EA3E4[0])
						break;
					v7x->mail_0x5E_94.channel[0].source = 0;
					v6 = v7x->nextLinkedId_0x34_52;
				}
			}
			v8 = a1x->mail_0x5E_94.channel[1].source;
			if (v8)
			{
				if (v8 != v1x->playerEntityIndex_0x94_148)
				{
					if (a1x->mail_0x5E_94.channel[1].amount.dword)
					{
						v1x->actionIndex_0x45_69 = 177;
						v1x->param_0x10_16 = a1x->levelPar3_0x46_70 << 8;
						v10 = a1x->mail_0x5E_94.channel[1].source;
						v1x->playerEntityIndex_0x94_148 = v10;
						PrepareEventSound_6E450(v10, -1, 4);
						a1x->flags_0xc_12_15.byte[2] |= 0x20u;
					}
					else if (!(a1x->flags_0xc_12_15.byte[2] & 0x20))
					{
						v1x->actionIndex_0x45_69 = 177;
						v1x->param_0x10_16 = a1x->levelPar3_0x46_70 << 8;
						v9 = a1x->mail_0x5E_94.channel[1].source;
						v1x->playerEntityIndex_0x94_148 = v9;
						PrepareEventSound_6E450(v9, -1, 4);
					}
				}
				for (i = v1x->nextLinkedId_0x34_52; ; i = v12x->nextLinkedId_0x34_52)
				{
					v12x = Entities_EA3E4[i];
					if (v12x == Entities_EA3E4[0])
						break;
					v12x->mail_0x5E_94.channel[1].source = 0;
				}
			}
		}
	}
	//return 0;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00026F10) --------------------------------------------------------
void FlyingManaApplyDamage_26F10(type_entity_0x6E8E* a1x)//207f10
{
	int v1; // eax
	signed __int16 v2; // cx
	__int16 v3; // si
	__int16 v4; // ax
	__int16 v5; // di
	__int16 v6; // ax

	if (a1x->timer_0x39_57)
	{
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			v1 = (a1x->mail_0x5E_94.channel[0].amount.dword >> 2) + a1x->actSpeed_0x82_130;
			v2 = a1x->maxSpeed_0x86_134;
			a1x->actSpeed_0x82_130 = v1;
			if ((signed __int16)v1 < v2)
				a1x->actSpeed_0x82_130 = v2;
			v3 = a1x->minSpeed_0x84_132;
			if (a1x->actSpeed_0x82_130 > v3)
				a1x->actSpeed_0x82_130 = v3;
			v4 = Maths::GetYawAngle_581E0(&Entities_EA3E4[a1x->mail_0x5E_94.channel[0].source]->position_0x4C_76, &a1x->position_0x4C_76);
			a1x->mail_0x5E_94.channel[0].source = 0;
			a1x->yaw_0x1C_28 = v4;
			a1x->roll_0x20_32 = v4;
		}
		v5 = a1x->mail_0x5E_94.channel[1].source;
		if (v5)
		{
			if (v5 != a1x->playerEntityIndex_0x94_148)
			{
				v6 = a1x->mail_0x5E_94.channel[1].source;
				a1x->playerEntityIndex_0x94_148 = v6;
				a1x->actionIndex_0x45_69 = 177;
				a1x->param_0x10_16 = 0;
				PrepareEventSound_6E450(v6, -1, 4);
			}
			a1x->mail_0x5E_94.channel[1].source = 0;
		}
	}
	if (a1x->life_0x8 < 0)
		a1x->actionIndex_0x45_69 = 181;
	//return 0;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00026FF0) --------------------------------------------------------
void FlyingManaUpdateSpeed_26FF0(type_entity_0x6E8E* a1x)//207ff0
{
	int16_t v1; // ax
	int16_t v2; // si
	int16_t v3; // di
	int v4; // eax
	int16_t v5x; // eax
	int v6; // esi
	int16_t v7; // si
	axis_3d v9x; // [esp+0h] [ebp-10h]
	//__int16 v10; // [esp+4h] [ebp-Ch]
	type_entity_0x6E8E* v11x; // [esp+8h] [ebp-8h]
	int v12; // [esp+Ch] [ebp-4h]

	v1 = a1x->actSpeed_0x82_130;
	if (v1 > a1x->maxSpeed_0x86_134)
		a1x->actSpeed_0x82_130 = v1 - 2;
	v2 = a1x->params_0x52_82.fov;
	v3 = a1x->params_0x52_82.pitch;
	SetEntityShiftRot_49EA0(a1x, a1x->levelPar3_0x46_70 << 8, a1x->params_0x52_82.fov);
	MoveCreature_1B8C0(a1x);
	if (!(a1x->instanceIndex_0x3E_62 & 0xF))
		FlyingManaAvoidOthers_27120(a1x);
	v11x = a1x;
	v12 = 0;
	v9x = a1x->position_0x4C_76;//20806C: the original leaves var_10 uninitialized when sub_10C40 <= 0 (stack garbage); DOSBox sets the same
	SetEntityShiftRot_49EA0(a1x, v3, v2);
	while (v11x != Entities_EA3E4[0])
	{
		v4 = GetTerrainAlt_10C40(&v11x->position_0x4C_76);
		if ((signed __int16)v4 > (signed __int16)v12)
		{
			v12 = v4;
			v9x = v11x->position_0x4C_76;
		}
		v11x = Entities_EA3E4[v11x->nextLinkedId_0x34_52];
	}
	v12 += 384;
	v5x = a1x->position_0x4C_76.z;
	if ((int16_t)v5x >= (int16_t)v12)
	{
		v7 = a1x->ownerPlayerId_0x24_36;
		if (v7)
		{
			v5x = v7 - 1;
			a1x->ownerPlayerId_0x24_36 = v7 - 1;
		}
		else
		{
			a1x->position_0x4C_76.z = v5x - 2;
		}
	}
	else
	{
		v6 = a1x->typeParams_0xA0_160x->maxSlope_160_0x10_16;
		v5x = GetTileSlope_1B7A0(&v9x);
		if (v5x > v6)
			a1x->position_0x4C_76.z += 0x100;
		else
			a1x->position_0x4C_76.z += 0x40;
		a1x->ownerPlayerId_0x24_36 = 0x40;
	}
	//return v5;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00027120) --------------------------------------------------------
void FlyingManaAvoidOthers_27120(type_entity_0x6E8E* a1x)//208120
{
	int v1; // edi
	int v2; // esi
	type_entity_0x6E8E* v3x; // ecx
	__int16 v5; // ax

	v1 = 2 * a1x->params_0x52_82.fov + 32;
	v2 = 2 * a1x->params_0x52_82.pitch;
	v3x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
	if (v3x <= Entities_EA3E4[0])
		return;
	do
	{
		if (v3x->id_0x1A_26 != a1x->id_0x1A_26
			&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v3x->position_0x4C_76.x) < v2
			&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v3x->position_0x4C_76.y) < v2
			&& abs((int16_t)a1x->position_0x4C_76.z - (int16_t)v3x->position_0x4C_76.z) < v1)
		{
			v5 = a1x->position_0x4C_76.z;
			if (v5 >= v3x->position_0x4C_76.z)
				a1x->position_0x4C_76.z = v5 + 64;
		}
		v3x = v3x->next_0;
	} while (v3x > Entities_EA3E4[0]);
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000271D0) --------------------------------------------------------
void FlyingManaSteerToTarget_271D0(type_entity_0x6E8E* a1x)//2081d0
{
	//uint8_t* result; // eax
	type_entity_0x6E8E* v2x; // eax
	__int16 v3; // si
	unsigned __int16 v4; // ax
	type_entity_0x6E8E* v5x; // ebx
	unsigned __int16 v6; // cx

	//result = a1;
	if (a1x->targetId_0x96_150)
	{
		v2x = Entities_EA3E4[a1x->targetId_0x96_150];
		v3 = v2x->subSpellIndex_0x2A_42;
		v4 = (v3 + GetSegmentAngle_273C0(v2x->animationFrame_0x5C_92, v2x->param3_0x36_54, a1x->levelPar3_0x46_70, v2x->levelPar3_0x46_70)) & 0x7FF;
		a1x->param2_0x2C_44 = v4;
		v5x = Entities_EA3E4[a1x->leaderId_0x32_50];
		if (v5x)
		{
			v6 = v5x->leaderId_0x32_50;
			if (v6)
				v5x = Entities_EA3E4[v6];
		}
		predictedAxis_EB398ar = v5x->position_0x4C_76;
		MoveEntity_57FA0(&predictedAxis_EB398ar, v4, 0, a1x->params_0x52_82.pitch + v5x->params_0x52_82.pitch);
		predictedAxis_EB398ar.z = v5x->params_0x52_82.pitch - a1x->params_0x52_82.pitch + v5x->position_0x4C_76.z;
		/*result = */CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (000272C0) --------------------------------------------------------
void FlyingManaUpdateAnimation_272C0(type_entity_0x6E8E* a1x)//2082c0
{
	signed __int16 v1; // ax
	unsigned __int8 v2; // dl
	unsigned __int8 v3; // ch
	char v4; // ah
	char v5; // dl
	int v6; // eax
	int v7; // edx
	signed __int16 v9; // [esp+0h] [ebp-4h]

	if (a1x->levelPar3_0x46_70 >= 11)
	{
		v1 = GetAnimationPhase_27430(a1x->animationFrame_0x5C_92);
		v2 = a1x->animationFrame_0x5C_92;
		v9 = v1;
		if (v2 && v2 < 0x10u)
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 48);
		if (a1x->param3_0x36_54 & 1)
		{
			v3 = v9 + a1x->animationFrame_0x5C_92;
			a1x->animationFrame_0x5C_92 = v3;
			if (v3 > 0x64u)
			{
				v4 = a1x->param3_0x36_54;
				a1x->animationFrame_0x5C_92 = 100;
				a1x->param3_0x36_54 = v4 & 0xFE;
			}
		}
		else if (a1x->animationFrame_0x5C_92 > v9)
		{
			a1x->animationFrame_0x5C_92 -= v9;
		}
		else
		{
			v5 = a1x->param3_0x36_54 | 1;
			a1x->param3_0x36_54 = v5;
			a1x->animationFrame_0x5C_92 = 0;
			a1x->param3_0x36_54 = v5 ^ 2;
		}
	}
	a1x->subSpellIndex_0x2A_42 += a1x->param2_0x2C_44;
	LOBYTE(v6) = a1x->instanceIndex_0x3E_62;
	a1x->subSpellIndex_0x2A_42 &= 0x7ff;
	if (!(v6 & 3))
	{
		v6 = abs(a1x->param2_0x2C_44) - 5;
		if ((signed __int16)v6 < 11)
			LOWORD(v6) = 11;
		if (a1x->param2_0x2C_44 <= 0)
			v7 = -(signed __int16)v6;
		else
			LOWORD(v7) = v6;
		a1x->param2_0x2C_44 = v7;
	}
	//return v6;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (000273C0) --------------------------------------------------------
int GetSegmentAngle_273C0(__int16 a1, char a2, __int16 a3, __int16 a4)//2083c0
{
	__int16 v4; // ax
	int result; // eax
	__int16 v6; // ax

	v4 = abs(a3);
	result = ((15 - a4) * v4 + v4 * a1) & 0x7FF;
	if (a3 >= 0)
	{
		if (a2 & 2)
			return result;
		v6 = 2048 - result;
	}
	else if (a2 & 2)
	{
		v6 = 1024 - result;
	}
	else
	{
		v6 = result + 1024;
	}
	return v6 & 0x7FF;
}

//----- (00027430) --------------------------------------------------------
int GetAnimationPhase_27430(int a1)//208430
{
	int v1; // eax

	HIWORD(v1) = HIWORD(a1);
	if ((signed __int16)a1 >= 96)
		return 2;
	if ((signed __int16)a1 >= 87)
		return 3;
	if ((signed __int16)a1 >= 60)
		return 4;
	LOBYTE(v1) = (signed __int16)a1 < 30;
	BYTE1(v1) = 0;
	return v1 + 5;
}

//----- (00027470) --------------------------------------------------------
type_entity_0x6E8E* GetChainSegment_27470(type_entity_0x6E8E* a1x, __int16 a2)//208470
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	if (a2)
	{
		while (1)
		{
			resultx = Entities_EA3E4[resultx->nextLinkedId_0x34_52];
			if (resultx == Entities_EA3E4[0])
				break;
			if (resultx->levelPar3_0x46_70 == a2)
				return resultx;
		}
		resultx = 0;
	}
	return resultx;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000274C0) --------------------------------------------------------
void AppendChainSegment_274C0(type_entity_0x6E8E* event1, type_entity_0x6E8E* event2, type_entity_0x6E8E* event3, int16_t a4)//2084c0
{
	*event2 = *event3;
	event2->leaderId_0x32_50 = event3 - world_D41A0.entities_0x6E8E;
	event3->nextLinkedId_0x34_52 = event2 - world_D41A0.entities_0x6E8E;
	event2->nextLinkedId_0x34_52 = 0;
	event2->instanceIndex_0x3E_62 = abs((x_BYTE)a4) & 1;
	event2->flags_0xc_12_15.byte[0] &= 0xFBu;
	event2->levelPar3_0x46_70 = a4;
	event2->actionIndex_0x45_69 = 180;
	event2->param2_0x2C_44 = 0;
	event2->playerEntityIndex_0x94_148 = 0;
	event2->mana_0x90_144 = 0;
	event2->targetId_0x96_150 = event1 - world_D41A0.entities_0x6E8E;
	predictedAxis_EB398ar = event1->position_0x4C_76;
	AddEventToMap_57D70(event2, &predictedAxis_EB398ar);
	CopyMaxLifeToLife_49A20(event2);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00027590) --------------------------------------------------------
void SetFlyingManaSprite_27590(/*signed int a1, */type_entity_0x6E8E* a2x)//208590
{
	__int16 v2; // ax
	__int16 v3; // di
	__int16 v4; // ax
	int result; // eax
	__int16 v6; // ax
	type_entity_0x6E8E* v7x; // ebx

	v2 = GetManaSphereColorIndexFromEntityId_369F0(/*a1, */a2x->playerEntityIndex_0x94_148);
	v3 = v2;
	v4 = GetManaSpriteIndex_278F0(v2, a2x->levelPar3_0x46_70, 0);
	SetEntitySprite_49D50(a2x, v4);
	for (result = a2x->nextLinkedId_0x34_52; ; result = v7x->nextLinkedId_0x34_52)
	{
		v7x = Entities_EA3E4[result];
		if (v7x == Entities_EA3E4[0])
			break;
		v6 = GetManaSpriteIndex_278F0(v3, a2x->levelPar3_0x46_70, v7x->levelPar3_0x46_70);
		SetEntitySprite_49D50(v7x, v6);
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00027610) --------------------------------------------------------
void SetFlyingManaChainSize_27610(/*signed int a1, */type_entity_0x6E8E* a2x)//208610
{
	__int16 v2; // di
	__int32 v3; // rtt
	int result; // eax
	__int32 v5; // rtt
	type_entity_0x6E8E* v6x; // ebx

	v2 = GetManaSphereColorIndexFromEntityId_369F0(/*a1, */a2x->playerEntityIndex_0x94_148);
	//v3 = 550 * (unsigned __int16)x_WORD_D951C[4 + 7 * (signed __int16)GetManaSpriteIndex_278F0(v2, a2x->byte_0x46_70, 0)];
	v3 = 550 * spriteDefsTable_D951C[(signed __int16)GetManaSpriteIndex_278F0(v2, a2x->levelPar3_0x46_70, 0)].worldSize_8;
	SetEntityShiftRot_49EA0(a2x, v3 / 1000, v3 / 1000);
	for (result = a2x->nextLinkedId_0x34_52; ; result = v6x->nextLinkedId_0x34_52)
	{
		v6x = Entities_EA3E4[result];
		if (v6x == Entities_EA3E4[0])
			break;
		//v5 = 550 * (unsigned __int16)x_WORD_D951C[4 + 7 * (signed __int16)GetManaSpriteIndex_278F0(v2, a2x->byte_0x46_70, v6x->byte_0x46_70)];
		v5 = 550 * spriteDefsTable_D951C[(signed __int16)GetManaSpriteIndex_278F0(v2, a2x->levelPar3_0x46_70, v6x->levelPar3_0x46_70)].worldSize_8;
		SetEntityShiftRot_49EA0(v6x, v5 / 1000, v5 / 1000);
	}
	//return result;
}
// D9524: using guessed type __int16 x_WORD_D9524[];
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000276E0) --------------------------------------------------------
void UpdateChainSegments_276E0(type_entity_0x6E8E* a1x)//2086e0
{
	//signed int result; // eax
	int i; // ebx
	type_entity_0x6E8E* v3x; // ebx

	//result = a1;
	for (i = a1x->nextLinkedId_0x34_52; ; i = v3x->nextLinkedId_0x34_52)
	{
		v3x = Entities_EA3E4[i];
		if (v3x == Entities_EA3E4[0])
			break;
		/*result = */FlyingManaSteerToTarget_271D0(v3x);
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00027720) --------------------------------------------------------
void SetFlyingManaLevel_27720(type_entity_0x6E8E* a1x, signed __int16 a2)//208720
{
	type_entity_0x6E8E* ix; // ebx
	signed __int16 v3; // di
	type_entity_0x6E8E* v4x; // edi
	__int16 v5; // ST0C_2
	type_entity_0x6E8E* v6x; // ebx
	type_entity_0x6E8E* v7x; // eax
	type_entity_0x6E8E* v8x; // ST10_4
	type_entity_0x6E8E* v9x; // [esp+4h] [ebp-Ch]
	//int v10; // [esp+8h] [ebp-8h]
	char v11; // [esp+Ch] [ebp-4h]

	LOBYTE(a2) = a2 | 1;
	v11 = 0;
	if (a2 >= 1 && a2 <= 15 && a1x->levelPar3_0x46_70 != a2)
	{
		for (ix = a1x; ; ix = Entities_EA3E4[ix->nextLinkedId_0x34_52])
		{
			v3 = ix->nextLinkedId_0x34_52;
			if (!v3)
				break;
		}
		if (a1x->levelPar3_0x46_70 >= a2)
		{
			//v10 = (a1x->byte_0x46_70 - a2) / 2;
			while (v3 < (a1x->levelPar3_0x46_70 - a2) / 2)
			{
				v6x = Entities_EA3E4[ix->leaderId_0x32_50];
				v7x = v6x;
				ix = Entities_EA3E4[v6x->leaderId_0x32_50];
				ix->nextLinkedId_0x34_52 = 0;
				v8x = Entities_EA3E4[v7x->nextLinkedId_0x34_52];
				DisableEntityDrawing04_57F10(v7x);
				v3++;
				DisableEntityDrawing04_57F10(v8x);
			}
		}
		else
		{
			v4x = NewEvent_4A050();
			if (v4x)
			{
				v9x = NewEvent_4A050();
				if (v9x)
				{
					AppendChainSegment_274C0(a1x, v4x, ix, abs(ix->levelPar3_0x46_70) + 1);
					v5 = -(abs(ix->levelPar3_0x46_70) + 1);
					ix = v9x;
					AppendChainSegment_274C0(a1x, v9x, v4x, v5);
				}
				else
				{
					DisableEntityDrawing04_57F10(v4x);
					v11 = 1;
				}
			}
			else
			{
				v11 = 1;
			}
		}
		if (!v11)
		{
			a1x->levelPar3_0x46_70 = a2;
			SetFlyingManaSprite_27590(a1x);
			SetFlyingManaChainSize_27610(a1x);
		}
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00027880) --------------------------------------------------------
void FlyingManaCountdown_27880(type_entity_0x6E8E* a1x)//208880
{
	__int16 v1; // dx
	char v2; // ah
	signed int v3; // edx

	v1 = a1x->targetId_0x96_150;
	if (v1)
	{
		a1x->targetId_0x96_150 = v1 - 1;
	}
	else
	{
		v2 = a1x->levelPar3_0x46_70;
		a1x->targetId_0x96_150 = 1024;
		if (v2 <= 13)
			SetFlyingManaLevel_27720(a1x, v2 + 2);
		v3 = a1x->mana_0x90_144;
		if (v3 < 50000)
			a1x->mana_0x90_144 = v3 + 1000;
	}
}

//----- (000278F0) --------------------------------------------------------
int GetManaSpriteIndex_278F0(int a1, __int16 a2, __int16 a3)//2088f0
{
	int v3; // eax

	v3 = (signed __int16)abs(a3);
	LOWORD(v3) = (unsigned __int8)manaSpriteTable_D400C[(signed __int16)(a2 >> 1)][(signed __int16)v3];
	return a1 + v3;
}

//----- (00027930) --------------------------------------------------------
void FlyingManaScripted_27930(type_entity_0x6E8E* event)//208930
{
	CreatureRunScript_1D5D0(event, 176);
}

//----- (00027950) --------------------------------------------------------
void MoonDwellerIdle_27950(type_entity_0x6E8E* event)//208950
{
	type_entity_0x6E8E* ix; // ecx
	int v2; // ecx
	signed __int16 v3; // cx
	unsigned __int8 v4; // cl
	int v5y;
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // eax
	//int v7; // eax
	__int16 v8; // ax
	axis_3d* v9x; // edi
	char v11; // [esp-Ch] [ebp-10h]
	char v12; // [esp-8h] [ebp-Ch]
	int v13; // [esp-4h] [ebp-8h]

	//fix
	v5y = 0;
	//fix

	MoveCreature_1B8C0(event);
	for (ix = gameState_D41A4.creatureLists_38403x[event->model_0x40_64]; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->id_0x1A_26 != event->id_0x1A_26
			&& abs((int16_t)event->position_0x4C_76.x - (int16_t)ix->position_0x4C_76.x) < event->params_0x52_82.pitch
			&& abs((int16_t)event->position_0x4C_76.y - (int16_t)ix->position_0x4C_76.y) < event->params_0x52_82.pitch)
		{
			event->roll_0x20_32 = Maths::GetYawAngle_581E0(&ix->position_0x4C_76, &event->position_0x4C_76);
			break;
		}
	}
	v2 = event->position_0x4C_76.z - event->param2_0x2C_44;
	if (abs(v2) >= 256)
	{
		if (v2 <= 0)
			v3 = 32;
		else
			v3 = -32;
		event->position_0x4C_76.z += v3;
	}
	v4 = event->levelPar3_0x46_70;
	if (v4 < 1u)
	{
		if (v4) {
			MoonDwellerCheckDamage_28110(event);
			return;
		}
		event->actSpeed_0x82_130 = event->maxSpeed_0x86_134;
		v5y = event->param_0x10_16;
		event->param_0x10_16 = v5y - 1;
		if (v5y)
		{
			MoonDwellerCheckDamage_28110(event);
			return;
		}
	LABEL_27:
		v13 = v5y;
		v12 = 1;
		v11 = -72;
		goto LABEL_28;
	}
	if (v4 <= 1u)
	{
		v6x = FindNearestCastle_28000(event);
		if (v6x)
		{
			event->param2_0x2C_44 = 0x2000;
			event->targetId_0x96_150 = v6x - world_D41A0.entities_0x6E8E;
			SetEntityActionState_27FE0(event, 184, 2, 0);
		}
		else
		{
			v8 = GetTerrainAlt_10C40(&event->position_0x4C_76);
			HIBYTE(v8) += 7;
			event->param2_0x2C_44 = v8;
			SetEntityActionState_27FE0(event, 184, 0, 80);
		}
		MoonDwellerCheckDamage_28110(event);
		return;
	}
	if (v4 != 2)
	{
		MoonDwellerCheckDamage_28110(event);
		return;
	}
	v5x = GetBalloonTarget_28420(event);
	if (!v5x)
		goto LABEL_27;
	if (!(event->instanceIndex_0x3E_62 & 3))
	{
		v9x = &v5x->position_0x4C_76;
		event->roll_0x20_32 = Maths::GetYawAngle_581E0(&event->position_0x4C_76, &v5x->position_0x4C_76);
		if ((signed int)Maths::EuclideanDistXYZ_58490(&event->position_0x4C_76, v9x) < 768)
		{
			v13 = 500;
			v12 = 0;
			v11 = -71;
		LABEL_28:
			SetEntityActionState_27FE0(event, v11, v12, v13);
			MoonDwellerCheckDamage_28110(event);
			return;
		}
	}
	MoonDwellerCheckDamage_28110(event);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00027B20) --------------------------------------------------------
void MoonDwellerWander_27B20(type_entity_0x6E8E* a1x)//208b20
{
	char v1; // al
	int v2; // edx
	type_entity_0x6E8E* v3x; // esi
	__int16 v5; // dx
	char v6; // [esp+0h] [ebp-4h]

	v6 = 0;
	v1 = a1x->levelPar3_0x46_70;
	if (v1)
	{
		if (v1 == 1)
		{
			v5 = a1x->position_0x4C_76.z;
			if (v5 >= 0x2000)
			{
				SetEntityActionState_27FE0(a1x, 184, 0, 80);
				MoonDwellerCheckDamage_28110(a1x);
				return;
			}
			a1x->position_0x4C_76.z = v5 + 32;
		}
	}
	else
	{
		v2 = a1x->param_0x10_16;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
		a1x->param_0x10_16 = --v2;
		if (v2 && (v3x = GetBalloonTarget_28420(a1x)) != 0 && !IsTooCloseToSameModel_28060(a1x))
		{
			if (MoveTowardsEntity_28390(a1x, v3x))
				SetEntityActionState_27FE0(a1x, 187, 0, 0);
		}
		else
		{
			v6 = 1;
		}
		if (v6)
		{
			SetEntityActionState_27FE0(a1x, 185, 1, 0);
			MoonDwellerCheckDamage_28110(a1x);
			return;
		}
	}
	MoonDwellerCheckDamage_28110(a1x);
}

//----- (00027C10) --------------------------------------------------------
void MoonDwellerFollow_27C10(type_entity_0x6E8E* a1x)//208c10
{
	unsigned __int8 v1; // al
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // eax
	type_entity_0x6E8E* v4x; // esi
	type_entity_0x6E8E* v5x; // edi
	int v6; // edx
	type_entity_0x6E8E* v7x; // eax
	char v9; // [esp+0h] [ebp-8h]
	char v10; // [esp+4h] [ebp-4h]

	v10 = 0;
	v9 = 0;
	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 59);
	v1 = a1x->levelPar3_0x46_70;
	if (v1 < 1u)
	{
		if (v1)
			goto LABEL_24;
		v2x = GetBalloonTarget_28420(a1x);
		if (!v2x || v2x->flags_0xc_12_15.byte[0] & 0x40)
		{
			v9 = 1;
		}
		else
		{
			a1x->param2_0x2C_44 = 18;
			SetEntityActionState_27FE0(a1x, 187, 1, 64);
		}
	}
	else if (v1 > 1u)
	{
		if (v1 == 3)
		{
			a1x->targetId_0x96_150 = 0;
			v7x = FindNearestCastle_28000(a1x);
			if (v7x
				&& (a1x->targetId_0x96_150 = v7x - world_D41A0.entities_0x6E8E,
					(signed int)Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v7x->position_0x4C_76) <= 3584))
			{
				SetEntityActionState_27FE0(a1x, 185, 0, 500);
			}
			else
			{
				v10 = 1;
			}
		}
		goto LABEL_24;
	}
	if (a1x->targetId_0x96_150)
	{
		v3x = GetBalloonTarget_28420(a1x);
		v4x = v3x;
		v5x = v3x;
		if (v3x && (v6 = a1x->param_0x10_16 - 1, (a1x->param_0x10_16 = v6) != 0))
		{
			v3x->flags_0xc_12_15.byte[0] |= 0x40u;
			v3x->targetId_0x96_150 = a1x - world_D41A0.entities_0x6E8E;
			a1x->param2_0x2C_44 += 10;
			if (EntitiesOverlap_106C0(a1x, v3x) || v4x->position_0x4C_76.z > a1x->position_0x4C_76.z)
			{
				a1x->mana_0x90_144 += v5x->mana_0x90_144;
				DisableEntityDrawing04_57F10(v5x);
				v9 = 1;
			}
		}
		else
		{
			v9 = 1;
		}
	}
	else
	{
		v10 = 1;
	}
	if (v9)
		SetEntityActionState_27FE0(a1x, 187, 3, 0);
LABEL_24:
	if (v10)
		SetEntityActionState_27FE0(a1x, 185, 1, 0);
	MoonDwellerCheckDamage_28110(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00027E00) --------------------------------------------------------
void MoonDwellerAttack_27E00(type_entity_0x6E8E* a1x)//208e00
{
	type_entity_0x6E8E* v1x; // ebx
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // esi
	type_entity_0x6E8E* ix; // ecx
	//int v5; // eax
	unsigned int v6; // edi
	//char result; // al
	char v8; // [esp+4h] [ebp-8h]

	v1x = a1x;
	v8 = 0;
	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 59);
	v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
	MoveCreature_1B8C0(a1x);
	v2x = GetCreatureTarget_1ED30(v1x, Entities_EA3E4[v1x->targetId_0x96_150]);
	v3x = v2x;
	if (v2x <= Entities_EA3E4[0] || v2x->life_0x8 < 0 || v2x->flags_0xc_12_15.byte[1] & 4)
	{
	LABEL_15:
		v8 = 1;
		goto LABEL_16;
	}
	if (!(a1x->instanceIndex_0x3E_62 & 3))
	{
		a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v2x->position_0x4C_76);
		for (ix = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; ix > Entities_EA3E4[0]; ix = ix->next_0)
		{
			if (ix->id_0x1A_26 != a1x->id_0x1A_26
				&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)ix->position_0x4C_76.x) < a1x->params_0x52_82.pitch
				&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)ix->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
			{
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&ix->position_0x4C_76, &a1x->position_0x4C_76);
				break;
			}
		}
	}
	//v5 = a1x->dword_0xA0_160;
	if (!(a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 & a1x->instanceIndex_0x3E_62))
	{
		v6 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
		if (Distance3D_583F0(&a1x->position_0x4C_76, &v3x->position_0x4C_76) < v6)
		{
			CreatureShootProjectile9_1D260(a1x, v3x);
			goto LABEL_16;
		}
		goto LABEL_15;
	}
LABEL_16:
	/*result = */MoonDwellerCheckDamage_28110(a1x);
	if (v8)
	{
		a1x->targetId_0x96_150 = 0;
		/*result = */SetEntityActionState_27FE0(a1x, 187, 3, 0);
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00027FA0) --------------------------------------------------------
void MoonDwellerPreKill_27FA0(type_entity_0x6E8E* a1x)//208fa0
{
	PreKillEntity_1C890(a1x, 184);
}

//----- (00027FC0) --------------------------------------------------------
void MoonDwellerKill_27FC0(type_entity_0x6E8E* a1x)//208fc0
{
	KillEntity_1C930(a1x);
}

//----- (00028000) --------------------------------------------------------
type_entity_0x6E8E* FindNearestCastle_28000(type_entity_0x6E8E* a1x)//209000
{
	unsigned int v1; // edi
	type_entity_0x6E8E* v2x; // esi
	type_entity_0x6E8E* v3x; // ebx
	unsigned int v5; // eax

	v1 = -1;
	v2x = 0;
	v3x = gameState_D41A4.manaSphereList_38523;
	if (v3x <= Entities_EA3E4[0])
		return 0;
	do
	{
		if (v3x->model_0x40_64 == 39)
		{
			v5 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v3x->position_0x4C_76);
			if (v5 < v1)
			{
				v2x = v3x;
				v1 = v5;
			}
		}
		v3x = v3x->next_0;
	} while (v3x > Entities_EA3E4[0]);
	return v2x;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00028060) --------------------------------------------------------
char IsTooCloseToSameModel_28060(type_entity_0x6E8E* a1x)//209060
{
	int v1; // esi
	type_entity_0x6E8E* ix; // ecx
	__int16 v3; // ax
	char v5; // [esp+4h] [ebp-4h]

	v5 = 0;
	v1 = 2 * a1x->params_0x52_82.pitch;
	for (ix = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		if (ix->id_0x1A_26 != a1x->id_0x1A_26
			&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)ix->position_0x4C_76.x) < v1
			&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)ix->position_0x4C_76.y) < v1
			&& abs(a1x->position_0x4C_76.z - ix->position_0x4C_76.z) < 2 * a1x->params_0x52_82.fov)
		{
			v3 = a1x->position_0x4C_76.z;
			if (v3 >= ix->position_0x4C_76.z)
			{
				v5 = 1;
				a1x->position_0x4C_76.z = v3 + 16;
				return v5;
			}
		}
	}
	return v5;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00028110) --------------------------------------------------------
void MoonDwellerCheckDamage_28110(type_entity_0x6E8E* a1x)//209110
{
	signed int v1; // esi
	__int16 v2; // ax
	type_entity_0x6E8E* v4x; // eax
	//type_str_0x6E8E* v5x; // ecx
	char v6; // al
	type_entity_0x6E8E* v7x; // eax
	//type_str_0x6E8E* v8x; // edi
	unsigned int v9; // esi
	type_entity_0x6E8E* v10x; // eax
	char v11; // dh

	v1 = 0;
	if (a1x->timer_0x39_57)
	{
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			v2 = a1x->mail_0x5E_94.channel[0].source;
			a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
			a1x->hitById_0x26_38 = v2;
			//a1x->hitById_0x26_38;
			a1x->mail_0x5E_94.channel[0].source = 0;
			if (Entities_EA3E4[a1x->hitById_0x26_38]->class_0x3F_63 == 3)//209148 mov ax,[ebx+26h]; 209159 cmp byte ptr [eax+3Fh],3
			{
				v1 = 1;
				v4x = GetBalloonTarget_28420(a1x);
				//v5x = v4x;
				if (v4x && v4x->flags_0xc_12_15.byte[0] & 0x40 && v4x->targetId_0x96_150 == a1x - world_D41A0.entities_0x6E8E)
				{
					v6 = v4x->flags_0xc_12_15.byte[0];
					v4x->targetId_0x96_150 = 0;
					v4x->flags_0xc_12_15.byte[0] = v6 & 0xBF;
				}
				a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
				SetEntityActionState_27FE0(a1x, 186, 0, 0);
			}
		}
		else
		{
			a1x->hitById_0x26_38 = 0;
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v1 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		SetEntityActionState_27FE0(a1x, 188, 0, 0);
	}
	if (!v1 && !(a1x->instanceIndex_0x3E_62 & 0x1F))
	{
		v7x = FindBalloonTarget_282D0(a1x);
		//v8 = v7;
		if (v7x)
		{
			v9 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v7x->position_0x4C_76) < v9)
			{
				v10x = GetBalloonTarget_28420(a1x);
				if (v10x
					&& v10x->flags_0xc_12_15.byte[0] & 0x40
					&& v10x->targetId_0x96_150 == a1x - world_D41A0.entities_0x6E8E)
				{
					v11 = v10x->flags_0xc_12_15.byte[0];
					v10x->targetId_0x96_150 = 0;
					v10x->flags_0xc_12_15.byte[0] = v11 & 0xBF;
				}
				a1x->targetId_0x96_150 = v7x - world_D41A0.entities_0x6E8E;
				SetEntityActionState_27FE0(a1x, 186, 0, 0);
			}
		}
	}
	//return 0;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000282D0) --------------------------------------------------------
type_entity_0x6E8E* FindBalloonTarget_282D0(type_entity_0x6E8E* a1x)//2092d0
{
	unsigned int v1; // esi
	type_entity_0x6E8E* v2x; // edi
	type_entity_0x6E8E* ix; // ebx
	unsigned __int8 v4; // al
	type_str_164* v5x; // eax
	type_entity_0x6E8E* v6x; // eax
	type_entity_0x6E8E* v7x; // eax
	unsigned int v8; // eax

	v1 = -1;
	v2x = 0;
	for (ix = gameState_D41A4.class3List_38519; ix > Entities_EA3E4[0]; ix = ix->next_0)
	{
		v4 = ix->model_0x40_64;
		if (v4 <= 1u)
		{
			v5x = ix->playerData_0xA4_164x;
		}
		else
		{
			if (v4 != 3)
				continue;
			if (ix->mana_0x90_144 <= 0)
				continue;
			v6x = Entities_EA3E4[ix->id_0x1A_26];
			if (!v6x)
				continue;
			v5x = v6x->playerData_0xA4_164x;
		}
		v7x = Entities_EA3E4[v5x->CastleEntityIndex_0x3A_58];
		if (v7x)
		{
			if (!CompareAxisWithShift_10750(ix, v7x))
			{
				v8 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &ix->position_0x4C_76);
				if (v8 < v1)
				{
					v1 = v8;
					v2x = ix;
				}
			}
		}
	}
	return v2x;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00028420) --------------------------------------------------------
type_entity_0x6E8E* GetBalloonTarget_28420(type_entity_0x6E8E* a1x)//209420
{
	type_entity_0x6E8E* resulty = 0; // eax

	int resultx = 0;

	resultx = 0;
	if (a1x->targetId_0x96_150)
	{
		LOWORD(resultx) = a1x->targetId_0x96_150;
		resulty = Entities_EA3E4[resultx];
		if (resulty)
		{
			if (resulty->life_0x8 < 0
				|| resulty->flags_0xc_12_15.byte[1] & 4
				|| resulty->class_0x3F_63 != 10
				|| resulty->model_0x40_64 != 39)
			{
				resulty = 0;
			}
		}
	}
	return resulty;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00028470) --------------------------------------------------------
void MoonDwellerScripted_28470(type_entity_0x6E8E* a1x)//209470
{
	CreatureRunScript_1D5D0(a1x, 184);
}

//----- (00028490) --------------------------------------------------------
void Creature24Idle_28490(type_entity_0x6E8E* a1x)//209490
{
	CreatureIdle_1BD90(a1x, 192);
	if (a1x->actionIndex_0x45_69 == 192)
	{
		if (!(a1x->instanceIndex_0x3E_62 & 7))
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			if (!(a1x->rand_0x14_20 % 3u))
				a1x->actionIndex_0x45_69 = 193;
		}
		if (a1x->actionIndex_0x45_69 == 192)
			Creature24FindTarget_28690(a1x);
	}
	else
	{
		a1x->actionIndex_0x45_69 = 198;
	}
	Creature24SetSprite_287B0(a1x);
}

//----- (00028500) --------------------------------------------------------
void Creature24Wander_28500(type_entity_0x6E8E* a1x)//209500
{
	CreatureWander_1BF90(a1x, 192);
	if (a1x->actionIndex_0x45_69 == 193)
	{
		if (!(a1x->instanceIndex_0x3E_62 & 7))
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			if (!(a1x->rand_0x14_20 % 3u))
				a1x->actionIndex_0x45_69 = 192;
		}
		if (a1x->actionIndex_0x45_69 == 193)
			Creature24FindTarget_28690(a1x);
	}
	else
	{
		a1x->actionIndex_0x45_69 = 198;
	}
	Creature24SetSprite_287B0(a1x);
}

//----- (00028570) --------------------------------------------------------
void Creature24Attack_28570(type_entity_0x6E8E* a1x)//209570
{
	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 7);
	if ((unsigned __int16)CreatureAttack_1C310(a1x, 192, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureMeleeAttack1536_1CF20))
		a1x->actionIndex_0x45_69 = 198;
	Creature24SetSprite_287B0(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (000285D0) --------------------------------------------------------
void Creature24Follow_285D0(type_entity_0x6E8E* a1x)//2095d0
{
	a1x->actionIndex_0x45_69 = 193;
	CreatureIdle_1BD90(a1x, 192);
}

//----- (000285F0) --------------------------------------------------------
void Creature24PreKill_285F0(type_entity_0x6E8E* a1x)//2095f0
{
	PreKillEntity_1C890(a1x, 192);
}

//----- (00028610) --------------------------------------------------------
void Creature24Kill_28610(type_entity_0x6E8E* a1x)//209610
{
	KillEntity_1C930(a1x);
}

//----- (00028630) --------------------------------------------------------
void Creature24Hit_28630(type_entity_0x6E8E* a1x)//209630
{
	CreatureFlee_1C980(a1x, 192);
	Creature24FindTarget_28690(a1x);
	Creature24SetSprite_287B0(a1x);
}

//----- (00028660) --------------------------------------------------------
void Creature24Scripted_28660(type_entity_0x6E8E* a1x)//209660
{
	CreatureRunScript_1D5D0(a1x, 192);
	Creature24SetSprite_287B0(a1x);
}

//----- (00028690) --------------------------------------------------------
void Creature24FindTarget_28690(type_entity_0x6E8E* a1x)//209690
{
	int v1; // eax
	type_entity_0x6E8E* v2x; // esi
	int v3; // eax
	unsigned int v4; // edi
	signed int v5; // ST0C_4
	__int16 v6; // ax
	type_entity_0x6E8E* v7x; // eax
	//int v8; // eax
	unsigned int v9; // [esp+0h] [ebp-10h]
	type_entity_0x6E8E* v10x; // [esp+8h] [ebp-8h]
	unsigned int v11; // [esp+Ch] [ebp-4h]

	if (a1x->timer_0x39_57 && !(a1x->instanceIndex_0x3E_62 & 0xF))
	{
		v1 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
		v11 = -1;
		v10x = 0;
		v2x = gameState_D41A4.class3List_38519;
		v9 = v1 * v1;
		while (v2x > Entities_EA3E4[0])
		{
			v3 = (signed __int16)(v2x->position_0x4C_76.x - a1x->position_0x4C_76.x);
			v4 = (signed __int16)(v2x->position_0x4C_76.y - a1x->position_0x4C_76.y)
				* (signed __int16)(v2x->position_0x4C_76.y - a1x->position_0x4C_76.y)
				+ v3 * v3;
			if (v4 <= v9 && !(v2x->flags_0xc_12_15.byte[0] & 0x20))
			{
				v5 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
				v6 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v2x->position_0x4C_76);
				if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v6) < v5 && v4 < v11)
				{
					v10x = v2x;
					v11 = v4;
				}
			}
			v2x = v2x->next_0;
		}
		if (v10x)
		{
			v7x = GetCreatureTarget_1ED30(a1x, v10x);
			if (v7x > Entities_EA3E4[0] && v7x->life_0x8 >= 0 && !(v7x->flags_0xc_12_15.byte[1] & 4))
			{
				a1x->actionIndex_0x45_69 = 194;
				a1x->targetId_0x96_150 = v7x - world_D41A0.entities_0x6E8E;
			}
		}
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000287B0) --------------------------------------------------------
void Creature24SetSprite_287B0(type_entity_0x6E8E* a1x)//2097b0
{
	//uint8_t* result; // eax
	unsigned __int8 v2; // dl
	signed int v3; // edx
	__int16 v4; // dx

	//result = a1;
	v2 = a1x->actionIndex_0x45_69;
	if (v2 < 0xC2u)
	{
		if (v2 == 0xc0u)
		{
			v3 = 336;
			a1x->actSpeed_0x82_130 = 0;
			goto LABEL_11;
		}
		goto LABEL_9;
	}
	if (v2 <= 0xC2u)
	{
		v4 = a1x->minSpeed_0x84_132;
	}
	else
	{
		if (v2 != 0xc6u)
		{
		LABEL_9:
			v4 = a1x->maxSpeed_0x86_134;
			goto LABEL_10;
		}
		v4 = 2 * a1x->maxSpeed_0x86_134;
	}
LABEL_10:
	a1x->actSpeed_0x82_130 = v4;
	v3 = 335;
LABEL_11:
	if (v3 != a1x->spriteIndex_0x5A_90)
	{
		a1x->spriteIndex_0x5A_90 = v3;
		a1x->animationFrame_0x5C_92 = 0;
		//a1x->byte_0x5D_93 = spriteKindFrames_D8A2E[(unsigned __int8)x_WORD_D951C[6 + 7 * v3]];
		a1x->frameCount_0x5D_93 = spriteKindFrames_D8A2E[spriteDefsTable_D951C[v3].kind_12];

	}
	//return result;
}

//----- (00028860) --------------------------------------------------------
__int16 CymmerianIdle_28860(type_entity_0x6E8E* a1x)//209860 //tex 392,393
{
	char v1; // dh
	uint32_t v2; // eax
	__int16 v3; // ax
	int i; // edx
	int v5; // ecx
	type_entity_0x6E8E* v6x; // edx
	__int16 v7; // si
	int v8; // eax
	int v9; // ecx
	int v10; // ecx
	__int16 v11; // ax
	char v12; // cl
	type_entity_0x6E8E* v13x; // eax
	char v14; // dh
	__int16 v15; // ax
	unsigned int v16; // edx
	int v17; // eax
	unsigned __int16 v18; // di
	type_entity_0x6E8E* v19x; // esi
	unsigned __int16 v20; // dx
	type_entity_0x6E8E* v21x; // esi
	int v22; // ecx
	__int64 v23; // rtt
	char v24; // ch
	char v26; // [esp+0h] [ebp-4h]

	v26 = 0;
	v1 = a1x->levelPar3_0x46_70;
	v2 = 0;
	if (v1 != 1 && v1 != 2)
	{
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
			v3 = a1x->mail_0x5E_94.channel[0].source;
			a1x->mail_0x5E_94.channel[0].source = 0;
			a1x->hitById_0x26_38 = v3;
			v2 = 1;
		}
		else
		{
			a1x->hitById_0x26_38 = 0;
		}
		if (a1x->nextLinkedId_0x34_52)
		{
			for (i = a1x->nextLinkedId_0x34_52; ; i = v6x->nextLinkedId_0x34_52)
			{
				v6x = Entities_EA3E4[i];
				if (v6x == Entities_EA3E4[0])
					break;
				v5 = v6x->life_0x8;
				if (v5 < a1x->life_0x8)
				{
					a1x->life_0x8 = v5;
					a1x->hitById_0x26_38 = v6x->hitById_0x26_38;
					v2 = 1;
					break;
				}
			}
		}
		if (a1x->life_0x8 < 0)
		{
			a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
			v2 = 2;
		}
	}
	v7 = a1x->subSpellIndex_0x2A_42 - 1;
	a1x->subSpellIndex_0x2A_42 = v7;
	if (!v7)
		v2 = 2;
	if (v2 == 2)
	{
		a1x->actionIndex_0x45_69 = 204;
	}
	else
	{
		switch (a1x->levelPar3_0x46_70)
		{
		case 1:
			a1x->param_0x10_16 = 52;
			a1x->levelPar3_0x46_70 = 2;
			goto LABEL_20;
		case 2:
		LABEL_20:
			v8 = a1x->maxLife_0x4;
			v9 = a1x->param_0x10_16;
			a1x->mail_0x5E_94.channel[0].source = 0;
			v10 = v9 - 1;
			a1x->life_0x8 = v8;
			a1x->param_0x10_16 = v10;
			if (v10 < 0)
				goto LABEL_21;
			if (v10 > 13)
			{
				v11 = a1x->roll_0x20_32;
				HIBYTE(v11) = (HIBYTE(v11) + 1) & 7;
				a1x->roll_0x20_32 = v11;
			}
			break;
		case 3:
			v12 = 0;
			if (!a1x->ownerPlayerId_0x24_36
				|| (v13x = Entities_EA3E4[a1x->ownerPlayerId_0x24_36], v13x->class_0x3F_63 != 3)
				|| (v14 = v13x->model_0x40_64) != 0 && v14 != 1)
			{
				v12 = 1;
			}
			else if (v13x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
			{
				v15 = a1x->ownerPlayerId_0x24_36;
				a1x->levelPar3_0x46_70 = 5;
				a1x->targetId_0x96_150 = v15;
			}
			else
			{
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v16 = a1x->rand_0x14_20 % 0x64u;
				a1x->levelPar3_0x46_70 = 4;
				a1x->param_0x10_16 = v16 + 100;
			}
			if (v12)
			{
				a1x->levelPar3_0x46_70 = 8;
				a1x->param_0x10_16 = 100;
			}
			break;
		case 4:
			v17 = a1x->param_0x10_16 - 1;
			a1x->param_0x10_16 = v17;
			if (v17 < 0)
				goto LABEL_35;
			break;
		case 5:
			v18 = Entities_EA3E4[a1x->targetId_0x96_150]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
			if (v18)
			{
				if (!(a1x->instanceIndex_0x3E_62 & 7))
				{
					v19x = Entities_EA3E4[v18];
					a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v19x->position_0x4C_76);
					if (CompareAxisWithShift_10750(a1x, v19x))
						a1x->levelPar3_0x46_70 = 6;
				}
			}
			else
			{
			LABEL_35:
				a1x->levelPar3_0x46_70 = 3;
			}
			break;
		case 6:
			a1x->levelPar3_0x46_70 = 7;
			v26 = 1;
			goto LABEL_41;
		case 7:
		LABEL_41:
			v20 = Entities_EA3E4[a1x->targetId_0x96_150]->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
			if (v20)
			{
				v21x = Entities_EA3E4[v20];
				if (CompareAxisWithShift_10750(a1x, Entities_EA3E4[v20]))
				{
					SendMailToEntity_11900(a1x, v21x, 0, 0x3Cu);
				}
				else
				{
					a1x->levelPar3_0x46_70 = 5;
					v26 = 1;
				}
			}
			else
			{
			LABEL_21:
				a1x->levelPar3_0x46_70 = 3;
				v26 = 1;
			}
			break;
		case 8:
			v22 = a1x->param_0x10_16 - 1;
			a1x->param_0x10_16 = v22;
			if (v22 < 0)
				a1x->actionIndex_0x45_69 = 204;
			break;
		default:
			break;
		}
		if (!(a1x->instanceIndex_0x3E_62 & 7))
		{
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v23 = a1x->rand_0x14_20 % 0x9Du;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			//a1x->roll_0x20_32 += (2 * (unsigned __int64)(v23 / 79) - 1) * (a1x->word_0x14_20 % 0x17Du);
			a1x->roll_0x20_32 += (2 * (v23 / 79) - 1) * (a1x->rand_0x14_20 % 0x17Du);
			a1x->roll_0x20_32 &= 0x7ff;
		}
		MoveCreature_1B8C0(a1x);
		v2 = TerrainTileIsWater_104D0(&a1x->position_0x4C_76);
		if (v2 == 1)
		{
			if (a1x->spriteIndex_0x5A_90 == 314)
			{
				LOWORD(v2) = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
				if ((signed __int16)v2 < a1x->position_0x4C_76.z)
					/*LOWORD(v2) = */SetEntityIndexAndRot_49CD0(a1x, 313);
			}
			else
			{
				/*LOWORD(v2) = */SetEntityIndexAndRot_49CD0(a1x, 314);
				a1x->minSpeed_0x84_132 = 35;
				v26 = 1;
			}
		}
		else if (a1x->spriteIndex_0x5A_90 != 313)
		{
			/*LOWORD(v2) = */SetEntityIndexAndRot_49CD0(a1x, 313);
			a1x->minSpeed_0x84_132 = 60;
			v26 = 1;
		}
		if (v26)
		{
			LOWORD(v2) = a1x->minSpeed_0x84_132;
			v24 = a1x->levelPar3_0x46_70;
			a1x->actSpeed_0x82_130 = (x_WORD)v2;
			if (v24 == 2)
				a1x->actSpeed_0x82_130 = (x_WORD)v2 + 50;
		}
	}
	return (signed __int16)v2;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00028C30) --------------------------------------------------------
int CymmerianWander_28C30(type_entity_0x6E8E* a1x)//209c30
{
	int result; // eax
	// fix if begin
	result = 0;
	// end

	CreatureWander_1BF90(a1x, 200);
	if (a1x->life_0x8 < 0)
		a1x->life_0x8 = 0;
	return result;
}

//----- (00028C60) --------------------------------------------------------
signed int CymmerianAttack_28C60(type_entity_0x6E8E* a1x)//209c60
{
	signed int result; // eax

	PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 37);
	result = CreatureAttack_1C310(a1x, 200, (unsigned __int16(*)(type_entity_0x6E8E*, type_entity_0x6E8E*))CreatureShootFireball_1CC20);
	if (a1x->life_0x8 < 0)
		a1x->life_0x8 = 0;
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00028CC0) --------------------------------------------------------
type_entity_0x6E8E* CymmerianFollow_28CC0(type_entity_0x6E8E* a1x)//209cc0
{
	type_entity_0x6E8E* resultx; // eax
	int v2; // edx

	resultx = a1x;
	v2 = a1x->life_0x8;
	a1x->actionIndex_0x45_69 = 201;
	if (v2 < 0)
		a1x->life_0x8 = 0;
	return resultx;
}

//----- (00028CE0) --------------------------------------------------------
void CymmerianPreKill_28CE0(type_entity_0x6E8E* a1x)//209ce0 //tex 393
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	//__int16 v3; // dx
	//__int16 v4; // ax
	//__int16 v5; // ax
	//uint8_t* v6; // edx
	type_entity_0x6E8E* v7x; // eax
	//int result; // eax
	int v9; // [esp+0h] [ebp-10h]
	signed int i; // [esp+4h] [ebp-Ch]
	type_entity_0x6E8E* v11x; // [esp+8h] [ebp-8h]
	int v12; // [esp+Ch] [ebp-4h]

	if (a1x->levelPar3_0x46_70)
	{
		PreKillEntity_1C890(a1x, 200);
		return;
	}
	if (GetFreeEntityCount_4A810() <= 1)
	{
		TransformEntityToManaSphere_36BA0(a1x, false);
	}
	else
	{
		v12 = a1x->mana_0x90_144;
		v9 = v12 / 3;
		for (i = 0; i < 3; i++)
		{
			v1x = NewEvent_4A050();
			v2x = v1x;
			v11x = v1x;
			if (v1x)
			{
				v1x->actionIndex_0x45_69 = 200;
				v1x->class_0x3F_63 = 5;
				v1x->model_0x40_64 = 25;
				v1x->position_0x4C_76.x = a1x->position_0x4C_76.x;
				v1x->position_0x4C_76.y = a1x->position_0x4C_76.y;
				v1x->position_0x4C_76.z = a1x->position_0x4C_76.z;
				v1x->levelPar3_0x46_70 = 3;
				v1x->minSpeed_0x84_132 = 35;
				v1x->maxSpeed_0x86_134 = 60;
				v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132 + 50;
				//v3 = v1x->word_0x14_20;
				v1x->mana_0x90_144 = v9;
				v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
				//v4 = v1x->word_0x14_20;
				v2x->maxLife_0x4 = 80;
				v2x->subSpellIndex_0x2A_42 = 15000;
				//v5 = (v1x->word_0x14_20 & 0x7FF) - 1;
				v2x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
				v2x->yaw_0x1C_28 = v2x->roll_0x20_32;
				v2x->mailMask_0x38_56 = 1;
				//v6 = a1x->byte_0x40_64 + x_D41A0_BYTEARRAY_0;
				//LOBYTE(v5) = (*(x_BYTE *)(a1x->byte_0x40_64 + x_D41A0_BYTEARRAY_0 + 16))++;
				v2x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[a1x->model_0x40_64]++;
				v2x->typeParams_0xA0_160x = &str_D7BD6[95]; //(type_str_160*)&unk_D7BD6[0xc9e];
				v2x->timer_0x39_57 = 64;
				v12 -= v9;
				v2x->xtype_0x41_65 = 3;
				AddEventToMap_57D70(v2x, &a1x->position_0x4C_76);
				CopyMaxLifeToLife_49A20(v2x);
				SetEntityIndexAndRot_49CD0(v2x, 314);
				SetEntityShiftRot_49EA0(v2x, 32, 32);
				v2x->levelPar3_0x46_70 = 1;
				v2x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
			}
		}
		if (v11x)
			v11x->mana_0x90_144 += v12;
	}
	v7x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 1);
	if (v7x)
		v7x->id_0x1A_26 = a1x->id_0x1A_26;
	a1x->actionIndex_0x45_69 = 205;
	//result = (int)x_D41A0_BYTEARRAY_0;
	world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2 += 3;
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (00028EC0) --------------------------------------------------------
void CymmerianKill_28EC0(type_entity_0x6E8E* a1x)//209ec0
{
	if (a1x->levelPar3_0x46_70)
	{
		if (a1x->ownerPlayerId_0x24_36 == world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.creaturesKilledPercent_373++;//0x209EEC/0x209F24 inc dword ptr [edx+3139h]
		KillEntity_1C930(a1x);
	}
	else
	{
		if (a1x->ownerPlayerId_0x24_36 == world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240)
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.creaturesKilledPercent_373++;//0x209EEC/0x209F24 inc dword ptr [edx+3139h]
		a1x->life_0x8 = -1;
		DisableEntityDrawing04_57F10(a1x);
	}
}

//----- (00028F50) --------------------------------------------------------
void CymmerianScripted_28F50(type_entity_0x6E8E* a1x)//209f50
{
	if (a1x->levelPar3_0x46_70)
	{
		ClearEntityStage_12470(a1x, 200);
		a1x->levelPar3_0x46_70 = 3;
	}
	else
	{
		CreatureRunScript_1D5D0(a1x, 200);
	}
}

//----- (00028F90) --------------------------------------------------------
void MummyIdle_28F90(type_entity_0x6E8E* a1x)//209f90
{
	CreatureIdle_1BD90(a1x, 208);
	MummyResetAttackFlag_293B0(a1x);
}

//----- (00028FC0) --------------------------------------------------------
void MummyWander_28FC0(type_entity_0x6E8E* a1x)//209fc0
{
	CreatureWander_1BF90(a1x, 208);
	MummyResetAttackFlag_293B0(a1x);
}

//----- (00028FF0) --------------------------------------------------------
void MummyAttack_28FF0(type_entity_0x6E8E* a1x)//209ff0
{
	unsigned int v1; // esi
	__int16 v2; // ax
	int i; // ecx
	int v4; // eax
	type_entity_0x6E8E* v5x; // ecx
	type_entity_0x6E8E* v6x; // esi
	char v7; // dh
	type_entity_0x6E8E* jx; // ecx
	int v9; // edx
	signed int v10; // eax
	//int v11; // eax
	unsigned int v12; // edx
	//int v14; // edx
	int v15; // eax
	__int16 v16; // dx
	//int v17; // edx
	int v18; // eax
	__int16 v19; // cx

	if (!(a1x->instanceIndex_0x3E_62 & 0x1F))
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 62);
	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		v1 = 1;
		a1x->hitById_0x26_38 = v2;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				v1 = 1;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		v1 = 2;
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
	}
	if (v1 < 1)
	{
		if (v1)
		{
			MummySetAttackFlag_293D0(a1x);
			return;
		}
	}
	else
	{
		if (v1 > 1)
		{
			if (v1 == 2)
				a1x->actionIndex_0x45_69 = 212;
			MummySetAttackFlag_293D0(a1x);
			return;
		}
		a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
	}
	MoveCreature_1B8C0(a1x);
	v6x = Entities_EA3E4[a1x->targetId_0x96_150];
	if (v6x > Entities_EA3E4[0] && v6x->class_0x3F_63 == 3)
	{
		v7 = v6x->model_0x40_64;
		if ((!v7 || v7 == 1) && v6x->life_0x8 >= 0 && !(v6x->flags_0xc_12_15.byte[1] & 4))
		{
			if (!(a1x->instanceIndex_0x3E_62 & 3))
			{
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v6x->position_0x4C_76);
				for (jx = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64]; jx > Entities_EA3E4[0]; jx = jx->next_0)
				{
					if (jx->id_0x1A_26 != a1x->id_0x1A_26
						&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)jx->position_0x4C_76.x) < a1x->params_0x52_82.pitch
						&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)jx->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
					{
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&jx->position_0x4C_76, &a1x->position_0x4C_76);
						break;
					}
				}
			}
			v9 = v6x->mana_0x90_144 - (v6x->manaRegen_0x88_136 + 14);
			v6x->mana_0x90_144 = v9;
			if (v9 < 0)
				v6x->mana_0x90_144 = 0;
			if (a1x->instanceIndex_0x3E_62 & 3)
				return MummySetAttackFlag_293D0(a1x);
			v10 = Distance3D_583F0(&a1x->position_0x4C_76, &v6x->position_0x4C_76);
			if (v10 <= a1x->typeParams_0xA0_160x->range_160_0x1c_28)
			{
				if (v10 >= 2048)
					return MummySetAttackFlag_293D0(a1x);
				if (v6x->model_0x40_64)
					return MummySetAttackFlag_293D0(a1x);
				//v11 = (int)x_D41A0_BYTEARRAY_0;
				//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) + 9439;
				world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
				v12 = world_D41A0.rand_0x8 % 0x3Fu;
				if (v12 < 4)
					return MummySetAttackFlag_293D0(a1x);
				if (v12 <= 4)
				{
					//v17 = v6x->dword_0xA4_164;
					v18 = v6x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107;
					if (v18 == -1)
						return MummySetAttackFlag_293D0(a1x);
					v19 = v6x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v18];
					if (!v19)
						return MummySetAttackFlag_293D0(a1x);
					SpellHitByEntity_69300(Entities_EA3E4[v19], a1x);
				}
				else
				{
					if (v12 != 5)
						return MummySetAttackFlag_293D0(a1x);
					//v14 = v6x->dword_0xA4_164;
					v15 = v6x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105;
					if (v15 == -1)
						return MummySetAttackFlag_293D0(a1x);
					v16 = v6x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v15];
					if (!v16)
						return MummySetAttackFlag_293D0(a1x);
					SpellHitByEntity_69300(Entities_EA3E4[v16], a1x);
				}
				return MummySetAttackFlag_293D0(a1x);
			}
		}
	}
	a1x->actionIndex_0x45_69 = 209;
	return MummySetAttackFlag_293D0(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00029300) --------------------------------------------------------
void MummyFollow_29300(type_entity_0x6E8E* a1x)//20a300
{
	CreatureFollowLeader_1C560(a1x, 0xD0u);
	MummyResetAttackFlag_293B0(a1x);
}

//----- (00029330) --------------------------------------------------------
void MummyPreKill_29330(type_entity_0x6E8E* a1x)//20a330
{
	PreKillEntity_1C890(a1x, 208);
}

//----- (00029350) --------------------------------------------------------
void MummyKill_29350(type_entity_0x6E8E* a1x)//20a350
{
	KillEntity_1C930(a1x);
}

//----- (00029380) --------------------------------------------------------
void MummyScripted_29380(type_entity_0x6E8E* a1x)//20a380
{
	CreatureRunScript_1D5D0(a1x, 208);
	MummyResetAttackFlag_293B0(a1x);
}

//----- (000293B0) --------------------------------------------------------
void MummyResetAttackFlag_293B0(type_entity_0x6E8E* a1x)//20a3b0
{
	//int result; // eax

	//result = a1;
	if (a1x->actionIndex_0x45_69 == 210)
	{
		a1x->flags_0xc_12_15.byte[2] &= 0x7Fu;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
	}
	//return result;
}

//----- (000293D0) --------------------------------------------------------
void MummySetAttackFlag_293D0(type_entity_0x6E8E* a1x)//20a3d0
{
	//int result; // eax
	char v2; // dh

	//result = a1;
	if (a1x->actionIndex_0x45_69 != 210)
	{
		v2 = a1x->flags_0xc_12_15.byte[2];
		a1x->targetId_0x96_150 = 0;
		a1x->flags_0xc_12_15.byte[2] = v2 | 0x80;
		a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
	}
	//return result;
}

//----- (00029400) --------------------------------------------------------
void HydraIdle_29400(type_entity_0x6E8E* a1x)//20a400
{
	unsigned int v1; // eax
	type_entity_0x6E8E* ix; // eax
	int v3; // ecx
	__int16 v4; // ax
	signed int v5; // esi
	unsigned int v7; // [esp+0h] [ebp-4h]

	v1 = a1x->param_0x10_16;
	v7 = 0;
	a1x->param_0x10_16 = v1 + 1;
	if (v1 < 9)
	{
		if (v1 < 3)
		{
			if (!v1)
			{
				SetEntitySpriteIfChanged_2AED0(a1x, 337);
				a1x->life_0x8 = 1000000;
				a1x->targetId_0x96_150 = 0;
			}
			goto LABEL_26;
		}
		if (v1 <= 3)
		{
		LABEL_15:
			v7 = 1;
			ix = a1x;
			goto LABEL_35;
		}
		if (v1 != 6)
		{
		LABEL_26:
			if (!v7)
				goto LABEL_36;
			ix = a1x;
			goto LABEL_35;
		}
	}
	else
	{
		if (v1 <= 9)
		{
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			v3 = (a1x->rand_0x14_20 & 7) + 8;
			v4 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->rand_0x14_20 = v4;
			v5 = 0;
			predictedAxis_EB398ar.x += (x_WORD)v3 << 8;
			predictedAxis_EB398ar.y += ((v4 & 7) + 8) << 8;
			while (v5 < 128)
			{
				predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
				if (!CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1) && !CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 4) && GetTileSlopeWide_1B830(&predictedAxis_EB398ar) < 32)
					break;
				v5++;
				MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, 768);
			}
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 22);
			goto LABEL_36;
		}
		if (v1 >= 0xF)
		{
			if (v1 > 0xF)
			{
				if (v1 == 18)
				{
					a1x->actionIndex_0x45_69 = 218;
					v7 = 3;
					SetEntitySpriteIfChanged_2AED0(a1x, 337);
					a1x->targetId_0x96_150 = 0;
					a1x->levelPar3_0x46_70 = 1;
				}
				goto LABEL_26;
			}
			goto LABEL_15;
		}
		if (v1 != 12)
			goto LABEL_26;
	}
	v7 = 2;
LABEL_35:
	for (ix = a1x; ix > Entities_EA3E4[0]; ix = Entities_EA3E4[ix->nextLinkedId_0x34_52])
	{
		//ix->byte_0xe_14 &= 0x7Fu;
		//ix->byte_0xf_15 &= 0xFEu;
		ix->flags_0xc_12_15.word[1] &= 0xFE7F;
		if (v7 >= 1)
		{
			if (v7 <= 1)
			{
				ix->flags_0xc_12_15.byte[3] |= 1u;
			}
			else if (v7 == 2)
			{
				ix->flags_0xc_12_15.byte[2] |= 0x80u;
			}
		}
	}
LABEL_36:
	a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	HydraUpdateBody_29A90(a1x);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (00029670) --------------------------------------------------------
void HydraWander_29670(type_entity_0x6E8E* a1x)//20a670
{
	signed int v1; // eax
	__int16 v2; // ax
	char v3; // al

	v1 = HydraCheckDamage_2A6B0(a1x);
	if (v1)
	{
		if (v1 != 1)
		{
			HydraUpdateBody_29A90(a1x);
			return;
		}
		a1x->actionIndex_0x45_69 = 218;
		v2 = a1x->hitById_0x26_38;
		a1x->life_0x8 = 1000000;
		a1x->targetId_0x96_150 = v2;
	}
	v3 = MoveDragonEntity_2AF10(a1x, 1);
	if ((unsigned __int8)v3 >= 3u)
	{
		if ((unsigned __int8)v3 <= 3u)
		{
			HydraUpdateBody_29A90(a1x);
			return;
		}
		if (v3 == 4)
		{
			a1x->actionIndex_0x45_69 = 216;
			a1x->param_0x10_16 = 0;
			HydraUpdateBody_29A90(a1x);
			return;
		}
	}
	if (!(a1x->instanceIndex_0x3E_62 & 0x3F))
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		a1x->roll_0x20_32 = (a1x->rand_0x14_20 % 0x1C7u + a1x->roll_0x20_32 - 227) & 0x7FF;
	}
	HydraUpdateBody_29A90(a1x);
}

//----- (00029710) --------------------------------------------------------
void HydraAttack_29710(type_entity_0x6E8E* a1x)//20a710
{
	signed int v1; // eax
	char v2; // al
	type_entity_0x6E8E* v3x; // esi
	signed int v4; // edi
	__int16 v5; // ax
	unsigned int v6; // edi
	char v8; // [esp+0h] [ebp-4h]

	v8 = 0;
	v1 = HydraCheckDamage_2A6B0(a1x);
	if (v1)
	{
		if (v1 != 1)
			goto LABEL_19;
		a1x->life_0x8 = 1000000;
		a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
	}
	v2 = MoveDragonEntity_2AF10(a1x, a1x->levelPar3_0x46_70 == 0);
	if (v2 == 4)
	{
		a1x->actionIndex_0x45_69 = 216;
		a1x->param_0x10_16 = 0;
	}
	else
	{
		v3x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v3x <= Entities_EA3E4[0] || v3x->life_0x8 < 0 || v3x->flags_0xc_12_15.byte[1] & 4)
			goto LABEL_24;
		if (!(a1x->instanceIndex_0x3E_62 & 3) && v2 != 3 && !a1x->levelPar3_0x46_70)
			a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v3x->position_0x4C_76);
		if (!(a1x->instanceIndex_0x3E_62 & 0x1F))
		{
			v4 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
			v5 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v3x->position_0x4C_76);
			if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v5) <= v4)
			{
				a1x->levelPar3_0x46_70 = 1;
				SetEntitySpriteIfChanged_2AED0(a1x, 337);
			}
			else
			{
				a1x->levelPar3_0x46_70 = 0;
				SetEntitySpriteIfChanged_2AED0(a1x, 315);
			}
			v6 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
			if (Distance3D_583F0(&a1x->position_0x4C_76, &v3x->position_0x4C_76) >= v6)
				LABEL_24:
			v8 = 1;
		}
	}
LABEL_19:
	if (v8)
	{
		a1x->actionIndex_0x45_69 = 217;
		SetEntitySpriteIfChanged_2AED0(a1x, 315);
		a1x->targetId_0x96_150 = 0;
		a1x->levelPar3_0x46_70 = 0;
	}
	HydraUpdateBody_29A90(a1x);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00029890) --------------------------------------------------------
void HydraFollow_29890(type_entity_0x6E8E* a1x)//20a890
{
	a1x->actionIndex_0x45_69 = 217;
	HydraWander_29670(a1x);
}

//----- (000298B0) --------------------------------------------------------
void HydraPreKill_298B0(type_entity_0x6E8E* a1x)//20a8b0
{
	a1x->life_0x8 = -1;
	PreKillEntity_1C890(a1x, 216);
}

//----- (000298D0) --------------------------------------------------------
int HydraKill_298D0(type_entity_0x6E8E* a1x)//20a8d0
{
	type_entity_0x6E8E* v1x; // eax

	a1x->life_0x8 = -1;
	TransformEntityToManaSphere_36BA0(a1x, true);
	if (!(a1x->flags_0xc_12_15.byte[2] & 0x10))
	{
		v1x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 1);
		if (v1x)
			v1x->id_0x1A_26 = a1x->id_0x1A_26;
	}
	HydraHideSegments_2AE80(a1x);
	return 1;
}

//----- (00029930) --------------------------------------------------------
void HydraScripted_29930(type_entity_0x6E8E* a1x)//20a930
{
	unsigned __int8 v1; // al
	char v2; // ah
	int v3; // eax
	type_entity_0x6E8E* v4x; // eax

	CreatureRunScript_1D5D0(a1x, 216);
	v1 = a1x->StageVar2_0x49_73;
	if (v1 < 2u || v1 > 2u && (v1 < 6u || v1 > 9u))
		SetEntitySpriteIfChanged_2AED0(a1x, 315);
	else
		SetEntitySpriteIfChanged_2AED0(a1x, 337);
	v2 = a1x->actionIndex_0x45_69;
	a1x->life_0x8 = 1000000;
	if (v2 == -38)
	{
		v3 = a1x->nextLinkedId_0x34_52;
		a1x->StageVar2_0x49_73 = 10;
		while (1)
		{
			v4x = Entities_EA3E4[v3];
			if (v4x <= Entities_EA3E4[0])
				break;
			if (v4x->actionIndex_0x45_69 == 233 && v4x->levelPar3_0x46_70 == 1)
			{
				v4x->levelPar3_0x46_70 = 2;
				v4x->targetId_0x96_150 = a1x->targetId_0x96_150;
			}
			v3 = v4x->nextLinkedId_0x34_52;
		}
	}
	else if (v2 == -40)
	{
		a1x->StageVar2_0x49_73 = 15;
	}
	HydraUpdateBody_29A90(a1x);
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00029A90) --------------------------------------------------------
void HydraUpdateBody_29A90(type_entity_0x6E8E* a1x)//20aa90
{
	//uint8_t* v1; // edx
	//int v2; // eax
	type_entity_0x6E8E* ix; // ebx
	type_D404C* v4yy; // esi
	axis_3d* v4z; // esi
	type_entity_0x6E8E* v4x; // esi
	uint16_t v4xx;
	unsigned __int8 v5; // al
	int v6; // eax
	__int16 v7; // di
	unsigned __int16 v8; // ax
	__int16 v9; // ax
	type_entity_0x6E8E* v10x; // eax
	//int v11; // eax
	//int v12; // ecx
	type_entity_0x6E8E* v13x; // eax
	int v14; // eax
	int v15; // ecx
	int v16; // edi
	int v17; // eax
	signed int v18; // ecx
	type_entity_0x6E8E* v19x; // edx
	int j; // eax
	//int v21; // eax
	char v22; // dh
	signed int v23; // eax
	signed int v24; // eax
	int v25; // eax
	char v26; // cl
	__int16 v27; // ax
	__int16 v28; // ax
	int v29; // ecx
	int k; // eax
	int v31; // ecx
	unsigned int v32; // edx
	uint32_t v34x; // [esp+0h] [ebp-10h]
	type_entity_0x6E8E* v35x; // [esp+4h] [ebp-Ch]
	char v36; // [esp+8h] [ebp-8h]
	char v37; // [esp+Ch] [ebp-4h]

	uint16_t v1x;
	//type_entity_0x6E8E* v1y;

	//fix
	v34x = 0x355188;//20AA96 sub esp,10h: [ebp-10h] uninitialized, mostly 355188h in the original; DOSBox sets the same
	v35x = 0;
	//v1y = 0;
	//fix

	//HIWORD(v1) = HIWORD(a1);
	//LOWORD(v2) = a1x->nextLinkedId_0x34_52;
	for (ix = Entities_EA3E4[a1x->nextLinkedId_0x34_52]; ix > Entities_EA3E4[0]; ix = Entities_EA3E4[ix->nextLinkedId_0x34_52])
	{
		if (CommandLineParams.DoDebugSequences()) {
			//add_compare(0x20aab8, CommandLineParams.DoDebugafterload());
		}

		if (ix->actionIndex_0x45_69 == 233)
		{
			//LOBYTE(v1) = 0;
			v37 = 0;
			//BYTE1(v1) = *(x_BYTE *)(i + 62) + 1;
			//v4y = (x_WORD*)((char*)ar_D404C + 22 * ix->byte_0x3B_59);
			v4yy = &str_D404C[ix->poseIndex_0x3B_59];
			v5 = ix->levelPar3_0x46_70;
			ix->instanceIndex_0x3E_62++;// = BYTE1(v1);
			if (v5 <= 5u)
			{
				ix->rand_0x14_20 = 9377 * ix->rand_0x14_20 + 9439;
				ix->levelPar1_0x43_67 = ix->rand_0x14_20 % 0x14u;
				HydraPlaceSegment_2A5B0(a1x, ix, 672);
				HydraForwardDamage_2A660(a1x, ix);
				if (ix->levelPar3_0x46_70 == 1)
				{
					ix->rand_0x14_20 = 9377 * ix->rand_0x14_20 + 9439;
					v6 = ix->rand_0x14_20 & 7;
					v7 = ix->hitById_0x26_38;
					v34x = (ix->rand_0x14_20 & 7);
					if (v7)
					{
						if (v6 < 4)
						{
							ix->hitById_0x26_38 = 0;
							ix->levelPar3_0x46_70 = 2;
							v8 = ix->param3_0x36_54;
							ix->targetId_0x96_150 = v7;
							v8 += 22;
							ix->param3_0x36_54 = v8;
							if (v8 > 0x44u)
								ix->param3_0x36_54 = 68;
						}
					}
					else if (v6 < 4 && a1x->targetId_0x96_150 && !(ix->instanceIndex_0x3E_62 & 7))
					{
						v9 = a1x->targetId_0x96_150;
						ix->levelPar3_0x46_70 = 2;
						ix->targetId_0x96_150 = v9;
					}
				}
			}
			switch (ix->levelPar3_0x46_70)
			{
			case 0:
				ix->targetId_0x96_150 = 0;
				ix->levelPar3_0x46_70 = 1;
				ix->param2_0x2C_44 = 0;
				ix->param3_0x36_54 = 0;
				ix->minSpeed_0x84_132 = 16;
				goto LABEL_15;
			case 1:
			LABEL_15:
				if (a1x->timer_0x39_57)
				{
					if (!(ix->instanceIndex_0x3E_62 & 7))
					{
						//v1 = v34;
						if (v34x)
						{
							if (v34x > 4)
								ix->levelPar3_0x46_70 = 4;
						}
						else
						{
							v10x = HydraFindTarget_2A6F0(ix);
							v35x = v10x;
							if (v10x)
							{
								//v1 = (uint8_t*)(v11 % 168);
								ix->levelPar3_0x46_70 = 2;
								ix->targetId_0x96_150 = v10x - world_D41A0.entities_0x6E8E;
							}
						}
					}
					if (!(ix->instanceIndex_0x3E_62 & 7) && !(v34x & 1))
					{
						//v12 = ix->dword_0xA0_160;
						ix->rand_0x14_20 = 9377 * ix->rand_0x14_20 + 9439;
						v4xx = (a1x->yaw_0x1C_28 + v4yy->yawOffset_12 - ix->typeParams_0xA0_160x->wanderAngle_160_0x1e_30);
						//v1 = v4 + *(unsigned __int16 *)(i + 20) % (unsigned int)*(signed __int16 *)(v12 + 30);
						ix->yaw_0x1C_28 = (v4xx + ix->rand_0x14_20 % ix->typeParams_0xA0_160x->wanderAngle_160_0x1e_30);
					}
				}
				break;
			case 2:
				ix->levelPar3_0x46_70 = 3;
				ix->levelPar2_0x44_68 = 0;
				ix->param2_0x2C_44 = 2;
				ix->minSpeed_0x84_132 = 16;
				goto LABEL_26;
			case 3:
			LABEL_26:
				v13x = HydraGetTarget_2A7B0(ix);
				v35x = v13x;
				if (v13x)
				{
					//LOBYTE(v1) = *(x_BYTE *)(i + 68);
					if (ix->levelPar2_0x44_68 < 1u)
					{
						if (!ix->levelPar2_0x44_68 && !ix->actSpeed_0x82_130)
						{
							v4z = &v13x->position_0x4C_76;
							ix->levelPar2_0x44_68 = 1;
							ix->param2_0x2C_44 = 1;
							ix->minSpeed_0x84_132 = 16;
							ix->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&ix->position_0x4C_76, &v13x->position_0x4C_76);
							ix->pitch_0x1E_30 = Maths::GetPitchAngle_58210(&ix->position_0x4C_76, v4z);
							//v1 = a1x;
							ix->roll_0x20_32 = ix->yaw_0x1C_28 - a1x->yaw_0x1C_28;
							ix->fov_0x22_34 = ix->pitch_0x1E_30 - a1x->pitch_0x1E_30;
						}
					}
					else if (ix->levelPar2_0x44_68 <= 1u)
					{
						if (ix->actSpeed_0x82_130 == 192)
						{
							ix->param2_0x2C_44 = 3;
							ix->levelPar2_0x44_68 = 3;
							ix->param_0x10_16 = 4;
							v37 = 1;
						}
					}
					else if (ix->levelPar2_0x44_68 == 3)
					{
						v37 = 2;
						v14 = ix->param_0x10_16 - 1;
						ix->param_0x10_16 = v14;
						if (!v14)
						{
							ix->levelPar3_0x46_70 = 0;
							ix->param_0x10_16 = 1;
						}
					}
				}
				else
				{
					ix->levelPar3_0x46_70 = 0;
				}
				break;
			case 4:
				ix->levelPar3_0x46_70 = 5;
				ix->levelPar2_0x44_68 = 0;
				ix->param2_0x2C_44 = 2;
				ix->minSpeed_0x84_132 = 16;
				goto LABEL_40;
			case 5:
			LABEL_40:
				switch (ix->levelPar2_0x44_68)
				{
				case 0:
					if (!ix->actSpeed_0x82_130)
					{
						ix->levelPar2_0x44_68 = 1;
						ix->param2_0x2C_44 = 1;
						ix->minSpeed_0x84_132 = -16;
						ix->roll_0x20_32 = v4yy->yawOffset_12;
						ix->fov_0x22_34 = v4yy->fov_14;
						//LOWORD(v1) = *(x_WORD *)(i + 32);
						ix->yaw_0x1C_28 = ix->roll_0x20_32 + a1x->yaw_0x1C_28;
						ix->pitch_0x1E_30 = ix->fov_0x22_34 + a1x->pitch_0x1E_30;
					}
					break;
				case 1:
					if (ix->actSpeed_0x82_130 == -192)
					{
						ix->levelPar2_0x44_68 = 2;
						ix->param_0x10_16 = 2;
					}
					break;
				case 2:
					v15 = ix->param_0x10_16 - 1;
					ix->param_0x10_16 = v15;
					if (!v15)
					{
						ix->param2_0x2C_44 = 4;
						ix->levelPar2_0x44_68 = 6;
						ix->param_0x10_16 = 1;
					}
					break;
				case 5:
					//v1 = *(x_DWORD *)(i + 16) - 1;
					ix->param_0x10_16 = ix->param_0x10_16 - 1;
					if (ix->param_0x10_16 <= 4)
					{
						ix->levelPar3_0x46_70 = 0;
						ix->param_0x10_16 = 4;
					}
					break;
				case 6:
					PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 17);
					v16 = ix->param_0x10_16 + 1;
					ix->param_0x10_16 = v16;
					if (v16 >= 4)
						ix->levelPar2_0x44_68 = 5;
					break;
				default:
					goto LABEL_94;
				}
				break;
			case 6:
				ix->param_0x10_16 = 0;
				ix->param2_0x2C_44 = 2;
				ix->levelPar3_0x46_70 = 7;
				ix->levelPar2_0x44_68 = 0;
				ix->minSpeed_0x84_132 = 80;
				goto LABEL_52;
			case 7:
			LABEL_52:
				v36 = 0;
				HydraPlaceSegment_2A5B0(a1x, ix, 672);
				v17 = ix->levelPar2_0x44_68;
				switch ((x_BYTE)v17)
				{
				case 0:
					if (!ix->actSpeed_0x82_130)
					{
						ix->levelPar2_0x44_68 = 1;
						ix->param2_0x2C_44 = 1;
						ix->roll_0x20_32 = v4yy->yawOffset_12;
						//LOWORD(v17) = v4y[7];
						ix->fov_0x22_34 = v4yy->fov_14;
					}
					break;
				case 1:
					//LOWORD(v17) = ix->word_0x82_130;
					v36 = 1;
					if (ix->actSpeed_0x82_130 == 192)
					{
						ix->levelPar2_0x44_68 = 7;
						ix->param2_0x2C_44 = 5;
						ix->param_0x10_16 = 8;
					}
					break;
				case 7:
					v36 = 1;
					v17 = ix->param_0x10_16 - 1;
					ix->param_0x10_16 = v17;
					if (!v17)
					{
						ix->levelPar2_0x44_68 = 8;
						ix->param2_0x2C_44 = 6;
						ix->fov_0x22_34 = 0;
						ix->levelPar1_0x43_67 = 0;
						ix->param3_0x36_54 = 0;
						ix->minSpeed_0x84_132 = 12;
						ix->param_0x10_16 = 0;
					}
					break;
				case 8:
					v18 = ix->param_0x10_16;
					if (v18 > 10)
					{
						ix->levelPar3_0x46_70 = 8;
					}
					else
					{
						if (v18)
						{
							v19x = Entities_EA3E4[ix->nextLinkedId_0x34_52];
							for (j = 0; ; j++)
							{
								v35x = v19x;
								if (j >= 9 - ix->param_0x10_16)
									break;
								v19x = Entities_EA3E4[v19x->nextLinkedId_0x34_52];
							}
						}
						else
						{
							v35x = ix;
						}
						if (v35x > Entities_EA3E4[0])
							v35x->flags_0xc_12_15.byte[0] = (v35x->flags_0xc_12_15.byte[0] | 1) & 0xF7;
						//v21 = ix->dword_0x10_16 + 1;
						v22 = ix->levelPar1_0x43_67 + 1;
						ix->param_0x10_16++;// = v21;
						//LOWORD(v21) = v22;
						v17 = 28 * v22;
						ix->levelPar1_0x43_67 = v22;
						ix->param3_0x36_54 += v17;
					}
					break;
				default:
					break;
				}
				if (v36)
				{
					if (ix->instanceIndex_0x3E_62 & 1)
						v23 = -204;
					else
						v23 = 204;
					v17 = a1x->yaw_0x1C_28 + v4yy->yawOffset_12 + v23;
					ix->yaw_0x1C_28 = v17;
				}
				HydraMoveSegment_2A340(/*v17, a1x, a1x, (int)v4y,*/ /*a1x,*/ ix);
				HydraSetSegmentPose_2A940(a1x, ix);
				break;
			case 8:
				ix->levelPar3_0x46_70 = 9;
				ix->param_0x10_16 = 100;
				a1x->poseIndex_0x3B_59--;
				goto LABEL_83;
			case 9:
			LABEL_83:
				v25 = ix->param_0x10_16 - 1;
				ix->param_0x10_16 = v25;
				if (!v25)
					ix->levelPar3_0x46_70 = 10;
				break;
			case 0xA:
				a1x->poseIndex_0x3B_59++;
				ix->levelPar3_0x46_70 = 11;
				ix->param2_0x2C_44 = 5;
				v26 = ix->flags_0xc_12_15.byte[0];
				ix->param_0x10_16 = 7;
				v26 &= 0xF6u;
				v27 = ix->nextLinkedId_0x34_52;
				ix->flags_0xc_12_15.byte[0] = v26;
				ix->targetId_0x96_150 = v27;
				ix->flags_0xc_12_15.byte[0] = v26 | 8;
				ix->roll_0x20_32 = v4yy->yawOffset_12;
				v28 = v4yy->fov_14;
				ix->actSpeed_0x82_130 = 156;
				ix->fov_0x22_34 = v28;
				goto LABEL_86;
			case 0xB:
			LABEL_86:
				v29 = ix->param_0x10_16 - 1;
				ix->param_0x10_16 = v29;
				if (v29 <= 0)
				{
					ix->levelPar3_0x46_70 = 12;
					ix->param_0x10_16 = 0;
				}
				break;
			case 0xC:
				if (ix->param_0x10_16 < 9)
				{
					v1x = ix->nextLinkedId_0x34_52;
					for (k = 0; ; k++)
					{
						v4x = Entities_EA3E4[v1x];
						if (k >= ix->param_0x10_16)
							break;
						v1x = (unsigned __int16)v4x->nextLinkedId_0x34_52;
					}
					v4x->flags_0xc_12_15.byte[0] &= 0xFEu;
					v31 = ix->param_0x10_16 + 1;
					ix->targetId_0x96_150 = v4x->nextLinkedId_0x34_52;
					ix->param_0x10_16 = v31;
					if (v31 >= 9)
					{
						ix->rand_0x14_20 = 9377 * ix->rand_0x14_20 + 9439;
						v32 = ix->rand_0x14_20 % 0x398u;
						ix->mail_0x5E_94.channel[0].source = 0;
						ix->levelPar3_0x46_70 = 0;
						//v1 = v32 + 920;
						ix->life_0x8 = v32 + 920;
					}
				}
				break;
			case 0xD:
				ix->levelPar3_0x46_70 = 14;
				ix->levelPar1_0x43_67 = 10;
				ix->param_0x10_16 = 10;
				goto LABEL_77;
			case 0xE:
			LABEL_77:
				//v4 = (x_WORD *)(ix->dword_0x10_16 - 1);
				ix->param_0x10_16--;// = (x_DWORD)v4;
				if (!ix->param_0x10_16)
					ix->levelPar3_0x46_70 = 15;
				break;
			case 0xF:
				v35x = ix;
				ix->levelPar3_0x46_70 = 8;
				v24 = 0;
				do
				{
					v35x->flags_0xc_12_15.byte[0] = (v35x->flags_0xc_12_15.byte[0] | 1) & 0xF7;
					//v1y = Entities_EA3E4[v35x->nextLinkedId_0x34_52];
					v24++;
					v35x = Entities_EA3E4[v35x->nextLinkedId_0x34_52];
				} while (v24 < 10);
				break;
			default:
				break;
			}
		LABEL_94:
			//v2 = ix->byte_0x46_70;
			switch (ix->levelPar3_0x46_70)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				HydraMoveSegment_2A340(/*v2, v1y, a1x, (int)v4y,*/ /*a1x,*/ ix);
				HydraSetSegmentPose_2A940(a1x, ix);
				HydraAttachSegment_2AA90(/*ix,*/ a1x, ix);
				if (v37)
				{
					HydraChooseAttack_2A7F0(ix, v35x, v37 == 1);
					PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 17);
				}
				goto LABEL_99;
			case 6:
			case 7:
				/*LOWORD(v2) = */HydraAttachSegment_2AA90(/*ix, */a1x, ix);
				break;
			case 0xB:
			case 0xC:
				HydraPlaceSegment_2A5B0(a1x, ix, 672);
				HydraSetSegmentPose_2A940(a1x, ix);
				HydraAttachSegment_2AA90(/*ix,*/ a1x, ix);
				v35x = Entities_EA3E4[ix->targetId_0x96_150];
				predictedAxis_EB398ar = v35x->position_0x4C_76;
				CopyEntityPosition_57CF0(ix, &predictedAxis_EB398ar);
			LABEL_99:
				/*LOWORD(v2) = */HydraShootFromSegment_2A9F0(a1x, ix);
				break;
			default:
				continue;
			}
		}
	}
	//	return v2;
}
// D404C: using guessed type void *off_D404C;
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (0002A340) --------------------------------------------------------
void HydraMoveSegment_2A340(/*int a1, type_str_0x6E8E* a2x, type_str_0x6E8E* a3x, int a4,*/ /*type_str_0x6E8E* a5x,*/ type_entity_0x6E8E* a6x)//20b340
{
	int16_t v6x; // eax
	int16_t v7x; // eax
	__int16 v8; // cx
	int v9; // edi
	__int16 v10; // dx
	__int16 v11; // di
	__int16 v12; // cx
	__int16 v13; // cx

	int16_t a1z;
	int16_t a2z;
	int16_t a3z;
	int16_t a4z;

	//LOWORD(v6) = a6x->word_0x2C_44;
	switch (a6x->param2_0x2C_44)
	{
	case 0:
		a1z = a6x->maxSpeed_0x86_134;
		a2z = a6x->param3_0x36_54;
		//v7 = (int)(a2z + a1z + 73);
		v8 = (a2z + a1z + 73) + a6x->roll_0x20_32;
		v7x = a6x->maxSpeed_0x86_134;
		a4z = a6x->param3_0x36_54;
		a3z = a6x->fov_0x22_34;
		a6x->roll_0x20_32 = v8;
		v9 = (int)(a4z + v7x + 62 + a3z);
		v7x = a6x->actSpeed_0x82_130;
		a6x->fov_0x22_34 = v9;
		if (v7x != 192)
		{
			v10 = a6x->minSpeed_0x84_132 + a6x->actSpeed_0x82_130;
			a6x->actSpeed_0x82_130 = v10;
			if (abs(v10) > 192)
			{
				if (a6x->minSpeed_0x84_132 <= 0)
					a6x->actSpeed_0x82_130 = -192;
				else
					a6x->actSpeed_0x82_130 = 192;
				a6x->minSpeed_0x84_132 = -a6x->minSpeed_0x84_132;
			}
		}
		if (!(a6x->instanceIndex_0x3E_62 & 1))
		{
			v11 = a6x->param3_0x36_54;
			if (v11)
				a6x->param3_0x36_54 = v11 - 1;
		}
		//LOBYTE(v6) = a6x->byte_0x43_67;
		if (a6x->levelPar1_0x43_67)
		{
			if (a6x->levelPar1_0x43_67 == 3 && a6x->actSpeed_0x82_130 == 192)
			{
				a6x->minSpeed_0x84_132 = -16;
				//LOWORD(v6) = a6x->word_0x84_132;
				a6x->actSpeed_0x82_130 += a6x->minSpeed_0x84_132;
			}
		}
		else
		{
			a6x->rand_0x14_20 = 9377 * a6x->rand_0x14_20 + 9439;
			//LOWORD(v6) = a6x->word_0x14_20 / 0x1Cu;
			a6x->maxSpeed_0x86_134 = a6x->rand_0x14_20 % 0x1Cu;
		}
		break;
	case 1:
		//v6 = abs(a6x->word_0x82_130);
		if (abs(a6x->actSpeed_0x82_130) < 192)
		{
			//LOWORD(v6) = a6x->word_0x84_132;
			a6x->actSpeed_0x82_130 += a6x->minSpeed_0x84_132;
		}
		if (a6x->minSpeed_0x84_132 <= 0)
		{
			//v6 = a6x->word_0x82_130;
			if (a6x->actSpeed_0x82_130 < -192)
				LABEL_23:
			a6x->actSpeed_0x82_130 = -192;
		}
		else if (a6x->actSpeed_0x82_130 > 192)
		{
		LABEL_21:
			a6x->actSpeed_0x82_130 = 192;
		}
		break;
	case 2:
		//v6 = abs(a6x->word_0x82_130);
		if (abs(a6x->actSpeed_0x82_130) < a6x->minSpeed_0x84_132)
		{
			a6x->actSpeed_0x82_130 = 0;
		}
		else
		{
			v12 = a6x->actSpeed_0x82_130;
			if (v12 <= 0)
			{
				//LOWORD(v6) = a6x->word_0x84_132;
				a6x->actSpeed_0x82_130 = a6x->minSpeed_0x84_132 + v12;
			}
			else
			{
				//LOWORD(v6) = a6x->word_0x84_132;
				a6x->actSpeed_0x82_130 = v12 - a6x->minSpeed_0x84_132;
			}
		}
		break;
	case 3:
	case 4:
		//v6 = a6x->dword_0x10_16 - 1;
		switch (a6x->param_0x10_16)
		{
		case 1:
			goto LABEL_23;
		case 2:
			a6x->actSpeed_0x82_130 = -130;
			return;// v6;
		case 3:
			a6x->actSpeed_0x82_130 = -23;
			return;// v6;
		case 4:
			goto LABEL_21;
		default:
			return;// v6;
		}
	case 6:
		v13 = a6x->position_0x4C_76.z - a6x->param3_0x36_54;
		a6x->actSpeed_0x82_130 -= a6x->minSpeed_0x84_132;
		a6x->position_0x4C_76.z = v13;
		v6x = GetTerrainAlt_10C40(&a6x->position_0x4C_76);
		if ((int16_t)a6x->position_0x4C_76.z < v6x)
			a6x->position_0x4C_76.z = v6x;
		break;
	default:
		return;// v6;
	}
	//return v6;
}

//----- (0002A5B0) --------------------------------------------------------
void HydraPlaceSegment_2A5B0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, __int16 a3)//20b5b0
{
	type_D404C* v3x; // ebx
	__int16 v4; // si
	__int16 v5; // ax

	//v3 = (__int16*)((char*)ar_D404C + 22 * a2x->byte_0x3B_59);
	v3x = &str_D404C[a2x->poseIndex_0x3B_59];
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	MoveEntity_57FA0(&predictedAxis_EB398ar, v3x->angle_2 + a1x->yaw_0x1C_28, 0, v3x->distance_0);
	v4 = v3x->fov_14;
	v5 = a1x->pitch_0x1E_30;
	predictedAxis_EB398ar.z += v3x->height_4;
	MoveEntity_57FA0(&predictedAxis_EB398ar, v3x->yawOffset_12 + a1x->yaw_0x1C_28, v4 + v5, a3);
	CopyEntityPosition_57CF0(a2x, &predictedAxis_EB398ar);
}
// D404C: using guessed type void *off_D404C;
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (0002A660) --------------------------------------------------------
void HydraForwardDamage_2A660(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//20b660
{
	//type_entity_0x6E8E* resultx; // eax
	__int16 v3; // dx
	signed int v4; // edx
	__int16 v5; // dx
	int v6; // ecx

	//resultx = a2x;
	v3 = a2x->mail_0x5E_94.channel[0].source;
	if (v3)
	{
		a1x->mail_0x5E_94.channel[0].source = v3;
		v4 = a2x->mail_0x5E_94.channel[0].amount.dword;
		a1x->mail_0x5E_94.channel[0].amount.dword = v4;
		if (v4 > 76)
			v4 = 76;
		a2x->life_0x8 -= v4;
		v5 = a2x->mail_0x5E_94.channel[0].source;
		a2x->mail_0x5E_94.channel[0].source = 0;
		v6 = a2x->life_0x8;
		a2x->hitById_0x26_38 = v5;
		if (v6 < 0)
			a2x->levelPar3_0x46_70 = 6;
	}
	//return result;
}

//----- (0002A6B0) --------------------------------------------------------
signed int HydraCheckDamage_2A6B0(type_entity_0x6E8E* a1x)//20b6b0
{
	__int16 v1; // bx
	signed int v2; // edx
	char v3; // dl

	v1 = a1x->mail_0x5E_94.channel[0].source;
	v2 = 0;
	if (v1)
	{
		v3 = a1x->poseIndex_0x3B_59;
		a1x->hitById_0x26_38 = v1;
		if (v3)
		{
			v2 = 1;
		}
		else
		{
			a1x->actionIndex_0x45_69 = 220;
			v2 = 2;
			a1x->ownerPlayerId_0x24_36 = v1;
		}
		a1x->mail_0x5E_94.channel[0].source = 0;
	}
	return v2;
}

//----- (0002A6F0) --------------------------------------------------------
type_entity_0x6E8E* HydraFindTarget_2A6F0(type_entity_0x6E8E* a1x)//20b6f0
{
	int v1; // esi
	type_entity_0x6E8E* v2x; // edi
	type_entity_0x6E8E* v3x; // ebx
	int v4; // esi
	signed int v5; // ST08_4
	__int16 v6; // ax
	int v8; // [esp+4h] [ebp-10h]
	signed int v9; // [esp+10h] [ebp-4h]

	v1 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
	v2x = 0;
	v9 = 0x10000000;
	v3x = gameState_D41A4.class3List_38519;
	v8 = v1 * v1;
	while (v3x > Entities_EA3E4[0])
	{
		v4 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v3x->position_0x4C_76);
		if (v4 < v8)
		{
			v5 = a1x->typeParams_0xA0_160x->wanderAngle_160_0x1e_30;
			v6 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v3x->position_0x4C_76);
			if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v6) < v5 && v4 < v9)
			{
				v2x = v3x;
				v9 = v4;
			}
		}
		v3x = v3x->next_0;
	}
	return v2x;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002A7B0) --------------------------------------------------------
type_entity_0x6E8E* HydraGetTarget_2A7B0(type_entity_0x6E8E* a1x)//20b7b0
{
	unsigned __int16 v1; // bx
	type_entity_0x6E8E* resultx; // eax
	type_entity_0x6E8E* v3x; // edx

	v1 = a1x->targetId_0x96_150;
	resultx = 0;
	if (v1)
	{
		v3x = Entities_EA3E4[v1];
		if (v3x->life_0x8 >= 0 && !(v3x->flags_0xc_12_15.byte[1] & 4))
			resultx = Entities_EA3E4[v1];
	}
	return resultx;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002A7F0) --------------------------------------------------------
void HydraChooseAttack_2A7F0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x, char a3)//20b7f0
{
	type_entity_0x6E8E* v3x; // esi
	signed int v4; // edx
	unsigned int v5; // eax
	type_entity_0x6E8E* v6x; // eax
	__int16 v7; // [esp-4h] [ebp-8h]

	v3x = 0;
	if (a3)
	{
		a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
		v4 = a1x->rand_0x14_20 % 0xCu;
		a1x->rand_0x14_20 += gameState_D41A4.setting_30;
		a1x->manaRegen_0x88_136 = (v4 > 7) + 1;
	}
	v5 = a1x->manaRegen_0x88_136;
	if (v5 >= 1)
	{
		if (v5 <= 1)
		{
			if (!a3)
				goto LABEL_13;
			v6x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 0);
			v3x = v6x;
			if (!v6x)
				goto LABEL_13;
			v6x->levelPar1_0x43_67 = 10;
			v6x->levelPar2_0x44_68 = 0;
			v7 = 15;
		}
		else
		{
			if (v5 != 2)
				goto LABEL_13;
			v6x = CreateEntity_4A190(&a1x->position_0x4C_76, 9, 9);
			v3x = v6x;
			if (!v6x)
				goto LABEL_13;
			v6x->levelPar1_0x43_67 = 10;
			v6x->levelPar2_0x44_68 = 23;
			v7 = 23;
		}
		v6x->subSpellIndex_0x2A_42 = 850;
		PrepareEventSound_6E450(a1x->id_0x1A_26, -1, v7);
	}
LABEL_13:
	if (v3x)
	{
		v3x->id_0x1A_26 = a1x->id_0x1A_26;
		v3x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
		v3x->pitch_0x1E_30 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &a2x->position_0x4C_76);
		v3x->position_0x4C_76.z += a1x->params_0x52_82.fov / 2;
		v3x->targetId_0x96_150 = a1x->targetId_0x96_150;
		v3x->typeParams_0xA0_160x = &str_D7BD6[106]; //(type_str_160*)&unk_D7BD6[0xe14];
		v3x->xsubtype_0x42_66 = a2x->model_0x40_64;
		v3x->xtype_0x41_65 = a2x->class_0x3F_63;
	}
}
// D41A4: using guessed type int x_DWORD_D41A4;
// D89EA: using guessed type void *off_D89EA;

//----- (0002A940) --------------------------------------------------------
void HydraSetSegmentPose_2A940(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//20b940
{
	//signed int result; // eax
	type_D404C* v3x; // edx

	//result = a1;
	if (segmentPoseFlag_E9BA8)
	{
		//v3 = (char*)ar_D404C + 22 * a2x->byte_0x3B_59;
		v3x = &str_D404C[a2x->poseIndex_0x3B_59];
		a2x->roll_0x20_32 = v3x->yawOffset_12;
		//LOWORD(v3) = v3x->word_14;
		a2x->actSpeed_0x82_130 = 192;
		a2x->levelPar3_0x46_70 = 0;
		a2x->fov_0x22_34 = v3x->fov_14;
		a1x->flags_0xc_12_15.byte[1] |= 8u;
	}
	else
	{
		a1x->flags_0xc_12_15.byte[1] &= 0xF7u;
	}
	if (a2x->actSpeed_0x82_130)
	{
		predictedAxis_EB398ar = a2x->position_0x4C_76;
		MoveEntity_57FA0(
			&predictedAxis_EB398ar,
			a2x->roll_0x20_32 + a1x->yaw_0x1C_28,
			a2x->fov_0x22_34 + a1x->pitch_0x1E_30,
			a2x->actSpeed_0x82_130);
		CopyEntityPosition_57CF0(a2x, &predictedAxis_EB398ar);
	}
	//return result;
}
// D404C: using guessed type void *off_D404C;
// E9BA8: using guessed type int segmentPoseFlag_E9BA8;
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (0002A9F0) --------------------------------------------------------
void HydraShootFromSegment_2A9F0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//20b9f0
{
	//int v2; // eax
	type_D404C* v3x; // eax
	__int16 v4; // di
	__int16 v5; // ST0C_2
	__int16 v6; // dx

	//v2 = 22 * a2x->byte_0x3B_59;
	predictedAxis_EB398ar = a2x->position_0x4C_76;
	//v3 = (x_WORD*)((char*)ar_D404C + v2);
	v3x = &str_D404C[a2x->poseIndex_0x3B_59];
	v4 = v3x->fov_14;
	v5 = v3x->pitch_6;
	v6 = a1x->pitch_0x1E_30;
	predictedAxis_EB398ar.z += v3x->zOffset_10;
	MoveEntity_57FA0(&predictedAxis_EB398ar, v3x->yawOffset_12 + a1x->yaw_0x1C_28, v4 + v6, v5);
	CopyEntityPosition_57CF0(a2x, &predictedAxis_EB398ar);
}
// D404C: using guessed type void *off_D404C;
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (0002AA90) --------------------------------------------------------
void HydraAttachSegment_2AA90(/*type_str_0x6E8E* a1x,*/ type_entity_0x6E8E* a2x, type_entity_0x6E8E* a3x)//20ba90
{
	type_D404C* v3x; // ecx
	__int16 v4; // ax
	int v5; // eax
	signed int v6; // esi
	type_entity_0x6E8E* v7x; // edi
	//int v8; // eax
	int v9; // ebx
	__int16 v10; // ax
	unsigned __int16 result; // ax
	axis_3d v12x; // [esp+0h] [ebp-20h]
	//int v13; // [esp+4h] [ebp-1Ch]
	axis_3d v14x; // [esp+8h] [ebp-18h]
	//__int16 v15; // [esp+Ch] [ebp-14h]
	//__int16* v16; // [esp+10h] [ebp-10h]
	int v17; // [esp+14h] [ebp-Ch]
	int v18; // [esp+18h] [ebp-8h]
	int v19; // [esp+1Ch] [ebp-4h]

	int32_t a1y;

	//v3 = (__int16*)((char*)ar_D404C + 22 * a3x->byte_0x3B_59);
	v3x = &str_D404C[a3x->poseIndex_0x3B_59];
	v12x = a2x->position_0x4C_76;
	v4 = v3x->distance_0;
	//v16 = v3;
	MoveEntity_57FA0(&v12x, v3x->angle_2 + a2x->yaw_0x1C_28, 0, v4);
	v12x.z += v3x->height_4;
	v14x = a3x->position_0x4C_76;
	v5 = (signed int)(Distance3D_583F0(&v12x, &v14x) - 468) / 24;
	v18 = 16 - v5;
	if (16 - v5 <= 15)
	{
		if (16 - v5 < 0)
			v18 = 0;
	}
	else
	{
		v18 = 15;
	}
	//LOWORD(v19) = Maths::GetYawAngle_581E0(&v12x, &v14x);
	v19 = Maths::GetYawAngle_581E0(&v12x, &v14x);
	//LOWORD(v17) = Maths::GetPitchAngle_58210(&v12x, &v14x);
	v17 = Maths::GetPitchAngle_58210(&v12x, &v14x);
	v6 = 0;
	v7x = Entities_EA3E4[a3x->nextLinkedId_0x34_52];
	do
	{
		//v8 = 3 * v18;
		switch (v6)
		{
		case 0:
			a1y = 0;
			break;
		case 1:
		case 8:
			v9 = hydraSegmentOffsets_D40BC[v18][0];
			goto LABEL_12;
		case 2:
		case 7:
			v9 = hydraSegmentOffsets_D40BC[v18][1];
		LABEL_12:
			a1y = -v9;//fix
			break;
		case 3:
		case 6:
			a1y = hydraSegmentOffsets_D40BC[v18][1];
			break;
		case 4:
		case 5:
			a1y = hydraSegmentOffsets_D40BC[v18][0];
			break;
		default:
			break;
		}
		if (v6)
			MoveEntity_57FA0(&v12x, v19, a1y + v17, 96);
		CopyEntityPosition_57CF0(v7x, &v12x);
		if (a3x->levelPar3_0x46_70 == 7 && a3x->levelPar2_0x44_68 == 8)
		{
			v10 = GetTerrainAlt_10C40(&v7x->position_0x4C_76);
			if (v7x->position_0x4C_76.z <= v10)
				v7x->position_0x4C_76.z = v10;
		}
		result = v7x->nextLinkedId_0x34_52;
		v6++;
		v7x = Entities_EA3E4[result];
	} while (v6 < 9);
	//return result;
}
// D404C: using guessed type void *off_D404C;
// D40BC: using guessed type int x_DWORD_D40BC[];
// D40C0: using guessed type int x_DWORD_D40C0[];
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002AC50) --------------------------------------------------------
void HydraResetRotation_2AC50(type_entity_0x6E8E* a1x)//20bc50 //tex 394
{
	type_entity_0x6E8E* v1x; // ebx
	//uint8_t* v2; // ecx
	//int v3; // edx
	//uint8_t* v4; // ecx
	//char v5; // ah

	v1x = a1x;
	a1x->roll_0x20_32 = 0;
	v1x->yaw_0x1C_28 = a1x->roll_0x20_32;
	//v2 = x_D41A0_BYTEARRAY_0;
	v1x->pitch_0x1E_30 = a1x->roll_0x20_32;
	a1x->minSpeed_0x84_132 = 64;
	a1x->maxSpeed_0x86_134 = 0;
	a1x->actSpeed_0x82_130 = 30;
	a1x->poseIndex_0x3B_59 = 5;
	a1x->life_0x8 = 1000000;
	a1x->maxLife_0x4 = 36000;
	a1x->mana_0x90_144 = 20000;
	v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
	//v3 = a1x->byte_0x40_64;
	a1x->fov_0x22_34 = 0;
	//v4 = a1x->byte_0x40_64 + x_D41A0_BYTEARRAY_0;
	a1x->mailMask_0x38_56 = 1;
	//LOBYTE(v3) = (*(_BYTE *)(a1x->byte_0x40_64 + x_D41A0_BYTEARRAY_0 + 16))++;
	a1x->typeParams_0xA0_160x = &str_D7BD6[97]; //(type_str_160*)&unk_D7BD6[0xce2];
	a1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[a1x->model_0x40_64]++;
	//v5 = a1x->flags_0xc_12_15.dbyte3_4.byte2 | 0x80;
	//LOBYTE(v3) = *(x_BYTE *)(a1x->dword_0xA0_160 + 26);
	a1x->xtype_0x41_65 = 3;
	a1x->flags_0xc_12_15.byte[3] |= 0x80;
	a1x->timer_0x39_57 = a1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 + 1;
	SetEntityIndexAndRot_49CD0(a1x, 315);
	SetEntityShiftRot_49EA0(a1x, 1024, 1536);
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D88B8: using guessed type void *off_D88B8;

//----- (0002AD40) --------------------------------------------------------
void HydraCheckSegmentsDead_2AD40(type_entity_0x6E8E* a1x)//20bd40 //tex 402
{
	unsigned __int16 result; // ax
	signed int v2; // esi
	type_entity_0x6E8E* ix; // ebx
	__int16 v4; // cx
	int v5; // eax

	result = a1x->nextLinkedId_0x34_52;
	v2 = 1;
	for (ix = Entities_EA3E4[result]; ix > Entities_EA3E4[0]; ix = Entities_EA3E4[ix->nextLinkedId_0x34_52])
	{
		if (ix->actionIndex_0x45_69 == 233)
		{
			SetEntityIndexAndRot_49CD0(ix, 316);
			ix->flags_0xc_12_15.byte[3] |= 0xA0u;
			ix->rand_0x14_20 = 9377 * ix->rand_0x14_20 + 9439;
			v4 = ix->rand_0x14_20;
			ix->roll_0x20_32 = ix->rand_0x14_20 & 0x7FF;
			ix->rand_0x14_20 = 9377 * v4 + 9439;
			ix->fov_0x22_34 = ix->rand_0x14_20 & 0x7FF;
			ix->minSpeed_0x84_132 = 16;
			ix->actSpeed_0x82_130 = 16;
			ix->typeParams_0xA0_160x = &str_D7BD6[103]; //(type_str_160*)&unk_D7BD6[0xdae];
			ix->mailMask_0x38_56 = 1;
			v5 = 460 * v2 + 920;
			ix->life_0x8 = v5;
			ix->maxLife_0x4 = v5;
			/*result = */HydraSetSegmentPose_2A940(a1x, ix);
		}
		v2++;
	}
	//return result;
}
// D8984: using guessed type void *off_D8984;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002AE30) --------------------------------------------------------
void HydraKillSegments_2AE30(type_entity_0x6E8E* a1x)//20be30 //tex 410
{
	//int result; // eax
	int ix; // ebx
	type_entity_0x6E8E* v3x; // ebx

	//result = a1;
	for (ix = a1x->nextLinkedId_0x34_52; ; ix = v3x->nextLinkedId_0x34_52)
	{
		v3x = Entities_EA3E4[ix];
		if (v3x <= Entities_EA3E4[0])
			break;
		if (v3x->actionIndex_0x45_69 == 234)
		{
			/*result = */SetEntityIndexAndRot_49CD0(v3x, 317);
			v3x->flags_0xc_12_15.byte[3] |= 0xA0u;
		}
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002AE80) --------------------------------------------------------
void HydraHideSegments_2AE80(type_entity_0x6E8E* a1x)//20be80
{
	int ix; // ebx
	type_entity_0x6E8E* v2x; // ebx
	//int result; // eax

	if (a1x && a1x > Entities_EA3E4[0])
	{
		for (ix = a1x->nextLinkedId_0x34_52; ; ix = v2x->nextLinkedId_0x34_52)
		{
			v2x = Entities_EA3E4[ix];
			if (v2x <= Entities_EA3E4[0])
				break;
			DisableEntityDrawing04_57F10(v2x);
		}
		DisableEntityDrawing04_57F10(a1x);
	}
	//return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002AF10) --------------------------------------------------------
char MoveDragonEntity_2AF10(type_entity_0x6E8E* a1x, char a2)//20bf10
{
	char v2; // dh
	char v3; // al
	signed int v4; // eax
	__int16 v5; // ax
	signed int v7; // [esp+0h] [ebp-1Ch]
	unsigned __int16 v8; // [esp+4h] [ebp-18h]
	int v9; // [esp+8h] [ebp-14h]
	char v10; // [esp+Ch] [ebp-10h]
	char v11; // [esp+10h] [ebp-Ch]
	char v12; // [esp+14h] [ebp-8h]
	char v13; // [esp+18h] [ebp-4h]

	v2 = a1x->flags_0xc_12_15.byte[2];
	v10 = 0;
	v12 = 0;
	if (v2 & 0x10)
		a1x->flags_0xc_12_15.byte[2] = v2 & 0xEF;
	else
		a1x->flags_0xc_12_15.byte[0] &= 0xFEu;
	v3 = a1x->flags_0xc_12_15.byte[1];
	if (v3 & 8)
	{
		a1x->flags_0xc_12_15.byte[1] = v3 & 0xF7;
		v13 = 4;
	}
	else
	{
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		if (a2)
			MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		if (a2
			&& a1x->position_0x4C_76.x >> 8 == predictedAxis_EB398ar.x >> 8
			&& a1x->position_0x4C_76.y >> 8 == predictedAxis_EB398ar.y >> 8)
		{
			v10 = 1;
			v12 = 1;
			v13 = 1;
			a1x->flags_0xc_12_15.byte[2] &= 0xFBu;
		}
		else if (CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1) || CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 4) || GetTileSlopeWide_1B830(&predictedAxis_EB398ar) >= 32)
		{
			if (a1x->yaw_0x1C_28 == a1x->roll_0x20_32)
			{
				a1x->flags_0xc_12_15.byte[2] |= 4u;
				v7 = 91;
				v9 = 1;
				v11 = BYTE1(v7);
				while (v7 <= 1024)
				{
					predictedAxis_EB398ar = a1x->position_0x4C_76;
					v8 = (a1x->yaw_0x1C_28 + v9 * v7) & 0x7FF;
					MoveEntity_57FA0(&predictedAxis_EB398ar, v8, 0, a1x->actSpeed_0x82_130);
					predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
					if (!CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 1) && !CheckPositionCollision_102D0(a1x, &predictedAxis_EB398ar, 4) && GetTileSlopeWide_1B830(&predictedAxis_EB398ar) < 32)
					{
						v11 = 1;
						break;
					}
					v9 = 0 - v9;
					if (v9 == 1)
						v4 = 91;
					else
						v4 = 0;
					v7 += v4;
				}
				if (v11)
				{
					a1x->roll_0x20_32 = v8;
					v12 = 1;
					v13 = 3;
				}
				else
				{
					v13 = 4;
				}
			}
			else
			{
				a1x->flags_0xc_12_15.byte[2] |= 4u;
				v12 = 1;
				v13 = 3;
			}
		}
		else
		{
			v10 = 1;
			v12 = 1;
			v13 = 2;
			a1x->flags_0xc_12_15.byte[2] &= 0xFBu;
		}
		if (a2 && v10)
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		if (v12)
		{
			v5 = a1x->yaw_0x1C_28
				+ TurnTowardsAngle_58350(
					a1x->yaw_0x1C_28,
					a1x->roll_0x20_32,
					a1x->typeParams_0xA0_160x->yawTurnLimit_160_0x4_4,
					a1x->typeParams_0xA0_160x->subtype_160_0x2_2);
			HIBYTE(v5) &= 7u;
			a1x->yaw_0x1C_28 = v5;
		}
		a1x->position_0x4C_76.z = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	}
	if (v13 == 4)
	{
		a1x->actionIndex_0x45_69 = 216;
		a1x->param_0x10_16 = 0;
	}
	return v13;
}
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];
// EB39C: using guessed type __int16 x_WORD_EB398ar[2];

//----- (0002B1D0) --------------------------------------------------------
void ManaIdle_2B1D0(type_entity_0x6E8E* a1x)//20c1d0
{
	//signed int index; // eax

	/*index = */CreatureIdle_1BD90(a1x, 224);
	//HIBYTE(index) = *(x_BYTE *)(a1 + 69);
	if (a1x->actionIndex_0x45_69 == 226)
		/*result = */ManaStartAttack_2B840(a1x);
	//return result;
}

//----- (0002B200) --------------------------------------------------------
void ManaWander_2B200(type_entity_0x6E8E* a1x)//20c200
{
	CreatureWander_1BF90(a1x, 224);
	if (a1x->actionIndex_0x45_69 == 226)
		ManaStartAttack_2B840(a1x);
}

//----- (0002B260) --------------------------------------------------------
void ManaAttack_2B260(type_entity_0x6E8E* a1x)//20c260
{
	signed int v1; // eax
	__int16 v2; // dx
	type_entity_0x6E8E* v3x; // edx
	int v4; // edi
	type_entity_0x6E8E* v5x; // ecx
	//uint8_t* v6; // edx
	__int16 v7; // ax
	char v8; // dl
	unsigned __int16 v9; // di
	char v10; // al
	type_entity_0x6E8E* v11x; // edx
	int v12; // esi
	char v13; // al
	type_entity_0x6E8E* v14x; // ecx
	char v15; // cl
	signed __int16 v16; // ax
	char v17; // al
	unsigned __int16 v18; // cx
	type_entity_0x6E8E* v19x; // edx
	unsigned int v20; // esi
	int v21; // esi
	axis_3d v23x; // [esp+0h] [ebp-1Ch]
	//unsigned __int16 v24; // [esp+4h] [ebp-18h]
	type_entity_0x6E8E* v25x; // [esp+8h] [ebp-14h]
	char v26; // [esp+Ch] [ebp-10h]
	//char v27; // [esp+10h] [ebp-Ch]
	//char v28; // [esp+14h] [ebp-8h]
	char v29; // [esp+18h] [ebp-4h]

	// fix if begin
	v25x = 0;
	// end

	v1 = ManaCheckDamage_2B9A0(a1x);
	if ((unsigned int)v1 <= 1)
	{
		LOBYTE(v1) = a1x->levelPar3_0x46_70;
		switch ((x_BYTE)v1)
		{
		case 0:
			ManaSetMode_2B860(a1x, 3u);
			/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 1u);
			return;// v1;
		case 1:
			CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
			/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 2u);
			return;// v1;
		case 2:
			LOBYTE(v1) = 0;
			v2 = a1x->targetId_0x96_150;
			v29 = 0;
			if (v2)
			{
				v3x = Entities_EA3E4[a1x->targetId_0x96_150];
				v25x = v3x;
				if (v3x > Entities_EA3E4[0] && v3x->life_0x8 >= 0 && !(v3x->flags_0xc_12_15.byte[1] & 4))
					LOBYTE(v1) = 1;
			}
			if (!(x_BYTE)v1 || (v4 = a1x->param_0x10_16 - 1, a1x->param_0x10_16 = v4, v4 <= 0))
			{
				v29 = 1;
				goto LABEL_31;
			}
			v23x = v25x->position_0x4C_76;
			MoveEntity_57FA0(&v23x, v25x->yaw_0x1C_28, 0, 768);
			if (!(a1x->instanceIndex_0x3E_62 & 3))
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v23x);
			if (a1x->instanceIndex_0x3E_62 & 3)
				goto LABEL_22;
			//v28 = 0;
			v5x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
			while (2)
			{
				if (v5x > Entities_EA3E4[0])
				{
					if (v5x->id_0x1A_26 == a1x->id_0x1A_26
						|| abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v5x->position_0x4C_76.x) >= a1x->params_0x52_82.pitch
						|| abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v5x->position_0x4C_76.y) >= a1x->params_0x52_82.pitch)
					{
						v5x = v5x->next_0;
						continue;
					}
					a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v5x->position_0x4C_76, &a1x->position_0x4C_76);
					//v28 = 1;
				}
				break;
			}
		LABEL_22:
			v1 = MoveCreature_1B8C0(a1x);
			if (v1 == 3)
			{
				/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 7u);
			}
			else if (!(a1x->instanceIndex_0x3E_62 & 3) && a1x->param_0x10_16 < 14)
			{
				v1 = Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v23x);
				if (v1 < 2768896)
				{
					LOBYTE(v1) = IsAnyCreature28Alive_2B7E0(a1x);
					if (!(x_BYTE)v1)
						/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 3u);
				}
			}
		LABEL_31:
			if (v29)
			{
				LOBYTE(v1) = IsAnyCreature28Alive_2B7E0(a1x);
				if (!(x_BYTE)v1)
					/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 3u);
			}
			return;// v1;
		case 3:
			ManaSetState_2BA50(a1x, 4u);
			ManaSetMode_2B860(a1x, 2u);
			a1x->maxStage_0x30_48 = a1x->yaw_0x1C_28;
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 38);
			goto LABEL_35;
		case 4:
		case 5:
		LABEL_35:
			if (a1x->param_0x10_16 <= 0)
			{
				/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 6u);
				return;// v1;
			}
			v7 = a1x->maxStage_0x30_48;
			a1x->yaw_0x1C_28 = v7;
			v8 = a1x->levelPar3_0x46_70;
			a1x->roll_0x20_32 = v7;
			if (v8 == 4)
			{
				v9 = a1x->targetId_0x96_150;
				v10 = 0;
				if (v9)
				{
					v11x = Entities_EA3E4[v9];
					v25x = v11x;
					if (v11x > Entities_EA3E4[0] && v11x->life_0x8 >= 0 && !(v11x->flags_0xc_12_15.byte[1] & 4))
						v10 = 1;
				}
				if (v10)
				{
					if (!(a1x->instanceIndex_0x3E_62 & 7) && Maths::EuclideanDistXY_584D0(&a1x->position_0x4C_76, &v25x->position_0x4C_76) > 802816)
						a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v25x->position_0x4C_76);
					v12 = a1x->param_0x10_16;
					if (a1x->param2_0x2C_44 - 3 > v12 && v12 > 3 && CreatureMeleeAttack768_1CED0(a1x, v25x))
						a1x->levelPar3_0x46_70 = 5;
				}
			}
			v13 = a1x->instanceIndex_0x3E_62;
			a1x->param_0x10_16--;
			if (v13 & 3)
				goto LABEL_58;
			//v27 = 0;
			v14x = gameState_D41A4.creatureLists_38403x[a1x->model_0x40_64];
			break;
		case 6:
			ManaSetMode_2B860(a1x, 3u);
			CreateEntity_4A190(&a1x->position_0x4C_76, 10, 5);
			v17 = 0;
			v18 = a1x->targetId_0x96_150;
			v26 = 0;
			if (v18)
			{
				v19x = Entities_EA3E4[v18];
				v25x = v19x;
				if (v19x > Entities_EA3E4[0] && v19x->life_0x8 >= 0 && !(v19x->flags_0xc_12_15.byte[1] & 4))
					v17 = 1;
			}
			if (v17)
			{
				v20 = a1x->typeParams_0xA0_160x->range_160_0x1c_28;
				if (Distance3D_583F0(&a1x->position_0x4C_76, &v25x->position_0x4C_76) < v20)
					v26 = 1;
			}
			if (v26)
				/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 2u);
			else
				/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 7u);
			return;// v1;
		case 7:
			a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
			a1x->roll_0x20_32 = a1x->rand_0x14_20 & 0x7FF;
			ManaSetState_2BA50(a1x, 8u);
			goto LABEL_76;
		case 8:
		LABEL_76:
			LOBYTE(v1) = MoveCreature_1B8C0(a1x);
			v21 = a1x->param_0x10_16 - 1;
			a1x->param_0x10_16 = v21;
			if (v21 <= 0)
				/*LOBYTE(v1) = */ManaSetState_2BA50(a1x, 9u);
			return;// v1;
		case 9:
			/*LOBYTE(v1) = */ManaSetMode_2B860(a1x, 1u);
			a1x->actionIndex_0x45_69 = 225;
			a1x->targetId_0x96_150 = 0;
			return;// v1;
		default:
			return;// v1;
		}
		while (v14x > Entities_EA3E4[0])
		{
			if (v14x->id_0x1A_26 != a1x->id_0x1A_26
				&& abs((int16_t)a1x->position_0x4C_76.x - (int16_t)v14x->position_0x4C_76.x) < a1x->params_0x52_82.pitch
				&& abs((int16_t)a1x->position_0x4C_76.y - (int16_t)v14x->position_0x4C_76.y) < a1x->params_0x52_82.pitch)
			{
				a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&v14x->position_0x4C_76, &a1x->position_0x4C_76);
				//v27 = 1;
				break;
			}
			v14x = v14x->next_0;
		}
	LABEL_58:
		MoveCreature_1B8C0(a1x);
		v15 = a1x->param_0x10_16;
		a1x->maxStage_0x30_48 = a1x->yaw_0x1C_28;
		if (v15 & 4)
			v16 = 56;
		else
			v16 = -56;
		LOWORD(v1) = a1x->yaw_0x1C_28 + v16;
		a1x->yaw_0x1C_28 = v1;
		a1x->yaw_0x1C_28 &= 0x7ff;
	}
	//return v1;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002B750) --------------------------------------------------------
void ManaFollow_2B750(type_entity_0x6E8E* a1x)//20c750
{
	//int result; // eax

	//result = a1;
	a1x->actionIndex_0x45_69 = 225;
	//return result;
}

//----- (0002B760) --------------------------------------------------------
void ManaPreKill_2B760(type_entity_0x6E8E* a1x)//20c760
{
	PreKillEntity_1C890(a1x, 224);
}

//----- (0002B780) --------------------------------------------------------
void ManaKill_2B780(type_entity_0x6E8E* a1x)//20c780
{
	KillEntity_1C930(a1x);
}

//----- (0002B7B0) --------------------------------------------------------
void ManaScripted_2B7B0(type_entity_0x6E8E* a1x)//20c7b0
{
	CreatureRunScript_1D5D0(a1x, 224);
	if (a1x->actionIndex_0x45_69 == 226)
		ManaStartAttack_2B840(a1x);
}

//----- (0002B7E0) --------------------------------------------------------
char IsAnyCreature28Alive_2B7E0(type_entity_0x6E8E* a1x)//20c7e0
{
	type_entity_0x6E8E* v1x; // eax
	char v3; // bl

	v1x = gameState_D41A4.creatureLists_38403x[112 / 4];
	if (v1x <= Entities_EA3E4[0])
		return 0;
	do
	{
		if (v1x != a1x && v1x->actionIndex_0x45_69 == 226)
		{
			v3 = v1x->levelPar3_0x46_70;
			if (v3 == 3 || v3 == 4 || v3 == 5)
				return 1;
		}
		v1x = v1x->next_0;
	} while (v1x > Entities_EA3E4[0]);
	return 0;
}
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002B840) --------------------------------------------------------
void ManaStartAttack_2B840(type_entity_0x6E8E* a1x)//20c840
{
	//int result; // eax

	//result = a1;
	a1x->actionIndex_0x45_69 = 226;
	a1x->levelPar3_0x46_70 = 0;
	//return result;
}

//----- (0002B860) --------------------------------------------------------
void ManaSetMode_2B860(type_entity_0x6E8E* a1x, unsigned __int8 a2)//20c860 //tex 319,311
{
	char v3; // ah
	char v4; // dh
	unsigned __int16 v5; // si
	char v6; // ch

	if (a2 < 2u)
	{
		if (a2 == 1)
		{
			v3 = a1x->flags_0xc_12_15.byte[0] & 0xF6;
			a1x->typeParams_0xA0_160x = &str_D7BD6[93];
			a1x->flags_0xc_12_15.byte[0] = v3 | 8;
			SetEntityIndexAndRot_49CD0(a1x, 292);
			SetEntityShiftRot_49EA0(a1x, 85, 42);
			a1x->actSpeed_0x82_130 = a1x->maxSpeed_0x86_134;
		}
	}
	else if (a2 <= 2u)
	{
		a1x->typeParams_0xA0_160x = &str_D7BD6[93]; //(type_str_160*)&unk_D7BD6[0xc5a];
		a1x->param2_0x2C_44 = 0;
		v4 = a1x->flags_0xc_12_15.byte[0] & 0xF6;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132;
		a1x->flags_0xc_12_15.byte[0] = v4;
		v5 = *(int16_t*)&dataBlock_D9F50[0x5b6];
		a1x->flags_0xc_12_15.byte[0] = v4 | 8;
		SetEntityIndexAndRot_49CD0(a1x, 291);
		SetEntityShiftRot_49EA0(a1x, 384, 768);
		MarkTmapGroupUsed_71AB0(v5, 0);
		if (m_ptrLoadedSprites_F66F0x[v5])
		{
			a1x->param_0x10_16 = GetAnimationByIndex_724F0(animations_E9C08x, v5)->CountOfFrames_16;
			a1x->param2_0x2C_44 = a1x->param_0x10_16;
		}
	}
	else if (a2 == 3)
	{
		a1x->timer_0x39_57 = 0;
		a1x->typeParams_0xA0_160x = &str_D7BD6[94];
		v6 = a1x->flags_0xc_12_15.byte[0] | 1;
		a1x->actSpeed_0x82_130 = a1x->minSpeed_0x84_132 - 28;
		a1x->flags_0xc_12_15.byte[0] = v6 & 0xF7;
	}
}

//----- (0002B9A0) --------------------------------------------------------
signed int ManaCheckDamage_2B9A0(type_entity_0x6E8E* a1x)//20c9a0
{
	unsigned int v1; // edx
	__int16 v2; // dx
	int i; // ebx
	int v4; // ecx
	type_entity_0x6E8E* v5x; // ebx

	v1 = 0;
	if (a1x->mail_0x5E_94.channel[0].source)
	{
		a1x->life_0x8 -= a1x->mail_0x5E_94.channel[0].amount.dword;
		v2 = a1x->mail_0x5E_94.channel[0].source;
		a1x->mail_0x5E_94.channel[0].source = 0;
		a1x->hitById_0x26_38 = v2;
		v1 = 1;
	}
	else
	{
		a1x->hitById_0x26_38 = 0;
	}
	if (a1x->nextLinkedId_0x34_52)
	{
		for (i = a1x->nextLinkedId_0x34_52; ; i = v5x->nextLinkedId_0x34_52)
		{
			v5x = Entities_EA3E4[i];
			if (v5x == Entities_EA3E4[0])
				break;
			v4 = v5x->life_0x8;
			if (v4 < a1x->life_0x8)
			{
				a1x->life_0x8 = v4;
				a1x->hitById_0x26_38 = v5x->hitById_0x26_38;
				v1 = 1;
				break;
			}
		}
	}
	if (a1x->life_0x8 < 0)
	{
		a1x->ownerPlayerId_0x24_36 = a1x->hitById_0x26_38;
		v1 = 2;
	}
	if (v1 >= 1)
	{
		if (v1 <= 1)
		{
			a1x->targetId_0x96_150 = a1x->hitById_0x26_38;
		}
		else if (v1 == 2)
		{
			a1x->actionIndex_0x45_69 = 228;
			return 2;
		}
	}
	return v1;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0002BA50) --------------------------------------------------------
void ManaSetState_2BA50(type_entity_0x6E8E* a1x, unsigned __int8 a2)//20ca50
{
	//unsigned __int8 result; // al

	//result = a2;
	a1x->levelPar3_0x46_70 = a2;
	if (a2 < 2u)
		goto LABEL_10;
	if (a2 <= 2u)
	{
		a1x->param_0x10_16 = 32;
		return;// result;
	}
	if (a2 == 8)
		a1x->param_0x10_16 = 16;
	else
		LABEL_10:
	a1x->param_0x10_16 = 0;
	//return result;
}

//----- (0004B240) --------------------------------------------------------
type_entity_0x6E8E* AddDragon_4B240(axis_3d* position)//22c240 //tex 22
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	int8_t v6y;
	int8_t v6z;
	type_entity_0x6E8E* v8x; // eax
	type_entity_0x6E8E* v9x; // ebx
	type_entity_0x6E8E* v12x; // [esp+0h] [ebp-10h]
	type_entity_0x6E8E* v13x; // [esp+4h] [ebp-Ch]
	type_entity_0x6E8E* v14x; // [esp+8h] [ebp-8h]
	signed int v15; // [esp+Ch] [ebp-4h]

	if (GetFreeEntityCount_4A810() < 16)
		return 0;
	v1x = NewEvent_4A050();
	v2x = v1x;
	v12x = v1x;
	if (!v1x)
		return 0;
	v1x->actionIndex_0x45_69 = 1;
	v1x->class_0x3F_63 = 5;
	v1x->model_0x40_64 = 0;
	v1x->minSpeed_0x84_132 = 80;
	v1x->maxSpeed_0x86_134 = 16;
	v1x->actSpeed_0x82_130 = 30;
	v1x->maxLife_0x4 = 4000;
	v1x->mana_0x90_144 = 4500;
	v2x->maxMana_0x8C_140 = v1x->mana_0x90_144;
	v2x->mana_0x90_144 = v1x->mana_0x90_144 / 2;
	v2x->rand_0x14_20 = 9377 * v2x->rand_0x14_20 + 9439;
	v2x->roll_0x20_32 = (v2x->rand_0x14_20 & 0x7FF) - 1;
	v2x->yaw_0x1C_28 = (v2x->rand_0x14_20 & 0x7FF) - 1;
	v2x->pitch_0x1E_30 = v2x->roll_0x20_32;
	v2x->fov_0x22_34 = 0;
	v2x->param3_0x36_54 = 96;
	v2x->mailMask_0x38_56 = 1;
	v2x->param_0x10_16 = (v2x - world_D41A0.entities_0x6E8E) % 100;
	v6y = world_D41A0.modelCounters_0x10[v2x->model_0x40_64]++;
	v2x->typeParams_0xA0_160x = &str_D7BD6[71]; //(type_str_160*)&unk_D7BD6[0x96e];
	v2x->instanceIndex_0x3E_62 = v6y;
	v6z = (signed __int16)v2x->instanceIndex_0x3E_62 % v2x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
	v2x->xtype_0x41_65 = 3;
	v2x->param2_0x2C_44 = 0;
	v2x->levelPar3_0x46_70 = 0;
	v14x = v2x;
	v2x->fontTypeIndex_0x3D_61 = 0;
	v15 = 0;
	v2x->timer_0x39_57 = v2x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 - v6z + 4;
	while (v15 <= 15)
	{
		v8x = NewEvent_4A050();
		v9x = v8x;
		v13x = v8x;
		if (v8x)
		{
			qmemcpy(v8x, v12x, 0xA8u);
			v8x->leaderId_0x32_50 = v14x - world_D41A0.entities_0x6E8E;
			v14x->nextLinkedId_0x34_52 = v8x - world_D41A0.entities_0x6E8E;
			v8x->nextLinkedId_0x34_52 = 0;
			v8x->actionIndex_0x45_69 = 232;
			v12x->mana_0x90_144 = (v12x->maxMana_0x8C_140 - (__CFSHL__(v12x->maxMana_0x8C_140 >> 31, 5) + 32 * (v12x->maxMana_0x8C_140 >> 31))) >> 5;
			v8x->instanceIndex_0x3E_62 = v15;
			SetEntityIndexAndRot_49CD0(v8x, v15 + 19);
			v9x->param3_0x36_54 = v9x->params_0x52_82.pitch;
			AddEventToMap_57D70(v9x, position);
			CopyMaxLifeToLife_49A20(v9x);
		}
		v14x = v13x;
		++v15;
	}
	AddEventToMap_57D70(v12x, position);
	CopyMaxLifeToLife_49A20(v12x);
	SetEntityIndexAndRot_49CD0(v12x, 40);
	return v12x;
}

//----- (0004B490) --------------------------------------------------------
type_entity_0x6E8E* AddGoat_4B490(axis_3d* position)//22c490 - add creature Vulture(sup?) //tex 152
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 9;
		event->class_0x3F_63 = 5;
		event->model_0x40_64 = 1;
		event->minSpeed_0x84_132 = 54;
		event->maxSpeed_0x86_134 = 18;
		event->maxLife_0x4 = 600;
		event->actSpeed_0x82_130 = event->maxSpeed_0x86_134;
		SetEvent144_49C70(event);
		event->roll_0x20_32 = 0;
		event->yaw_0x1C_28 = event->roll_0x20_32;
		event->pitch_0x1E_30 = event->roll_0x20_32;
		event->param_0x10_16 = (event - world_D41A0.entities_0x6E8E) % 100;
		event->fov_0x22_34 = 0;
		event->mailMask_0x38_56 = 1;
		event->typeParams_0xA0_160x = &str_D7BD6[98]; //(type_str_160*)&unk_D7BD6[0xd04];
		event->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[event->model_0x40_64]++;
		event->timer_0x39_57 = event->typeParams_0xA0_160x->updateInterval_160_0x1a_26 + 1;
		event->xtype_0x41_65 = 3;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		SetEntityIndexAndRot_49CD0(event, 238);
	}
	return event;
}

//----- (0004B590) --------------------------------------------------------
type_entity_0x6E8E* AddBee_4B590(axis_3d* position)//22c590 //tex 14
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	int v3; // edx
	int v8; // ecx

	if (world_D41A0.terrain_2FECE.MapType != MapType_t::Day)
		return 0;
	v1x = NewEvent_4A050();
	v2x = v1x;
	if (!v1x)
		return 0;
	v1x->actionIndex_0x45_69 = 17;
	v1x->class_0x3F_63 = 5;
	v1x->model_0x40_64 = 2;
	v1x->minSpeed_0x84_132 = 64;
	v3 = v1x->minSpeed_0x84_132;
	v1x->maxSpeed_0x86_134 = 30;
	v1x->maxLife_0x4 = 3000;
	v1x->actSpeed_0x82_130 = v3 / 2;
	SetEvent144_49C70(v1x);
	v2x->rand_0x14_20 = 9377 * v2x->rand_0x14_20 + 9439;
	v2x->roll_0x20_32 = (v2x->rand_0x14_20 & 0x7FF) - 1;
	v2x->yaw_0x1C_28 = (v2x->rand_0x14_20 & 0x7FF) - 1;
	v2x->pitch_0x1E_30 = v2x->roll_0x20_32;
	v2x->fov_0x22_34 = 0;
	v2x->subSpellIndex_0x2A_42 = 200;
	v2x->xtype_0x41_65 = 3;
	v2x->xsubtype_0x42_66 = 0;
	v2x->param_0x10_16 = (v2x - world_D41A0.entities_0x6E8E) % 100;
	v2x->mailMask_0x38_56 = 1;
	v2x->typeParams_0xA0_160x = &str_D7BD6[73]; //(type_str_160*)&unk_D7BD6[0x9b2];
	v2x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
	v8 = v2x->typeParams_0xA0_160x->updateInterval_160_0x1a_26
		- v2x->instanceIndex_0x3E_62 % v2x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
	v2x->xtype_0x41_65 = 3;
	v2x->timer_0x39_57 = v8 + 4;
	AddEventToMap_57D70(v2x, position);
	CopyMaxLifeToLife_49A20(v2x);
	SetEntityIndexAndRot_49CD0(v2x, 3);
	SetEntityShiftRot_49EA0(v2x, 128, 128);
	return v2x;
}

//----- (0004B6F0) --------------------------------------------------------
type_entity_0x6E8E* AddWorm_4B6F0(axis_3d* position)//22c6f0 //tex 30
{
	type_entity_0x6E8E* resultx; // eax
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v6x; // ebx
	unsigned __int16 v8; // ax
	type_entity_0x6E8E* v9x; // [esp+0h] [ebp-10h]
	type_entity_0x6E8E* v10x; // [esp+4h] [ebp-Ch]
	type_entity_0x6E8E* v11x; // [esp+8h] [ebp-8h]
	signed int v12; // [esp+Ch] [ebp-4h]

	resultx = NewEvent_4A050();
	v9x = resultx;
	if (resultx)
	{
		resultx->actionIndex_0x45_69 = 25;
		resultx->class_0x3F_63 = 5;
		resultx->model_0x40_64 = 3;
		resultx->minSpeed_0x84_132 = 64;
		resultx->maxSpeed_0x86_134 = 16;
		resultx->actSpeed_0x82_130 = 30;
		resultx->maxLife_0x4 = 9000;
		SetEvent144_49C70(resultx);
		resultx->maxMana_0x8C_140 = resultx->mana_0x90_144;
		resultx->mana_0x90_144 = resultx->mana_0x90_144 / 2;
		resultx->rand_0x14_20 = 9377 * resultx->rand_0x14_20 + 9439;
		resultx->roll_0x20_32 = (resultx->rand_0x14_20 & 0x7FF) - 1;
		resultx->yaw_0x1C_28 = (resultx->rand_0x14_20 & 0x7FF) - 1;
		resultx->pitch_0x1E_30 = resultx->roll_0x20_32;
		resultx->fov_0x22_34 = 0;
		resultx->param3_0x36_54 = 96;
		resultx->mailMask_0x38_56 = 1;
		resultx->param_0x10_16 = (resultx - world_D41A0.entities_0x6E8E) % 100;
		resultx->typeParams_0xA0_160x = &str_D7BD6[74]; //(type_str_160*)&unk_D7BD6[0x9d4];
		resultx->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[resultx->model_0x40_64]++;
		v11x = resultx;
		resultx->xtype_0x41_65 = 3;
		v12 = 0;
		resultx->timer_0x39_57 = resultx->typeParams_0xA0_160x->updateInterval_160_0x1a_26 - (resultx->instanceIndex_0x3E_62 % resultx->typeParams_0xA0_160x->updateInterval_160_0x1a_26) + 4;
		while (v12 <= 15)
		{
			v5x = NewEvent_4A050();
			v6x = v5x;
			v10x = v5x;
			if (v5x)
			{
				qmemcpy(v5x, v9x, 0xA8u);
				v5x->leaderId_0x32_50 = v11x - world_D41A0.entities_0x6E8E;
				v11x->nextLinkedId_0x34_52 = v5x - world_D41A0.entities_0x6E8E;
				v5x->nextLinkedId_0x34_52 = 0;
				v5x->actionIndex_0x45_69 = 232;
				v5x->mana_0x90_144 = (v9x->maxMana_0x8C_140
					- (__CFSHL__(v9x->maxMana_0x8C_140 >> 31, 5)
						+ 32 * (v9x->maxMana_0x8C_140 >> 31))) >> 5;
				v5x->instanceIndex_0x3E_62 = v12;
				SetEntityIndexAndRot_49CD0(v5x, v12 + 89);
				v6x->params_0x52_82.pitch = 65 * spriteDefsTable_D951C[v12 + 89].speed_6 / 100;
				v6x->params_0x52_82.roll = 65 * spriteDefsTable_D951C[v12 + 89].speed_6 / 100;
				v6x->params_0x52_82.fov = 65 * spriteDefsTable_D951C[v12 + 89].worldSize_8 / 100;
				v8 = v6x->params_0x52_82.pitch;
				v6x->param3_0x36_54 = v8;
				if (!v12)
					v6x->param3_0x36_54 = 125 * v8 / 100;
				AddEventToMap_57D70(v10x, position);
				CopyMaxLifeToLife_49A20(v10x);
			}
			v11x = v10x;
			v12++;
		}
		AddEventToMap_57D70(v9x, position);
		CopyMaxLifeToLife_49A20(v9x);
		SetEntityIndexAndRot_49CD0(v9x, 88);
		v9x->params_0x52_82.pitch = 60 * spriteDefsTable_D951C[88].speed_6 / 100;//88 3
		v9x->params_0x52_82.roll = 60 * spriteDefsTable_D951C[88].speed_6 / 100;//88 3
		v9x->params_0x52_82.fov = 60 * spriteDefsTable_D951C[88].worldSize_8 / 100;//88 4
		resultx = v9x;
	}
	return resultx;
}

//----- (0004BA10) --------------------------------------------------------
type_entity_0x6E8E* AddArchers_4BA10(axis_3d* position)//22ca10 //tex 8
{
	type_entity_0x6E8E* entity = NewEvent_4A050();
	if (entity)
	{
		entity->actionIndex_0x45_69 = 33;
		entity->class_0x3F_63 = 5;
		entity->model_0x40_64 = 4;
		entity->minSpeed_0x84_132 = 30;
		entity->maxSpeed_0x86_134 = 0;
		entity->maxLife_0x4 = 1000;
		entity->actSpeed_0x82_130 = entity->minSpeed_0x84_132;
		SetEvent144_49C70(entity);
		entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
		entity->roll_0x20_32 = (entity->rand_0x14_20 & 0x7FF) - 1;
		entity->yaw_0x1C_28 = (entity->rand_0x14_20 & 0x7FF) - 1;
		entity->pitch_0x1E_30 = entity->roll_0x20_32;
		entity->fov_0x22_34 = 0;
		entity->param_0x10_16 = (entity - world_D41A0.entities_0x6E8E) % 100;
		entity->subSpellIndex_0x2A_42 = 500;
		entity->mailMask_0x38_56 = 1;
		entity->typeParams_0xA0_160x = &str_D7BD6[75]; //(type_str_160*)&unk_D7BD6[0x9f6];
		entity->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[entity->model_0x40_64]++;
		entity->xtype_0x41_65 = 3;
		entity->timer_0x39_57 = (entity->typeParams_0xA0_160x->updateInterval_160_0x1a_26 - entity->instanceIndex_0x3E_62 % entity->typeParams_0xA0_160x->updateInterval_160_0x1a_26) + 4;
		AddEventToMap_57D70(entity, position);
		CopyMaxLifeToLife_49A20(entity);
		SetEntityIndexAndRot_49CD0(entity, 0);
		SetEntityShiftRot_49EA0(entity, 128, 256);
	}
	return entity;
}

//----- (0004BBB0) --------------------------------------------------------
type_entity_0x6E8E* AddSkeletonArcher_4BBB0(axis_3d* a1x)//22cbb0 //tex 158
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	__int16 v3; // ax
	unsigned __int16 v4; // ax
	__int16 v5; // dx
	int v9; // esi

	v1x = NewEvent_4A050();
	v2x = v1x;
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 72;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 9;
		v1x->minSpeed_0x84_132 = 20;
		v1x->maxSpeed_0x86_134 = 0;
		v3 = v1x->minSpeed_0x84_132;
		v2x->maxLife_0x4 = 1000;
		v2x->actSpeed_0x82_130 = v3;
		SetEvent144_49C70(v2x);
		v2x->rand_0x14_20 = 9377 * v2x->rand_0x14_20 + 9439;
		v4 = v2x->rand_0x14_20;
		v2x->fov_0x22_34 = 0;
		v5 = v4 % 0x832u - 1;
		v2x->roll_0x20_32 = v5;
		v2x->yaw_0x1C_28 = v5;
		v2x->pitch_0x1E_30 = v2x->roll_0x20_32;
		v2x->subSpellIndex_0x2A_42 = 500;
		v2x->mailMask_0x38_56 = 1;
		v2x->typeParams_0xA0_160x = &str_D7BD6[80]; //(type_str_160*)&unk_D7BD6[0xaa0];
		v2x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v2x->model_0x40_64]++;
		v9 = v2x->typeParams_0xA0_160x->updateInterval_160_0x1a_26
			- v2x->instanceIndex_0x3E_62 % v2x->typeParams_0xA0_160x->updateInterval_160_0x1a_26;
		v2x->xtype_0x41_65 = 3;
		v2x->param_0x10_16 = 16;
		v2x->timer_0x39_57 = v9 + 4;
		AddEventToMap_57D70(v2x, a1x);
		v2x->position_0x4C_76.z = GetTerrainAlt_10C40(&v2x->position_0x4C_76);
		CopyMaxLifeToLife_49A20(v2x);
		SetEntityIndexAndRot_49CD0(v2x, 220);
		SetEntityShiftRot_49EA0(v2x, 128, 128);
		if (CheckPositionCollision_102D0(v2x, &v2x->position_0x4C_76, 1))
		{
			ReleaseEntity_57F20(v2x);
			v2x = 0;
		}
	}
	return v2x;
}

//----- (0004BD00) --------------------------------------------------------
type_entity_0x6E8E* AddEmu_4BD00(axis_3d* position)//22cd00 //tex 464
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	if (!(world_D41A0.terrain_2FECE.levelType_0x2FED2 & 2))
		return 0;
	v1x = NewEvent_4A050();
	v2x = v1x;
	if (!v1x)
		return 0;
	v1x->actionIndex_0x45_69 = 80;
	v1x->class_0x3F_63 = 5;
	v1x->model_0x40_64 = 10;
	v1x->maxLife_0x4 = 300000;
	SetEvent144_49C70(v1x);
	v2x->flags_0xc_12_15.dword |= 0x48800001u;
	v2x->subSpellIndex_0x2A_42 = 0;
	v2x->mailMask_0x38_56 = 1;
	v2x->typeParams_0xA0_160x = &str_D7BD6[105];//0x22CD64 offset off_D89C8 = row 105
	v2x->timer_0x39_57 = 64;
	v2x->xtype_0x41_65 = 3;
	v2x->param_0x10_16 = 0;
	v2x->levelPar3_0x46_70 = 0;
	v2x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v2x->model_0x40_64]++;
	AddEventToMap_57D70(v2x, position);
	v2x->position_0x4C_76.z = GetTerrainAlt_10C40(&v2x->position_0x4C_76);
	CopyMaxLifeToLife_49A20(v2x);
	SetEntityIndex_49C90(v2x, 341);
	v2x->params_0x52_82.yaw = 512;
	SetEntityShiftRot_49EA0(v2x, 1024, 1280);
	return v2x;
}

//----- (0004BDF0) --------------------------------------------------------
type_entity_0x6E8E* AddBuilder_4BDF0(axis_3d* position)//22cdf0 //tex 141
{
	type_entity_0x6E8E* v1x; // eax
	type_entity_0x6E8E* v2x; // ebx
	v1x = NewEvent_4A050();
	v2x = v1x;
	if (v1x)
	{
		v1x->maxSpeed_0x86_134 = 24;
		v1x->actSpeed_0x82_130 = v1x->maxSpeed_0x86_134;
		v1x->actionIndex_0x45_69 = 97;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 12;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->minSpeed_0x84_132 = 54;
		v1x->maxLife_0x4 = 1000;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->mana_0x90_144 = 0;
		v1x->fov_0x22_34 = 0;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
		v1x->subSpellIndex_0x2A_42 = 500;
		v1x->mailMask_0x38_56 = 1;
		v2x->typeParams_0xA0_160x = &str_D7BD6[101]; //(type_str_160*)&unk_D7BD6[0xd6a];
		v2x->timer_0x39_57 = 64;
		v2x->xtype_0x41_65 = 3;
		v2x->param_0x10_16 = 2;
		v2x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		AddEventToMap_57D70(v2x, position);
		CopyMaxLifeToLife_49A20(v2x);
		SetEntityIndexAndRot_49CD0(v2x, 221);
		SetEntityShiftRot_49EA0(v2x, 128, 128);
	}
	return v2x;
}

//----- (0004BF40) --------------------------------------------------------
type_entity_0x6E8E* AddVillager_4BF40(axis_3d* position)//22cf40 - add creature Townie //tex 183,199,180,155
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0x69;
		event->class_0x3F_63 = 0x05;
		event->model_0x40_64 = 0xD;
		event->minSpeed_0x84_132 = 54;
		event->maxSpeed_0x86_134 = 18;
		event->actSpeed_0x82_130 = event->maxSpeed_0x86_134;
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		event->roll_0x20_32 = (event->rand_0x14_20 & 0x7FF) - 1;
		event->yaw_0x1C_28 = (event->rand_0x14_20 & 0x7FF) - 1;
		event->pitch_0x1E_30 = event->roll_0x20_32;
		event->maxLife_0x4 = 1000;
		event->mana_0x90_144 = 0;
		event->fov_0x22_34 = 0;
		event->subSpellIndex_0x2A_42 = 500;
		event->param_0x10_16 = (event - world_D41A0.entities_0x6E8E) % 100;
		event->mailMask_0x38_56 = 1;
		event->typeParams_0xA0_160x = &str_D7BD6[100]; //(type_str_160*)&unk_D7BD6[0xd48];
		event->timer_0x39_57 = 64;
		event->xtype_0x41_65 = 3;
		event->param_0x10_16 = 2;
		event->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[event->model_0x40_64]++;
		AddEventToMap_57D70(event, position);
		CopyMaxLifeToLife_49A20(event);
		event->rand_0x14_20 = 9377 * event->rand_0x14_20 + 9439;
		switch (event->rand_0x14_20 % 9u)
		{
		case 0u:
		case 1u:
		case 2u:
			SetEntityIndexAndRot_49CD0(event, 242);
			break;
		case 3u:
		case 4u:
		case 5u:
			SetEntityIndexAndRot_49CD0(event, 271);
			break;
		case 6u:
		case 7u:
			SetEntityIndexAndRot_49CD0(event, 241);
			break;
		case 8u:
			SetEntityIndexAndRot_49CD0(event, 239);
			break;
		default:
			break;
		}
		SetEntityShiftRot_49EA0(event, 128, 128);
	}
	return event;
}

//----- (0004C0B0) --------------------------------------------------------
type_entity_0x6E8E* AddTrader_4C0B0(axis_3d* position)//22d0b0 //tex 138
{
	type_entity_0x6E8E* entity = NewEvent_4A050();
	if (entity)
	{
		entity->maxSpeed_0x86_134 = 18;
		entity->actSpeed_0x82_130 = entity->maxSpeed_0x86_134;
		entity->actionIndex_0x45_69 = 113;
		entity->class_0x3F_63 = 5;
		entity->model_0x40_64 = 14;
		entity->rand_0x14_20 = 9377 * entity->rand_0x14_20 + 9439;
		entity->minSpeed_0x84_132 = 54;
		entity->maxLife_0x4 = 1000;
		entity->roll_0x20_32 = (entity->rand_0x14_20 & 0x7FF) - 1;
		entity->yaw_0x1C_28 = (entity->rand_0x14_20 & 0x7FF) - 1;
		entity->pitch_0x1E_30 = entity->roll_0x20_32;
		entity->mana_0x90_144 = 0;
		entity->fov_0x22_34 = 0;
		entity->param_0x10_16 = (entity - world_D41A0.entities_0x6E8E) % 100;
		entity->subSpellIndex_0x2A_42 = 500;
		entity->mailMask_0x38_56 = 1;
		entity->typeParams_0xA0_160x = &str_D7BD6[100];
		entity->timer_0x39_57 = 64;
		entity->xtype_0x41_65 = 3;
		entity->param_0x10_16 = 2;
		entity->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[entity->model_0x40_64]++;
		AddEventToMap_57D70(entity, position);
		CopyMaxLifeToLife_49A20(entity);
		SetEntityIndexAndRot_49CD0(entity, 219);
		SetEntityShiftRot_49EA0(entity, 128, 128);
	}
	return entity;
}

//----- (0004C1E0) --------------------------------------------------------
type_entity_0x6E8E* AddExplodedHuman_4C1E0(axis_3d* position)//22d1e0 //tex 8
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 121;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 15;
		v1x->minSpeed_0x84_132 = 30;
		v1x->maxSpeed_0x86_134 = 0;
		v1x->maxLife_0x4 = 1000;
		v1x->roll_0x20_32 = 0;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		v1x->yaw_0x1C_28 = v1x->roll_0x20_32;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->mana_0x90_144 = 0;
		v1x->fov_0x22_34 = 0;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
		v1x->subSpellIndex_0x2A_42 = 500;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[83]; //(type_str_160*)&unk_D7BD6[0xb06];
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		v1x->xtype_0x41_65 = 3;
		v1x->timer_0x39_57 = (uint8_t)(v1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 - ((signed __int16)v1x->instanceIndex_0x3E_62 % v1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26) + 4);
		v1x->flags_0xc_12_15.byte[2] |= 2;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 0);
		SetEntityShiftRot_49EA0(v1x, 128, 128);
	}
	return v1x;
}

//----- (0004C310) --------------------------------------------------------
type_entity_0x6E8E* AddWyvern_4C310(axis_3d* position)//22d310 //tex 124
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 129;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 16;
		v1x->minSpeed_0x84_132 = 60;
		v1x->maxSpeed_0x86_134 = 20;
		v1x->maxLife_0x4 = 60000;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->fov_0x22_34 = 0;
		v1x->subSpellIndex_0x2A_42 = 500;
		v1x->mailMask_0x38_56 = 1;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
		v1x->typeParams_0xA0_160x = &str_D7BD6[84]; //(type_str_160*)&unk_D7BD6[0xb28];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->param_0x10_16 = 0;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 207);
		v1x->params_0x52_82.yaw = (5 * (unsigned __int16)*(int16_t*)&dataBlock_D9F50[294]
			- (__CFSHL__(5 * (unsigned __int16)*(int16_t*)&dataBlock_D9F50[294] >> 31, 3)
				+ 8 * (5 * (unsigned __int16)*(int16_t*)&dataBlock_D9F50[294] >> 31))) >> 3;
		SetEntityShiftRot_49EA0(v1x, 128, 128);
	}
	return v1x;
}

//----- (0004C460) --------------------------------------------------------
type_entity_0x6E8E* AddManticore_4C460(axis_3d* position)//22d460 //tex 263
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 137;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 17;
		v1x->minSpeed_0x84_132 = 68;
		v1x->maxSpeed_0x86_134 = 20;
		v1x->maxLife_0x4 = 10000;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->fov_0x22_34 = 0;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
		v1x->subSpellIndex_0x2A_42 = 350;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[85]; //(type_str_160*)&unk_D7BD6[0xb4a];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->param_0x10_16 = 0;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 285);
		SetEntityShiftRot_49EA0(v1x, 128, 128);
	}
	return v1x;
}

//----- (0004C590) --------------------------------------------------------
type_entity_0x6E8E* AddSentinel_4C590(axis_3d* position)//22d590 //tex 271
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = -109;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 18;
		v1x->minSpeed_0x84_132 = 10;
		v1x->maxSpeed_0x86_134 = 6;
		v1x->maxLife_0x4 = 36000;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->fov_0x22_34 = 0;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
		v1x->subSpellIndex_0x2A_42 = 500;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[86]; //(type_str_160*)&unk_D7BD6[0xb6c];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		v1x->param_0x10_16 = 100;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 286);
		SetEntityShiftRot_49EA0(v1x, 512, 512);
	}
	return v1x;
}

//----- (0004C6B0) --------------------------------------------------------
type_entity_0x6E8E* AddFirefly_4C6B0(axis_3d* position)//22d6b0 //tex 279
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 0x99;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 19;
		v1x->minSpeed_0x84_132 = 76;
		v1x->maxSpeed_0x86_134 = 8;
		v1x->maxLife_0x4 = 600;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->fov_0x22_34 = 0;
		v1x->subSpellIndex_0x2A_42 = 300;
		v1x->xtype_0x41_65 = 3;
		v1x->param_0x10_16 = (v1x - world_D41A0.entities_0x6E8E) % 100;
		v1x->xsubtype_0x42_66 = 0;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[88]; //(type_str_160*)&unk_D7BD6[0xbb0];
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		v1x->xtype_0x41_65 = 3;
		v1x->timer_0x39_57 = v1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 - v1x->instanceIndex_0x3E_62 % v1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 + 4;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 287);
		SetEntityShiftRot_49EA0(v1x, 85, 51);
	}
	return v1x;
}

//----- (0004C7F0) --------------------------------------------------------
type_entity_0x6E8E* AddSpider_4C7F0(axis_3d* a1x)//22d7f0 //tex 287
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = -95;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 20;
		v1x->minSpeed_0x84_132 = 32;
		v1x->maxSpeed_0x86_134 = 20;
		v1x->maxLife_0x4 = 5500;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->fov_0x22_34 = 0;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->subSpellIndex_0x2A_42 = 100;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[89]; //(type_str_160*)&unk_D7BD6[0xbd2];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		AddEventToMap_57D70(v1x, a1x);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 288);
		SetEntityShiftRot_49EA0(v1x, 384, 512);
	}
	return v1x;
}

//----- (0004C8F0) --------------------------------------------------------
type_entity_0x6E8E* AddDevil_4C8F0(axis_3d* position)//22d8f0
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actSpeed_0x82_130 = v1x->maxSpeed_0x86_134;
		v1x->actionIndex_0x45_69 = 169;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 21;
		v1x->minSpeed_0x84_132 = 96;
		v1x->maxLife_0x4 = 1000;
		v1x->mana_0x90_144 = 1000;
		v1x->fov_0x22_34 = 0;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->subSpellIndex_0x2A_42 = 400;
		v1x->mailMask_0x38_56 = 1;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;//fixed
		v1x->typeParams_0xA0_160x = &str_D7BD6[96]; //(type_str_160*)&unk_D7BD6[0xcc0];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->levelPar3_0x46_70 = 0;
		v1x->param2_0x2C_44 = 0;
		v1x->levelPar2_0x44_68 = 0;
		v1x->levelPar1_0x43_67 = 0;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		DevilSetSprite_26500(/*sub_49A20_remove_event(v1x),*/ v1x);
		DevilSetMode_268F0(v1x, 1u);
		SetEntityShiftRot_49EA0(v1x, 128, 128);
	}
	return v1x;
}

//----- (0004CA00) --------------------------------------------------------
type_entity_0x6E8E* AddFlyingMana_4CA00(axis_3d* position)//22da00
{
	type_entity_0x6E8E* v1x; // eax
	if (GetFreeEntityCount_4A810() < 15)
		return 0;
	v1x = NewEvent_4A050();
	if (!v1x)
		return 0;
	v1x->actionIndex_0x45_69 = 176;
	v1x->class_0x3F_63 = 5;
	v1x->model_0x40_64 = 22;
	v1x->minSpeed_0x84_132 = 128;
	v1x->maxSpeed_0x86_134 = 16;
	v1x->actSpeed_0x82_130 = v1x->maxSpeed_0x86_134;
	v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
	v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
	v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
	v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
	v1x->maxLife_0x4 = 2000;
	v1x->fov_0x22_34 = 0;
	v1x->typeParams_0xA0_160x = &str_D7BD6[90]; //(type_str_160*)&unk_D7BD6[0xbf4];
	v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
	v1x->xtype_0x41_65 = 3;
	v1x->mailMask_0x38_56 = 3;
	v1x->playerEntityIndex_0x94_148 = 0;
	v1x->params_0x52_82.yaw = 0;
	v1x->animationFrame_0x5C_92 = 0;
	v1x->param2_0x2C_44 = 11;
	v1x->subSpellIndex_0x2A_42 = 0;
	v1x->param3_0x36_54 = 0;
	v1x->targetId_0x96_150 = 1024;
	v1x->ownerPlayerId_0x24_36 = 0;
	v1x->levelPar3_0x46_70 = 15;
	v1x->timer_0x39_57 = v1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 - (signed __int16)v1x->instanceIndex_0x3E_62 % v1x->typeParams_0xA0_160x->updateInterval_160_0x1a_26 + 4;
	predictedAxis_EB398ar = *position;
	predictedAxis_EB398ar.z = GetTerrainAlt_10C40(&predictedAxis_EB398ar) + 384;
	AddEventToMap_57D70(v1x, &predictedAxis_EB398ar);
	SetEvent144_49C70(v1x);
	CopyMaxLifeToLife_49A20(v1x);
	return v1x;
}

//----- (0004CB60) --------------------------------------------------------
void AddFlyingManaSegments_4CB60(type_entity_0x6E8E* event)//22db60
{
	signed int v1; // ebx
	__int16 v2; // di
	int v3; // eax
	type_entity_0x6E8E* v5x; // [esp+0h] [ebp-8h]
	type_entity_0x6E8E* v6x; // [esp+4h] [ebp-4h]
	v1 = 1;
	v5x = event;
	while (v1 <= event->levelPar3_0x46_70 / 2)
	{
		v2 = 0;
		while (v2 < 2)
		{
			v6x = NewEvent_4A050();
			if (v6x)
			{
				if (v2)
					v3 = -(signed __int16)v1;
				else
					LOWORD(v3) = v1;
				AppendChainSegment_274C0(event, v6x, v5x, v3);
			}
			v2++;
			v5x = v6x;
		}
		v1++;
	}
	SetFlyingManaSprite_27590(event);
	SetFlyingManaChainSize_27610(event);
	UpdateChainSegments_276E0(event);
}

//----- (0004CBF0) --------------------------------------------------------
type_entity_0x6E8E* AddMoonDweller_4CBF0(axis_3d* position)//22dbf0 //tex 295
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 184;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 23;
		v1x->minSpeed_0x84_132 = 24;
		v1x->maxSpeed_0x86_134 = 14;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->maxLife_0x4 = 10000;
		v1x->mana_0x90_144 = 100;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->subSpellIndex_0x2A_42 = 500;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[91]; //(type_str_160*)&unk_D7BD6[0xc16];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->param2_0x2C_44 = 0x2000;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		position->z = v1x->param2_0x2C_44;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 289);
		SetEntityShiftRot_49EA0(v1x, 384, 384);
	}
	return v1x;
}

//----- (0004CCF0) --------------------------------------------------------
type_entity_0x6E8E* AddCreature24_4CCF0(axis_3d* position)//22dcf0 //tex 437
{
	type_entity_0x6E8E* v1x; // eax
	if (world_D41A0.terrain_2FECE.MapType != MapType_t::Cave)
		return 0;
	v1x = NewEvent_4A050();
	if (!v1x)
		return 0;
	v1x->actionIndex_0x45_69 = 193;
	v1x->class_0x3F_63 = 5;
	v1x->model_0x40_64 = 24;
	v1x->levelPar3_0x46_70 = 0;
	v1x->minSpeed_0x84_132 = 80;
	v1x->maxSpeed_0x86_134 = 24;
	v1x->maxLife_0x4 = 16000;
	v1x->actSpeed_0x82_130 = v1x->maxSpeed_0x86_134;
	SetEvent144_49C70(v1x);
	v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
	v1x->fov_0x22_34 = 0;
	v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
	v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
	v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
	v1x->subSpellIndex_0x2A_42 = 1500;
	v1x->mailMask_0x38_56 = 1;
	v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
	v1x->typeParams_0xA0_160x = &str_D7BD6[102]; //(type_str_160*)&unk_D7BD6[0xd8c];
	v1x->timer_0x39_57 = 64;
	v1x->xtype_0x41_65 = 3;
	AddEventToMap_57D70(v1x, position);
	CopyMaxLifeToLife_49A20(v1x);
	SetEntityIndexAndRot_49CD0(v1x, 335);
	SetEntityShiftRot_49EA0(v1x, 256, 640);
	return v1x;
}

//----- (0004CE00) --------------------------------------------------------
type_entity_0x6E8E* AddCymmerian_4CE00(axis_3d* position)//22de00 //tex 303
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 201;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 25;
		v1x->levelPar3_0x46_70 = 0;
		v1x->minSpeed_0x84_132 = 60;
		v1x->maxSpeed_0x86_134 = 20;
		v1x->maxLife_0x4 = 7500;
		v1x->actSpeed_0x82_130 = v1x->minSpeed_0x84_132;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->fov_0x22_34 = 0;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->subSpellIndex_0x2A_42 = 300;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[92]; //(type_str_160*)&unk_D7BD6[0xc38];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 290);
		SetEntityShiftRot_49EA0(v1x, 384, 384);
	}
	return v1x;
}

//----- (0004CF00) --------------------------------------------------------
type_entity_0x6E8E* AddMummy_4CF00(axis_3d* position)//22df00 //tex 411
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 209;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 26;
		v1x->minSpeed_0x84_132 = 25;
		v1x->maxSpeed_0x86_134 = 25;
		v1x->maxLife_0x4 = 4400;
		v1x->actSpeed_0x82_130 = v1x->maxSpeed_0x86_134;
		SetEvent144_49C70(v1x);
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->fov_0x22_34 = 0;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->subSpellIndex_0x2A_42 = 300;
		v1x->mailMask_0x38_56 = 1;
		v1x->typeParams_0xA0_160x = &str_D7BD6[99]; //(type_str_160*)&unk_D7BD6[0xd26];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 318);
		SetEntityShiftRot_49EA0(v1x, 256, 384);
		MummySetAttackFlag_293D0(v1x);
	}
	return v1x;
}

//----- (0004D000) --------------------------------------------------------
type_entity_0x6E8E* AddHydra_4D000(axis_3d* position)//22e000
{
	type_entity_0x6E8E* v2x; // eax
	type_entity_0x6E8E* v3x; // ebx
	int v4; // edi
	type_entity_0x6E8E* v5x; // eax
	type_entity_0x6E8E* v7x; // eax
	type_entity_0x6E8E* v8x; // ebx
	int v9; // eax
	type_entity_0x6E8E* v11x; // [esp+0h] [ebp-14h]
	signed int i; // [esp+4h] [ebp-10h]
	type_entity_0x6E8E* v13x; // [esp+8h] [ebp-Ch]
	type_entity_0x6E8E* v14x; // [esp+8h] [ebp-Ch]
	signed int v15; // [esp+Ch] [ebp-8h]
	char v16; // [esp+10h] [ebp-4h]
	type_entity_0x6E8E* a1x = 0;
	v16 = 0;
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
	{
		v16 = 1;
	}
	else
	{
		if (GetFreeEntityCount_4A810() >= 51)
		{
			v2x = NewEvent_4A050();
			v3x = v2x;
			a1x = v2x;
			if (v2x)
			{
				v2x->actionIndex_0x45_69 = 0xd9;
				v2x->class_0x3F_63 = 5;
				v2x->model_0x40_64 = 27;
				AddEventToMap_57D70(v2x, position);
				v13x = v3x;
				v15 = 0;
				v4 = v3x - world_D41A0.entities_0x6E8E;
				while (v15 < 5 && !v16)
				{
					v5x = NewEvent_4A050();
					v11x = v5x;
					if (v5x)
					{
						v5x->actionIndex_0x45_69 = 0xe9;
						v5x->class_0x3F_63 = 5;
						v5x->model_0x40_64 = 27;
						v5x->poseIndex_0x3B_59 = v15;
						v5x->id_0x1A_26 = v4;
						v5x->leaderId_0x32_50 = v4;
						v13x->nextLinkedId_0x34_52 = v5x - world_D41A0.entities_0x6E8E;
						v14x = v5x;
						v5x->nextLinkedId_0x34_52 = 0;
						AddEventToMap_57D70(v5x, position);
						for (i = 0; i < 9 && !v16; i++)
						{
							v7x = NewEvent_4A050();
							v8x = v7x;
							v11x = v7x;
							if (v7x)
							{
								v7x->actionIndex_0x45_69 = 234;
								v7x->class_0x3F_63 = 5;
								v7x->model_0x40_64 = 27;
								v7x->poseIndex_0x3B_59 = v15;
								v9 = v7x - world_D41A0.entities_0x6E8E;
								v8x->id_0x1A_26 = v4;
								v8x->leaderId_0x32_50 = v4;
								v14x->nextLinkedId_0x34_52 = v9;
								v8x->nextLinkedId_0x34_52 = 0;
								AddEventToMap_57D70(v8x, position);
							}
							else
							{
								v16 = 1;
							}
							v14x = v11x;
						}
					}
					else
					{
						v16 = 1;
					}
					v13x = v11x;
					v15++;
				}
			}
		}
		if (!a1x || v16)
		{
			HydraHideSegments_2AE80(a1x);
		}
		else
		{
			HydraResetRotation_2AC50(a1x);
			HydraCheckSegmentsDead_2AD40(a1x);
			HydraKillSegments_2AE30(a1x);
		}
	}
	if (v16)
		a1x = 0;
	return a1x;
}

//----- (0004D1D0) --------------------------------------------------------
type_entity_0x6E8E* AddMana_4D1D0(axis_3d* position)//22e1d0 //tex 319
{
	type_entity_0x6E8E* v1x = NewEvent_4A050();
	if (v1x)
	{
		v1x->actionIndex_0x45_69 = 225;
		v1x->class_0x3F_63 = 5;
		v1x->model_0x40_64 = 28;
		v1x->minSpeed_0x84_132 = 120;
		v1x->maxSpeed_0x86_134 = 64;
		v1x->maxLife_0x4 = 8000;
		SetEvent144_49C70(v1x);
		v1x->flags_0xc_12_15.byte[3] |= 8u;
		v1x->rand_0x14_20 = 9377 * v1x->rand_0x14_20 + 9439;
		v1x->fov_0x22_34 = 0;
		v1x->roll_0x20_32 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->yaw_0x1C_28 = (v1x->rand_0x14_20 & 0x7FF) - 1;
		v1x->pitch_0x1E_30 = v1x->roll_0x20_32;
		v1x->subSpellIndex_0x2A_42 = 2000;
		v1x->mailMask_0x38_56 = 1;
		v1x->instanceIndex_0x3E_62 = world_D41A0.modelCounters_0x10[v1x->model_0x40_64]++;
		v1x->typeParams_0xA0_160x = &str_D7BD6[93]; //(type_str_160*)&unk_D7BD6[0xC5A];
		v1x->timer_0x39_57 = 64;
		v1x->xtype_0x41_65 = 3;
		v1x->actSpeed_0x82_130 = v1x->maxSpeed_0x86_134 + (v1x->minSpeed_0x84_132 - v1x->maxSpeed_0x86_134) / 2;
		AddEventToMap_57D70(v1x, position);
		CopyMaxLifeToLife_49A20(v1x);
		SetEntityIndexAndRot_49CD0(v1x, 292);
		SetEntityShiftRot_49EA0(v1x, 85, 42);
	}
	return v1x;
}

//----- (00068BD0) --------------------------------------------------------
char IsCreatureModel0_68BD0(type_entity_0x6E8E*  /*a1x*/, type_entity_0x6E8E* a2x)//249bd0
{
	char result; // al

	result = a2x->class_0x3F_63;
	if (result == 5)
	{
		result = a2x->model_0x40_64;
		if (!result)
			a2x->fontTypeIndex_0x3D_61 = 32;
	}
	return result;
}

//----- (00068BF0) --------------------------------------------------------
void UpdateAllCreatures_68BF0()//249bf0
{
	for (int i = 0; i < 29; i++)
	{
		for (type_entity_0x6E8E* jx = gameState_D41A4.creatureLists_38403x[i]; jx > Entities_EA3E4[0]; jx = jx->next_0)
		{
			if (jx->life_0x8 >= 0)
			{
				if (CommandLineParams.DoDebugSequences()) {
					//add_compare(0x249c1b, CommandLineParams.DoDebugafterload());
				}
				UpdateChainedProjectile_68C70(jx);
			}
			else
			{
				jx->timer_0x39_57 = 0xfau;
				jx->chainCount_0x3A_58 = 0;
			}
		}
	}
	for (type_entity_0x6E8E* kx = gameState_D41A4.manaSphereList_38523; kx > Entities_EA3E4[0]; kx = kx->next_0)
		UpdateChainedProjectile_68C70(kx);
}
