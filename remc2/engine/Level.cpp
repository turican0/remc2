#include "Level.h"
#include <memory>
#include "EventsFunctions.h"
#include "Castle.h"
#include "Creatures.h"
#include "Effects.h"
#include "CommandLineParser.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Events.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"


POSITION textPosition_F4720; // weak

bool IsAfterLoad = false;

void BuildLevelSpriteList_71890();
void LoadLevelSpriteTmaps_718F0();


char x_BYTE_E29E8 = 1; // weak
type_str_164 unk_F42B0x[1136]; // weak//2c52b0
type_str_160 str_D7BD6[157] = {
{0x0005,0x0000,0x0013,0x0028,0x002A,0x0154,0x0007,0x00F4,0xFFFF,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0001,0x00EE,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0002,0x0003,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0003,0x0059,0x0058,0x002A,0x0154,0x0007,0x00F4,0xFFFF,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0004,0x0000,0x0001,0x00CE,0x00C2,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0005,0x00B9,0x002A,0x0154,0x0007,0x00F4,0xFFFF,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0006,0x00C1,0x0032,0x0031,0x00D8,0xFFFF,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0007,0x0055,0x00C6,0x0055,0x00C4,0xFFFF,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0008,0x002F,0x002A,0x0154,0x0007,0x00F4,0xFFFF,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0009,0x00DC,0x00ED,0x00CA,0x00C9,0x00F5,0x00C9,0x00CB,0xFFFF,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x000A,0x0157,0x0158,0x0155,0x0156,0x0159,0xFFFF,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x000B,0x00C8,0x002A,0x0154,0x0007,0x00F4,0x00D1,0xFFFF,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x000C,0x00DD,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x000D,0x00D9,0x00DA,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x000E,0x00DB,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x000F,0x0000,0x0001,0x00CE,0x00C3,0xFFFF,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0010,0x00CF,0x002A,0x0154,0x0007,0x00F4,0xFFFF,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0011,0x011D,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0012,0x011E,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0013,0x011F,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0014,0x0120,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0015,0x0131,0x0132,0x0133,0x0134,0x0135,0x0136,0x0137,0x0138,0x0000FFFF,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0016,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0017,0x0121,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0018,0x014F,0x0150,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x0019,0x0122,0x0139,0x013A,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x001A,0x013E,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x001B,0x013B,0x013C,0x013D,0x0151,0xFFFF,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0005,0x001C,0x0123,0x0124,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000A,0x0009,0x00D2,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000A,0x0022,0x00D4,0x00DF,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000A,0x0027,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000A,0x0039,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000A,0x002C,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000A,0x0034,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0000,0x002A,0x0154,0x0007,0x00F4,0xFFFF,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0001,0x00D1,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0002,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0004,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0007,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0008,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0009,0x004C,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x000A,0x00D4,0x00DF,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x000B,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x000C,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x000D,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x000E,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x000F,0x00D3,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0010,0x00D3,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0011,0x00D3,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0012,0x00D2,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0013,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0014,0x004C,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0015,0x0140,0x0125,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0016,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0017,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0018,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x000F,0x0019,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0000,0x0038,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x0C},0x0032,0x1000,0x0200,0x00,{0x00}},
{0x0001,0x0016,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0002,0x0071,0x0005,0x0071,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0003,0x0011,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0004,0x000B,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0005,0x0005,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0006,0x0000,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0007,0x0100,0x0000,0x0100,0x0000,0x0400,0x0100,0xFFF0,0x0100,0x0200,0xFFFFFEFF,{0x00,0x18},0x0028,0x2000,0x0200,0x00,{0x00}},
{0x0008,0x0100,0x0005,0x0100,0x0005,0x0300,0x0080,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x18},0x0028,0x2000,0x0200,0x00,{0x00}},
{0x0009,0x0038,0x0005,0x0016,0x0005,0x0600,0x0200,0xFFF0,0x0100,0x0200,0xFFFFFFFF,{0x00,0x0C},0x0032,0x1000,0x0200,0x00,{0x00}},
{0x000A,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x000F,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0C00,0x0200,0x00,{0x00}},
{0x000B,0x0016,0x0005,0x0016,0x0005,0x0700,0x0200,0xFFFC,0x0046,0x0155,0xFFFFFFFF,{0x00,0x0F},0x0028,0x0F00,0x0200,0x00,{0x00}},
{0x000C,0x0022,0x0005,0x0055,0x0005,0x0000,0x0000,0xFFFC,0x001E,0x0155,0xFFFFFFFF,{0x00,0x14},0x001E,0x1400,0x00AA,0x01,{0x00}},
{0x000D,0x0016,0x0005,0x0016,0x0005,0x0500,0x0200,0xFFFC,0x0046,0x0200,0xFFFFFFFF,{0x00,0x09},0x001E,0x0C00,0x0200,0x01,{0x00}},
{0x000E,0x0071,0x0011,0x0071,0x0011,0x0700,0x0033,0xFFE0,0x0014,0x0200,0xFFFFFFFF,{0x00,0x06},0x001E,0x1400,0x02AA,0x01,{0x00}},
{0x000F,0x0022,0x0005,0x0055,0x0005,0x0100,0x0000,0xFFC0,0x0014,0x0155,0xFFF080FE,{0x00,0x14},0x001E,0x1400,0x00AA,0x01,{0x00}},
{0x0010,0x0016,0x0005,0x0071,0x0000,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x0A},0x001E,0x1400,0x0200,0x01,{0x00}},
{0x0011,0x0038,0x0005,0x0016,0x0005,0x0100,0x0000,0xFFC0,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x001E,0x1400,0x0200,0x01,{0x00}},
{0x0012,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFFE0,0x0014,0x0000,0x00000001,{0x00,0x09},0x0023,0x1400,0x02AA,0x01,{0x00}},
{0x0013,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0F00,0x0200,0x01,{0x00}},
{0x0014,0x0038,0x0005,0x0016,0x0005,0x0500,0x0000,0xFFE7,0x0050,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1900,0x0200,0x01,{0x00}},
{0x0015,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0019,0x0800,0x0200,0x01,{0x00}},
{0x0016,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0C00,0x0200,0x01,{0x00}},
{0x0017,0x002D,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x003C,0x0200,0xFFFFFFFF,{0x00,0x09},0x000A,0x1E00,0x07C7,0x01,{0x00}},
{0x0018,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x0014,0x0200,0x00020000,{0x00,0x09},0x000F,0x0F00,0x0200,0x01,{0x00}},
{0x0019,0x0044,0x0005,0x0016,0x0005,0x0700,0x0100,0xFFF8,0x0046,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1200,0x0200,0x11,{0x00}},
{0x001A,0x002D,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0014,0x0F00,0x0200,0x01,{0x00}},
{0x001B,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0004,0x1900,0x0200,0x07,{0x00}},
{0x001C,0x0400,0x0005,0x0200,0x0005,0x0000,0x0000,0x0000,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0014,0x0F00,0x0200,0x01,{0x00}},
{0x001D,0x0071,0x0011,0x0071,0x0011,0x0700,0x0033,0xFFF8,0x0014,0x0200,0xFFFFFFFF,{0x00,0x06},0x0023,0x1400,0x02AA,0x01,{0x00}},
{0x001E,0x00E3,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0023,0x1400,0x02AA,0x00,{0x00}},
{0x001F,0x0100,0x0005,0x0016,0x0005,0x0000,0x0000,0x0000,0x0032,0x0200,0xFFFFFFFF,{0x00,0x09},0x0014,0x1400,0x0200,0x05,{0x00}},
{0x0020,0x0100,0x0005,0x0016,0x0005,0x2000,0x0000,0x0000,0x0014,0x0200,0xFFFFFFFF,{0x00,0x09},0x0003,0x0C00,0x02AA,0x05,{0x00}},
{0x0021,0x0071,0x0005,0x0016,0x0005,0x2000,0x0000,0xFFE0,0x0014,0x0200,0xFFFFFFFF,{0x00,0x09},0x0014,0x0C00,0x02AA,0x05,{0x00}},
{0x0022,0x0071,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0000,0x00000001,{0x00,0x09},0x0010,0x1000,0x02AA,0x05,{0x00}},
{0x0023,0x0100,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0000,0x00000001,{0x00,0x09},0x0010,0x1000,0x02AA,0x05,{0x00}},
{0x0024,0x00AA,0x0005,0x0016,0x0005,0x2000,0x0000,0xFF00,0x0200,0x0200,0xFFFFFFFF,{0x00,0x09},0x0010,0x1000,0x0400,0x05,{0x00}},
{0x0025,0x0400,0x0000,0x0016,0x0005,0x2000,0x0000,0x0000,0x0155,0x0200,0xFFFFFFFF,{0x00,0x09},0x0010,0x1200,0x009F,0x05,{0x00}},
{0x0026,0x0016,0x0005,0x0016,0x0005,0x0000,0x0000,0xFE00,0x00AA,0x00AA,0xFFF080FE,{0x00,0x09},0x0010,0x1200,0x00AA,0x07,{0x00}},
{0x0027,0x002D,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0020,0x0600,0x0355,0x09,{0x00}},
{0x0028,0x005B,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0E00,0x07C7,0x01,{0x00}},
{0x0029,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x000F,0x0200,0xFFFFFEFE,{0x00,0x09},0x0028,0x0E00,0x0200,0x09,{0x00}},
{0x002A,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x000F,0x0200,0xFFFFFEFE,{0x00,0x09},0x0028,0x0E00,0x0200,0x09,{0x00}},
{0x002B,0x00AA,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0008,0x0C00,0x0355,0x07,{0x00}},
{0x002C,0x0016,0x0005,0x0016,0x0005,0x0000,0x0000,0xFE00,0x0014,0x0100,0xFFF080FE,{0x00,0x09},0x0010,0x1400,0x02AA,0x07,{0x00}},
{0x002D,0x0100,0x0000,0x0100,0x0000,0x0C00,0x0100,0xFFF8,0x0100,0x0200,0xFFFFFEFF,{0x00,0x18},0x0028,0x2000,0x0200,0x00,{0x00}},
{0x002E,0x0005,0x0000,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFFFFFFF,{0x00,0x09},0x0004,0x1900,0x0200,0x07,{0x00}},
{0x002F,0x0011,0x0005,0x0071,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00}},
{0x0000,0x0038,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x0C},0x0032,0x1000,0x0200,0x00,{0x00}},
{0x0001,0x0016,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0002,0x0071,0x0005,0x0071,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0003,0x0011,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0004,0x000B,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0005,0x0005,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0006,0x0000,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0007,0x0100,0x0000,0x0100,0x0000,0x0400,0x0100,0xFFF0,0x0100,0x0200,0xFFFFFEFF,{0x00,0x18},0x0028,0x2000,0x0200,0x00,{0x00}},
{0x0008,0x0100,0x0005,0x0100,0x0005,0x0300,0x0080,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x18},0x0028,0x2000,0x0200,0x00,{0x00}},
{0x0009,0x0038,0x0005,0x0016,0x0005,0x0600,0x0200,0xFFF0,0x0100,0x0200,0xFFFFFFFF,{0x00,0x0C},0x0032,0x1000,0x0200,0x00,{0x00}},
{0x000A,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x000F,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0C00,0x0200,0x00,{0x00}},
{0x000B,0x0016,0x0005,0x0016,0x0005,0x0700,0x0200,0xFFFC,0x0046,0x0155,0xFFFFFFFF,{0x00,0x0F},0x0028,0x0F00,0x0200,0x00,{0x00}},
{0x000C,0x0022,0x0005,0x0055,0x0005,0x0000,0x0000,0xFFFC,0x001E,0x0155,0xFFFFFFFF,{0x00,0x14},0x001E,0x1400,0x00AA,0x01,{0x00}},
{0x000D,0x0016,0x0005,0x0016,0x0005,0x0500,0x0200,0xFFFC,0x0046,0x0200,0xFFFFFFFF,{0x00,0x09},0x001E,0x0C00,0x0200,0x01,{0x00}},
{0x000E,0x0071,0x0011,0x0071,0x0011,0x0700,0x0033,0xFFE0,0x0014,0x0200,0xFFFFFFFF,{0x00,0x06},0x001E,0x1400,0x02AA,0x01,{0x00}},
{0x000F,0x0022,0x0005,0x0055,0x0005,0x0100,0x0000,0xFFC0,0x0014,0x0155,0xFFF080FE,{0x00,0x14},0x001E,0x1400,0x00AA,0x01,{0x00}},
{0x0010,0x0016,0x0005,0x0071,0x0000,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x0A},0x001E,0x1400,0x0200,0x01,{0x00}},
{0x0011,0x0038,0x0005,0x0016,0x0005,0x0100,0x0000,0xFFC0,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x001E,0x1400,0x0200,0x01,{0x00}},
{0x0012,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFFE0,0x0014,0x0000,0x00000001,{0x00,0x09},0x0023,0x1400,0x02AA,0x01,{0x00}},
{0x0013,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0F00,0x0200,0x01,{0x00}},
{0x0014,0x0038,0x0005,0x0016,0x0005,0x0500,0x0000,0xFFE7,0x0050,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1900,0x0200,0x01,{0x00}},
{0x0015,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0019,0x0800,0x0200,0x01,{0x00}},
{0x0016,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0C00,0x0200,0x01,{0x00}},
{0x0017,0x002D,0x0005,0x0016,0x0005,0x0700,0x0000,0xFFFC,0x003C,0x0200,0xFFFFFFFF,{0x00,0x09},0x000A,0x1E00,0x07C7,0x01,{0x00}},
{0x0018,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x0014,0x0200,0x00020000,{0x00,0x09},0x000F,0x0F00,0x0200,0x01,{0x00}},
{0x0019,0x0044,0x0005,0x0016,0x0005,0x0700,0x0100,0xFFF8,0x0046,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1200,0x0200,0x11,{0x00}},
{0x001A,0x002D,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0014,0x0F00,0x0200,0x01,{0x00}},
{0x001B,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0004,0x1900,0x0200,0x07,{0x00}},
{0x001C,0x0400,0x0005,0x0200,0x0005,0x0000,0x0000,0x0000,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0014,0x0F00,0x0200,0x01,{0x00}},
{0x001D,0x0071,0x0011,0x0071,0x0011,0x0700,0x0033,0xFFF8,0x0014,0x0200,0xFFFFFFFF,{0x00,0x06},0x0023,0x1400,0x02AA,0x01,{0x00}},
{0x001E,0x00E3,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0023,0x1400,0x02AA,0x00,{0x00}},
{0x001F,0x0100,0x0005,0x0016,0x0005,0x0000,0x0000,0x0000,0x0032,0x0200,0xFFFFFFFF,{0x00,0x09},0x0014,0x1400,0x0200,0x05,{0x00}},
{0x0020,0x0100,0x0005,0x0016,0x0005,0x2000,0x0000,0x0000,0x0014,0x0200,0xFFFFFFFF,{0x00,0x09},0x0003,0x0C00,0x02AA,0x05,{0x00}},
{0x0021,0x0071,0x0005,0x0016,0x0005,0x2000,0x0000,0xFFE0,0x0014,0x0200,0xFFFFFFFF,{0x00,0x09},0x0014,0x0C00,0x02AA,0x05,{0x00}},
{0x0022,0x0071,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0000,0x00000001,{0x00,0x09},0x0010,0x1000,0x02AA,0x05,{0x00}},
{0x0023,0x0100,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0000,0x00000001,{0x00,0x09},0x0010,0x1000,0x02AA,0x05,{0x00}},
{0x0024,0x00AA,0x0005,0x0016,0x0005,0x2000,0x0000,0xFF00,0x0200,0x0200,0xFFFFFFFF,{0x00,0x09},0x0010,0x1000,0x0400,0x05,{0x00}},
{0x0025,0x0400,0x0000,0x0016,0x0005,0x2000,0x0000,0x0000,0x0155,0x0200,0xFFFFFFFF,{0x00,0x09},0x0010,0x1200,0x009F,0x05,{0x00}},
{0x0026,0x0016,0x0005,0x0016,0x0005,0x0000,0x0000,0xFE00,0x00AA,0x00AA,0xFFF080FE,{0x00,0x09},0x0010,0x1200,0x00AA,0x07,{0x00}},
{0x0027,0x002D,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0020,0x0600,0x0355,0x09,{0x00}},
{0x0028,0x005B,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0028,0x0E00,0x07C7,0x01,{0x00}},
{0x0029,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x000F,0x0200,0xFFFFFEFE,{0x00,0x09},0x0028,0x0E00,0x0200,0x09,{0x00}},
{0x002A,0x0016,0x0005,0x0016,0x0005,0x0100,0x0000,0xFF00,0x000F,0x0200,0xFFFFFEFE,{0x00,0x09},0x0028,0x0E00,0x0200,0x09,{0x00}},
{0x002B,0x00AA,0x0005,0x0016,0x0005,0x0000,0x0000,0xFF00,0x0014,0x0200,0xFFF080FE,{0x00,0x09},0x0008,0x0C00,0x0355,0x07,{0x00}},
{0x002C,0x0016,0x0005,0x0016,0x0005,0x0000,0x0000,0xFE00,0x0014,0x0100,0xFFF080FE,{0x00,0x09},0x0010,0x1400,0x02AA,0x07,{0x00}},
{0x002D,0x0100,0x0000,0x0100,0x0000,0x0C00,0x0100,0xFFF8,0x0100,0x0200,0xFFFFFEFF,{0x00,0x18},0x0028,0x2000,0x0200,0x00,{0x00}},
{0x002E,0x0005,0x0000,0x0016,0x0005,0x0100,0x0000,0xFF80,0x0014,0x0200,0xFFFFFFFF,{0x00,0x09},0x0004,0x1900,0x0200,0x07,{0x00}},
{0x002F,0x0011,0x0005,0x0071,0x0005,0x0700,0x0000,0xFFFC,0x0100,0x0200,0xFFFFFFFF,{0x00,0x09},0x0028,0x1000,0x0200,0x00,{0x00}},
{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00000000,{0x00,0x00},0x0000,0x0000,0x0000,0x00,{0x00} } };


// shadow D41A0 for a save (x64 fixes of dword_0xA4_164x)
void SaveLevelShadow(type_shadow_D41A0_BYTESTR_0* shadow)
{
	//fix for saving
	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
		//if (memory_readable(Entities_EA3E4[indexx]->playerData_0xA4_164x,4))
	{
		if (Entities_EA3E4[indexx]->playerData_0xA4_164x == unk_F42B0x)//0x014F82E8//0x2c75e28-(uint32_t)&D41A0_BYTESTR_0
			Entities_EA3E4[indexx]->playerData_0xA4_164x = (type_str_164*)0x2c75e28;
	}
	//fix for saving

	world_D41A0.typeParamsBase_0x36DF6 = &str_D7BD6[59]; //(x_DWORD)&unk_D7BD6[0x7d6];
	Convert_to_shadow_D41A0_BYTESTR_0(&world_D41A0, shadow);

	//fix for saving
	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
	{
		type_str_164* Zerotype_str_164 = 0;
		if (Entities_EA3E4[indexx]->playerData_0xA4_164x == (type_str_164*)&((int8_t*)Zerotype_str_164)[0x2c75e28])//0x014F82E8//0x2c75e28-(uint32_t)&D41A0_BYTESTR_0
			Entities_EA3E4[indexx]->playerData_0xA4_164x = unk_F42B0x;
	}
	//fix for saving
}

//----- (00055250) --------------------------------------------------------
bool SaveLevelSLEV_55250(uint8_t savefileindex, char* savefileindex2)//236250 //in game save
{
	bool success;
	int temptime; // edi
	long acttime; // eax

	success = false;
	sprintf(printbuffer, "%s/%s%d%s.DAT", SaveDirectory().c_str(), "SLEV", savefileindex + 1, savefileindex2);
	temptime = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393;
	acttime = j___clock();
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393 = acttime - world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393;

	type_shadow_D41A0_BYTESTR_0 shadow_type_D41A0_BYTESTR_0;
	SaveLevelShadow(&shadow_type_D41A0_BYTESTR_0);

	int size = sizeof(shadow_type_D41A0_BYTESTR_0);
	if (DataFileIO::OpenWriteClose_98C48(printbuffer, (uint8_t*)&shadow_type_D41A0_BYTESTR_0, size) == size) success = true;
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393 = temptime;

	return success;
}

//----- (00055320) --------------------------------------------------------
bool SaveLevelSMAP_55320(uint8_t savefileindex, char* savefileindex2)//236320 //file index added for develop
{
	FILE* savesmapfile; // eax
	size_t writedsize; // [esp+40h] [ebp-8h]
	Logger->debug("InGameSave-begin");
	sprintf(printbuffer, "%s/%s%d%s.DAT", SaveDirectory().c_str(), "SMAP", savefileindex + 1, savefileindex2);
	savesmapfile = DataFileIO::CreateOrOpenFile(printbuffer, 546);
	if (savesmapfile)
	{
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapTerrainType_10B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapHeightmap_11B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapShading_12B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapAngle_13B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)secondHeightmap_14B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapEntityIndex_15B4E0, 0x20000);
		writedsize = DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)building_F2CD0x, 4802) != 4802;
		DataFileIO::Close(savesmapfile);
	}
	Logger->debug("InGameSave-end-{}", writedsize);
	return (writedsize == 0);
}
// 10000: using guessed type void /*__noreturn*/ sub_10000();
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00055450) --------------------------------------------------------
bool SaveLevelSVER_55450(uint8_t savefileindex, int32_t levelNumber, char* savefileindex2)//236450 //fileindex added for developing
{
	int32_t data[2]; // [esp+40h] [ebp-8h]
	data[1] = levelNumber;
	data[0] = 15;
	bool success = false;
	sprintf(printbuffer, "%s/%s%d%s.DAT", SaveDirectory().c_str(), "SVER", savefileindex + 1, savefileindex2);
	if (DataFileIO::OpenWriteClose_98C48(printbuffer, (uint8_t*)data, 2*sizeof(int32_t)) == 8)
		success = true;
	return success;
}

//----- (000555D0) --------------------------------------------------------
bool LoadLevel_555D0(uint8_t fileindex, int levelindex, bool loadRegressionTest)//2365d0
{	
	int temp0x219A;
	int temp0x219E;
	int temp0x21A2;
	int temp0x21A6;
	GraphicsStruct_t tempGr;
	DisplayStruct_t tempDisp;
	type_str_0x2192 temp0x2192;
	type_str_0x2196 temp0x2196;
	type_str_0x21AA temp0x21AA;
	type_str_0x21AE temp0x21AE;
	type_str_0x21B2 temp0x21B2;
	type_str_0x21B6 temp0x21B6;

	IsAfterLoad = true;

	bool readSuccess = false;
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		tempGr = world_D41A0.m_GameSettings.m_Graphics;
		tempDisp = world_D41A0.m_GameSettings.m_Display;
		temp0x2192 = world_D41A0.m_GameSettings.graphics_0x2192;
		temp0x2196 = world_D41A0.m_GameSettings.display_0x2196;
		temp0x219A = world_D41A0.dword_0x219A;
		temp0x219E = world_D41A0.dword_0x219E;
		temp0x21A2 = world_D41A0.dword_0x21A2;
		temp0x21A6 = world_D41A0.dword_0x21A6;
		temp0x21AA = world_D41A0.visualSettings_0x21AA;
		temp0x21AE = world_D41A0.miscSettings_0x21AE;
		temp0x21B2 = world_D41A0.screenSettings_0x21B2;
		temp0x21B6 = world_D41A0.detailSettings_0x21B6;
		readSuccess = DataFileIO::TestExistingSaveFile_55750(fileindex, levelindex, loadRegressionTest);
		//adress  23662a
		if (readSuccess)
		{
			readSuccess = LoadLevelSMAP_558E0(fileindex, loadRegressionTest);
			if (readSuccess)
			{
				qmemcpy(
					&gameState_D41A4.savedPlayer_256ar,
					&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc],
					0x84Cu);
				readSuccess = LoadLevelSLEV_55A10(fileindex, loadRegressionTest);
				if (readSuccess)
				{
					ConvertObjectivePointers_55100(2);
					FixPointersAfterLoad_57680();
					CopyPlayerSpellData_549A0(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.str_611, &gameState_D41A4.savedPlayer_256ar.playerData_0x3E6_2BE4_12228.str_611);
					RebuildEntityLists_49F90();
					world_D41A0.freeEntityCount2_0x11e6 = -1;
					CreatePlayerSpellEntities_55AB0(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc]);
					ReloadLevelTmaps_71990();
					FlvInitSet_473B0();
				}
			}
		}
		world_D41A0.m_GameSettings.m_Graphics = tempGr;
		world_D41A0.m_GameSettings.m_Display = tempDisp;
		world_D41A0.m_GameSettings.graphics_0x2192 = temp0x2192;
		world_D41A0.m_GameSettings.display_0x2196 = temp0x2196;
		world_D41A0.dword_0x219A = temp0x219A;
		world_D41A0.dword_0x219E = temp0x219E;
		world_D41A0.dword_0x21A2 = temp0x21A2;
		world_D41A0.dword_0x21A6 = temp0x21A6;
		world_D41A0.visualSettings_0x21AA = temp0x21AA;
		world_D41A0.miscSettings_0x21AE = temp0x21AE;
		world_D41A0.screenSettings_0x21B2 = temp0x21B2;
		world_D41A0.detailSettings_0x21B6 = temp0x21B6;
	}
	return readSuccess;
}

//----- (000558E0) --------------------------------------------------------
bool LoadLevelSMAP_558E0(uint8_t savefileindex, bool loadRegressionTest)//2368e0
{
	Logger->debug("InGameLoad-begin\n");

	//fix
	gameState_D41A4.class3List_38519 = Entities_EA3E4[1];
	//fix

	// FIXME: cannot set this here.
	//        was before: debugafterload = 1;
	//        cannot be: CommandLineParams.DoDebugafterload() = 1;
	gameState_D41A4.setting_30 = (gameState_D41A4.setting_30 & ~0xFFu) | 0x3d;//fix same run after load, the low byte only as DOSBox 2368E0 mem_writeb(0x38cf50 + 0x1e, 0x3d)
	rand2_17B4E0 = 0x21ed;//fix random variable for debugging

	char path[512];
	sprintf(path, "%s", SaveDirectory().c_str());
	if (loadRegressionTest)
	{
		sprintf(path, "%sregressions", CommandLineParams.GetMemimagesPath().c_str());
		if (unitTests)
		{
			sprintf(path, "%s/SAVE", unitTestsPath.c_str());
		}
	}

	sprintf(printbuffer, "%s/%s%d.DAT", path, "SMAP", savefileindex + 1);
	FILE* loadfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (loadfile)
	{
		DataFileIO::Read(loadfile, (uint8_t*)mapTerrainType_10B4E0, 0x10000);
		DataFileIO::Read(loadfile, (uint8_t*)mapHeightmap_11B4E0, 0x10000);
		DataFileIO::Read(loadfile, (uint8_t*)mapShading_12B4E0, 0x10000);
		DataFileIO::Read(loadfile, (uint8_t*)mapAngle_13B4E0, 0x10000);
		DataFileIO::Read(loadfile, (uint8_t*)secondHeightmap_14B4E0, 0x10000);
		DataFileIO::Read(loadfile, (uint8_t*)mapEntityIndex_15B4E0, 0x20000);
		int truesize = DataFileIO::Read(loadfile, (uint8_t*)building_F2CD0x, 4802) == 4802;
		DataFileIO::Close(loadfile);
		if (truesize) {
			Logger->debug("InGameLoad-end-ok\n");
			return 1;
		}
	}
	Logger->debug("InGameLoad-end-error\n");
	return 0;
}

//----- (00055A10) --------------------------------------------------------
bool LoadLevelSLEV_55A10(uint8_t savefileindex, bool loadRegressionTest)//236a10
{
	bool success = false;

	char path[512];
	sprintf(path, "%s", SaveDirectory().c_str());
	if (loadRegressionTest)
	{
		sprintf(path, "%sregressions", CommandLineParams.GetMemimagesPath().c_str());
		if (unitTests)
		{
			sprintf(path, "%s/SAVE", unitTestsPath.c_str());
		}
	}

	sprintf(printbuffer, "%s/%s%d.DAT", path, "SLEV", savefileindex + 1);
	//x64 fix
	uint8_t* D41A0_pointer;
	type_shadow_D41A0_BYTESTR_0 shadow_D41A0_BYTESTR_0;
	D41A0_pointer = (uint8_t*)&shadow_D41A0_BYTESTR_0;
	if (DataFileIO::ReadFileAndDecompress(printbuffer, &D41A0_pointer) == sizeof(type_shadow_D41A0_BYTESTR_0))
	{
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393 = j___clock() - world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.time_393;
		success = true;
	}
	type_str_164* tempEntities_EA3E4_0 = Entities_EA3E4[0]->playerData_0xA4_164x;//fix for x64
	Convert_from_shadow_D41A0_BYTESTR_0(&shadow_D41A0_BYTESTR_0, &world_D41A0);
	Entities_EA3E4[0]->playerData_0xA4_164x =tempEntities_EA3E4_0;//fix for x64
	return success;
}

//----- (00055080) --------------------------------------------------------
bool SaveLevel_55080(uint8_t savefileindex, int32_t LevelNumber, char* savefileindex2)//236080 //filenameindex added for debugging
{
	bool success = false;
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		MarkLoadedTmaps_71930();
		ConvertObjectivePointers_55100(1);
		success = SaveLevelSLEV_55250(savefileindex, savefileindex2);
		if (success)
		{
			success = SaveLevelSMAP_55320(savefileindex, savefileindex2);
			if (success)
				success = SaveLevelSVER_55450(savefileindex, LevelNumber, savefileindex2);
		}
		ConvertObjectivePointers_55100(2);
		RebuildEntityLists_49F90();
		world_D41A0.freeEntityCount2_0x11e6 = -1;
	}
	return success;
}

// SMAP content in the file order
std::array<std::pair<uint8_t*, size_t>, 7> SmapParts()
{
	return { { { mapTerrainType_10B4E0, 0x10000 }, { mapHeightmap_11B4E0, 0x10000 }, { mapShading_12B4E0, 0x10000 },
		{ mapAngle_13B4E0, 0x10000 }, { secondHeightmap_14B4E0, 0x10000 },
		{ (uint8_t*)mapEntityIndex_15B4E0, 0x20000 }, { (uint8_t*)building_F2CD0x, 4802 } } };
}

// save for a recording (SLEV + SMAP), the game state stays unchanged
std::vector<uint8_t> SaveLevelToBuffer()
{
	std::vector<uint8_t> save(sizeof(type_shadow_D41A0_BYTESTR_0));
	auto shadow = (type_shadow_D41A0_BYTESTR_0*)save.data();
	auto live = std::make_unique<type_D41A0_BYTESTR_0>(world_D41A0);
	ConvertObjectivePointers_55100(1);
	SaveLevelShadow(shadow);
	world_D41A0 = *live;
	for (int i = 0; i < 1000; i++)//entity lists as indexes, the shadow keeps 32 bits of a pointer
	{
		shadow->pointers_0x246[i] = world_D41A0.freeEntities_0x246[i] ? world_D41A0.freeEntities_0x246[i] - world_D41A0.entities_0x6E8E : 0;
		shadow->dword_0x11EA[i] = world_D41A0.freeEntities2_0x11EA[i] ? world_D41A0.freeEntities2_0x11EA[i] - world_D41A0.entities_0x6E8E : 0;
	}
	for (const auto& part : SmapParts())
		save.insert(save.end(), part.first, part.first + part.second);
	return save;
}

// load of a recording save at the level start: the state and its pointers only
bool LoadLevelFromBuffer(const std::vector<uint8_t>& save)
{
	size_t pos = sizeof(type_shadow_D41A0_BYTESTR_0);
	for (const auto& part : SmapParts())
		pos += part.second;
	if (save.size() != pos)
		return false;
	pos = sizeof(type_shadow_D41A0_BYTESTR_0);
	for (const auto& part : SmapParts())
	{
		memcpy(part.first, &save[pos], part.second);
		pos += part.second;
	}
	auto shadow = std::make_unique<type_shadow_D41A0_BYTESTR_0>();
	memcpy(shadow.get(), save.data(), sizeof(type_shadow_D41A0_BYTESTR_0));
	auto live = std::make_unique<type_D41A0_BYTESTR_0>(world_D41A0);
	Convert_from_shadow_D41A0_BYTESTR_0(shadow.get(), &world_D41A0);
	for (int i = 0; i < 1000; i++)
	{
		world_D41A0.freeEntities_0x246[i] = shadow->pointers_0x246[i] && shadow->pointers_0x246[i] < 1000 ? &world_D41A0.entities_0x6E8E[shadow->pointers_0x246[i]] : nullptr;
		world_D41A0.freeEntities2_0x11EA[i] = shadow->dword_0x11EA[i] && shadow->dword_0x11EA[i] < 1000 ? &world_D41A0.entities_0x6E8E[shadow->dword_0x11EA[i]] : nullptr;
	}
	ConvertObjectivePointers_55100(2);
	for (int i = 1; i <= world_D41A0.countStageVars_0x36E00; i++)//ConvertObjectivePointers_55100(2) keeps these as offsets, as the original (0x2361C9)
	{
		auto& stageVar = world_D41A0.StageVars2_0x365F4[i];
		uint8_t type = stageVar.index_0x3647A_0;
		if ((type >= 3 && type <= 5 || type == 8 || type == 9) && !(stageVar.stage_0x3647A_1 & 2)
			&& stageVar.target_0x3647C_4.dword && stageVar.target_0x3647C_4.dword % sizeof(type_shadow_str_0x6E8E) == 0 && stageVar.target_0x3647C_4.dword < 1000 * sizeof(type_shadow_str_0x6E8E))
			stageVar.target_0x3647C_4.pointer_0x6E8E = &world_D41A0.entities_0x6E8E[stageVar.target_0x3647C_4.dword / sizeof(type_shadow_str_0x6E8E)];
	}
	FixPointersAfterLoad_57680();
	// live values: pointers the fixes overwrite and the settings, as LoadLevel_555D0
	world_D41A0.entities_0x6E8E[0].playerData_0xA4_164x = live->entities_0x6E8E[0].playerData_0xA4_164x;
	world_D41A0.dynamicLights_0x3664C[0].event_A.pointer_0x6E8E = live->dynamicLights_0x3664C[0].event_A.pointer_0x6E8E;
	world_D41A0.typeParamsBase_0x36DF6 = live->typeParamsBase_0x36DF6;
	world_D41A0.m_GameSettings = live->m_GameSettings;
	world_D41A0.dword_0x219A = live->dword_0x219A;
	world_D41A0.dword_0x219E = live->dword_0x219E;
	world_D41A0.dword_0x21A2 = live->dword_0x21A2;
	world_D41A0.dword_0x21A6 = live->dword_0x21A6;
	world_D41A0.visualSettings_0x21AA = live->visualSettings_0x21AA;
	world_D41A0.miscSettings_0x21AE = live->miscSettings_0x21AE;
	world_D41A0.screenSettings_0x21B2 = live->screenSettings_0x21B2;
	world_D41A0.detailSettings_0x21B6 = live->detailSettings_0x21B6;
	return true;
}

//----- (00049270) --------------------------------------------------------
void GenerateLevelFeatures_49270(Type_Level_2FECE* terrain)//22a270
{
	SetStagetagForTermod_49830(terrain);
	//adress 22A27D
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A280, CommandLineParams.DoDebugafterload());
	}
	GenerateEvents_49290(terrain, 1, 640, 480);
	if (CommandLineParams.DoDebugSequences()) {
		add_compare(0x22A288, CommandLineParams.DoDebugafterload());
	}
}

static std::string MapTypeFolder(MapType_t mt)
{
	switch (mt)
	{
	case MapType_t::Day:   return "DAY";
	case MapType_t::Night: return "NIGHT";
	case MapType_t::Cave:  return "CAVE";
	default:               return "DAY";
	}
}

static std::string DatBaseName(MapType_t mt)
{
	switch (mt)
	{
	case MapType_t::Day:   return "HSPRD0-0.DAT";
	case MapType_t::Night: return "HSPRN0-0.DAT";
	case MapType_t::Cave:  return "HSPRC0-0.DAT";
	default:               return "HSPRD0-0.DAT";
	}
}

static uint8_t NearestPaletteIndex(uint8_t r, uint8_t g, uint8_t b,
	const TColor* palette, int paletteSize)
{
	int   bestIdx = 0;
	int   bestDist = INT_MAX;
	for (int i = 0; i < paletteSize; ++i)
	{
		int dr = (int)r - (int)palette[i].red*4;
		int dg = (int)g - (int)palette[i].green*4;
		int db = (int)b - (int)palette[i].blue*4;
		int dist = dr * dr + dg * dg + db * db;
		if (dist < bestDist)
		{
			bestDist = bestIdx = i;
			bestIdx = i;
			bestDist = dist;
			if (dist == 0) break;
		}
	}
	return (uint8_t)bestIdx;
}

static void EncodeRLE(const uint8_t* raw, const uint8_t* hasAlpha,
	int w, int h,
	std::vector<uint8_t>& out)
{
	for (int y = 0; y < h; ++y)
	{
		const uint8_t* row = raw + y * w;
		const uint8_t* alphaRow = hasAlpha + y * w;
		int x = 0;
		while (x < w)
		{
			if (!alphaRow[x])
			{
				int skipStart = x;
				int skipLen = 0;
				while (x < w && !alphaRow[x] && skipLen < 127)
				{
					++skipLen;
					++x;
				}
				out.push_back((uint8_t)(-skipLen));
				continue;
			}
			int runStart = x;
			int runLen = 0;
			while (x < w && runLen < 127 && alphaRow[x])
			{
				++runLen;
				++x;
			}
			if (runLen > 0)
			{
				out.push_back((uint8_t)runLen);
				for (int i = runStart; i < runStart + runLen; ++i)
					out.push_back(row[i]);
			}
		}
		out.push_back(0x00);
	}
}

static bool PatchSprite(bitmap_pos_struct2_t* tabBase,
	uint8_t* datBase,
	size_t& datUsed,
	size_t          datCapacity,   // přidaný parametr
	int             spriteIndex,
	const char* pngPath,
	const TColor* palette,
	int             paletteSize)
{
	RGBAImage img;
	if (!BitmapIO::ReadImagePNG(pngPath, img))
	{
		Logger->warn("LoadFixedMenuGraphics: failed to load '{}'.", pngPath);
		return false;
	}

	if (img.width > 255 || img.height > 255)
	{
		Logger->warn("LoadFixedMenuGraphics: sprite '{}' too large ({}x{} > 255x255), skipped.",
			pngPath, img.width, img.height);
		return false;
	}
	const int numPixels = img.width * img.height;
	std::vector<uint8_t> indexed(numPixels);
	std::vector<uint8_t> opaque(numPixels, 1);

	for (int i = 0; i < numPixels; ++i)
	{
		uint8_t r = img.pixels[4 * i + 0];
		uint8_t g = img.pixels[4 * i + 1];
		uint8_t b = img.pixels[4 * i + 2];
		uint8_t a = img.pixels[4 * i + 3];

		if (img.hasAlpha && a < 128)
		{
			opaque[i] = 0;
			indexed[i] = 0;
		}
		else
		{
			opaque[i] = 1;
			indexed[i] = NearestPaletteIndex(r, g, b, palette, paletteSize);
		}
	}
	std::vector<uint8_t> rleData;
	rleData.reserve(numPixels + img.height * 2);
	EncodeRLE(indexed.data(), opaque.data(), img.width, img.height, rleData);
	size_t newOffset = datUsed;
	assert(datUsed + rleData.size() < datCapacity);  // místo < 3000000
	memcpy(datBase + newOffset, rleData.data(), rleData.size());
	datUsed += rleData.size();
	tabBase[spriteIndex].data_0 = (uint32_t)newOffset;
	tabBase[spriteIndex].width_4 = (uint8_t)img.width;
	tabBase[spriteIndex].height_5 = (uint8_t)img.height;
	Logger->debug("LoadFixedMenuGraphics: patched sprite {} from '{}' "
		"({}x{}, {} RLE bytes at offset {}).",
		spriteIndex, pngPath, img.width, img.height, rleData.size(), newOffset);
	return true;
}

static uint8_t* g_patchedFontDataBuffer = nullptr;
static uint8_t* g_patchedFontDataBuffer2 = nullptr;
static uint8_t* g_patchedFontDataBuffer3 = nullptr;
static uint8_t* g_patchedFontDataBuffer4 = nullptr;
static bool PatchFont(
	uint8_t* datBase,
	size_t& datUsed,
	size_t               datCapacity,
	bitmap_pos_struct_t* fontStruct,
	int                  fontIndex,
	const RGBAImage& img,
	const TColor* palette,
	int                  paletteSize)
{
	if (img.width > 255 || img.height > 255)
	{
		Logger->warn("LoadFixedFonts: font index {} too large ({}x{}), keeping original.",
			fontIndex, img.width, img.height);
		return false;
	}
	const int numPixels = img.width * img.height;
	std::vector<uint8_t> indexed(numPixels);
	std::vector<uint8_t> opaque(numPixels, 1);
	for (int i = 0; i < numPixels; ++i)
	{
		uint8_t r = img.pixels[4 * i + 0];
		uint8_t g = img.pixels[4 * i + 1];
		uint8_t b = img.pixels[4 * i + 2];
		uint8_t a = img.pixels[4 * i + 3];
		if (img.hasAlpha && a < 128)
		{
			opaque[i] = 0;
			indexed[i] = 0;
		}
		else
		{
			opaque[i] = 1;
			indexed[i] = NearestPaletteIndex(r, g, b, palette, paletteSize);
		}
	}
	std::vector<uint8_t> rleData;
	rleData.reserve(numPixels + img.height * 2);
	EncodeRLE(indexed.data(), opaque.data(), img.width, img.height, rleData);
	if (datUsed + rleData.size() > datCapacity)
	{
		Logger->error("LoadFixedFonts: font buffer overflow at index {}, keeping original.", fontIndex);
		return false;
	}
	size_t newOffset = datUsed;
	memcpy(datBase + newOffset, rleData.data(), rleData.size());
	datUsed += rleData.size();
	fontStruct[fontIndex].width_4 = (uint8_t)img.width;
	fontStruct[fontIndex].height_5 = (uint8_t)img.height;
	Logger->debug("LoadFixedFonts: patched font {} ({}x{}, {} RLE bytes at offset {}).",
		fontIndex, img.width, img.height, rleData.size(), newOffset);
	return true;
}

void LoadFixedFonts(int fontStructIndex, char* type)
{
	char dataPath[MAX_PATH];
	uint8_t** tempPal = xadatapald0dat2.colorPalette_var28;
	std::string subFolder;
	uint8_t* fontDatBase = NULL;
	bitmap_pos_struct2_t* datTabOffset0 = NULL;
	bitmap_pos_struct2_t* datTabOffset1 = NULL;
	bitmap_pos_struct_t* fontStruct = NULL;
	uint8_t** patchedBufferPtr = nullptr;
	filearray_struct* structForIndex2 = NULL;
	int createIndexType = 0;
	int charIndexOffset = 0;
	switch (fontStructIndex)
	{
	case 0:
		fontStruct = fontSprites_17DEC0;
		fontDatBase = menuState_17DE38.fontData_17DE54;
		datTabOffset0 = menuState_17DE38.fontTab_17DEC0;
		datTabOffset1 = menuState_17DE38.fontTabEnd_17DEC4;
		patchedBufferPtr = &g_patchedFontDataBuffer;
		break;
	case 1:
		fontStruct = font2Sprites_17DEC8;
		fontDatBase = menuState_17DE38.font2Data_17DE58;
		datTabOffset0 = menuState_17DE38.font2Tab_17DEC8;
		datTabOffset1 = menuState_17DE38.font2TabEnd_17DECC;
		patchedBufferPtr = &g_patchedFontDataBuffer2;
		break;
	case 2:
		fontStruct = *filearray_2aa18c[filearrayindex_HFONT3DATTAB].posistruct;
		fontDatBase = HFONT3DAT_BEGIN_BUFFER;
		createIndexType = 1;
		structForIndex2 = &filearray_2aa18c[filearrayindex_HFONT3DATTAB];
		patchedBufferPtr = &g_patchedFontDataBuffer3;
		charIndexOffset = 1;
		break;
	case 3:
		fontStruct = *filearray_2aa18c[filearrayindex_FONTS1DATTAB].posistruct;
		fontDatBase = (*filearray_2aa18c[filearrayindex_FONTS1DATTAB].posistruct)->data;
		createIndexType = 1;
		structForIndex2 = &filearray_2aa18c[filearrayindex_FONTS1DATTAB];
		patchedBufferPtr = &g_patchedFontDataBuffer4;
		charIndexOffset = 1;
		break;
	}
	if (!fontStruct || !patchedBufferPtr)
	{
		Logger->warn("LoadFixedFonts: invalid fontStructIndex {}.", fontStructIndex);
		return;
	}
	if (!strcmp(type, "intro"))
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SCREENS/HSCREEN0.DAT");
		LoadAndDecompressDatFile_7AA70(dataPath, *xadatapald0dat2.colorPalette_var28, 0x17C118, 0x300);
		subFolder = "intro";
	}
	else if (!strcmp(type, "4b")) { subFolder = "4b"; }
	else if (!strcmp(type, "6b")) { subFolder = "6b"; }
	else if (!strcmp(type, "6c")) { subFolder = "4b"; }
	else if (!strcmp(type, "HFONT3")) { subFolder = "6b"; }
	else if (!strcmp(type, "FONT1")) { subFolder = "FONT1"; }
	const TColor* palette = (const TColor*)*xadatapald0dat2.colorPalette_var28;
	xadatapald0dat2.colorPalette_var28 = tempPal;
	const int paletteSize = 256;
	if (!palette)
	{
		Logger->warn("LoadFixedFonts: palette not loaded, skipping.");
		return;
	}
	if (subFolder.empty())
	{
		Logger->warn("LoadFixedFonts: unknown type '{}', skipping.", type);
		return;
	}
	const int numSprites = 256;
	// Save original pointers before index rebuild overwrites them
	std::vector<uint8_t*> originalDataPtrs(numSprites);
	std::vector<uint8_t>  originalWidths(numSprites);
	std::vector<uint8_t>  originalHeights(numSprites);
	for (int i = 0; i < numSprites; ++i)
	{
		originalDataPtrs[i] = fontStruct[i].data;
		originalWidths[i] = fontStruct[i].width_4;
		originalHeights[i] = fontStruct[i].height_5;
	}
	std::string fontsDir = GetSubDirectoryPath(extendedFontsFolder.c_str(), subFolder.c_str());
	if (fontsDir.empty() || !DirExists(fontsDir.c_str()))
	{
		Logger->debug("LoadFixedFonts: patch folder '{}' not found, nothing to do.", fontsDir);
		return;
	}
	char patchDirBuf[512];
	strncpy(patchDirBuf, fontsDir.c_str(), sizeof(patchDirBuf) - 1);
	patchDirBuf[sizeof(patchDirBuf) - 1] = '\0';
	dirsstruct files = getListDir(patchDirBuf);
	struct PatchedEntry
	{
		int     charIndex;
		size_t  offset;
		uint8_t width;
		uint8_t height;
	};
	struct PngEntry
	{
		int       charIndex;
		RGBAImage img;
	};
	std::vector<PngEntry>     loadedPngs;
	std::vector<PatchedEntry> patchedList;
	size_t datCapacity = 0;
	for (int f = 0; f < files.number; ++f)
	{
		std::string filename = files.dir[f];
		if (filename.size() < 5) continue;
		std::string ext = filename.substr(filename.size() - 4);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext != ".png") continue;
		std::string indexStr = filename.substr(0, filename.size() - 4);
		int charIndex = -1;
		try { charIndex = std::stoi(indexStr); }
		catch (...)
		{
			Logger->warn("LoadFixedFonts: cannot parse index from '{}', skipped.", filename);
			continue;
		}
		if (charIndex < 0 || charIndex >= numSprites)
		{
			Logger->warn("LoadFixedFonts: index {} out of range [0,{}), skipped.", charIndex, numSprites);
			continue;
		}
		charIndex += charIndexOffset;
		RGBAImage img;
		std::string fullPath = fontsDir + "/" + filename;
		if (!BitmapIO::ReadImagePNG(fullPath.c_str(), img))
		{
			Logger->warn("LoadFixedFonts: failed to load '{}', skipped.", fullPath);
			continue;
		}
		if (img.width > 255 || img.height > 255)
		{
			Logger->warn("LoadFixedFonts: font '{}' too large ({}x{}), skipped.", fullPath, img.width, img.height);
			continue;
		}
		datCapacity += img.width * img.height + img.height * 2; // worst case RLE per glyph
		loadedPngs.push_back({ charIndex, std::move(img) });
	}
	if (loadedPngs.empty())
	{
		Logger->debug("LoadFixedFonts: no valid PNG files found for type '{}', nothing to do.", type);
		return;
	}
	datCapacity += 1024; // small safety margin
	if (*patchedBufferPtr)
	{
		FreeMem_83E80(*patchedBufferPtr);
		*patchedBufferPtr = nullptr;
	}
	uint8_t* patchedBuffer = (uint8_t*)Malloc_83CD0(datCapacity);
	if (!patchedBuffer)
	{
		Logger->error("LoadFixedFonts: failed to allocate {} bytes for patch buffer.", datCapacity);
		return;
	}
	*patchedBufferPtr = patchedBuffer;
	size_t datUsed = 0;
	int patchedCount = 0;
	for (auto& entry : loadedPngs)
	{
		size_t offsetBefore = datUsed;
		if (PatchFont(patchedBuffer, datUsed, datCapacity,
			fontStruct, entry.charIndex, entry.img, palette, paletteSize))
		{
			PatchedEntry pe;
			pe.charIndex = entry.charIndex;
			pe.offset = offsetBefore;
			pe.width = fontStruct[entry.charIndex].width_4;
			pe.height = fontStruct[entry.charIndex].height_5;
			patchedList.push_back(pe);
			++patchedCount;
		}
	}
	if (patchedCount == 0)
	{
		Logger->debug("LoadFixedFonts: no fonts patched for type '{}', all original kept.", type);
		FreeMem_83E80(patchedBuffer);
		*patchedBufferPtr = nullptr;
		return;
	}
	// Rebuild index table so pointers are recalculated from original base
	if (createIndexType == 0)
	{
		if (videoMode_180660 & 1)
			CreateIndexDatTabDoubled_98709(datTabOffset0, datTabOffset1, fontDatBase, fontStruct);
		else
			CreateIndexDatTab_9874D(datTabOffset0, datTabOffset1, fontDatBase, fontStruct);
	}
	else
		CreateIndexes_6EB90(structForIndex2);
	// Restore original pointers for glyphs that were NOT patched
	bool patchedSet[256] = {};
	for (auto& pe : patchedList)
		patchedSet[pe.charIndex] = true;
	for (int i = 0; i < numSprites; ++i)
	{
		if (!patchedSet[i])
		{
			fontStruct[i].data = originalDataPtrs[i];
			fontStruct[i].width_4 = originalWidths[i];
			fontStruct[i].height_5 = originalHeights[i];
		}
	}
	// Point patched glyphs into patch buffer and apply resolution scaling
	for (auto& pe : patchedList)
	{
		fontStruct[pe.charIndex].data = patchedBuffer + pe.offset;
		fontStruct[pe.charIndex].width_4 = pe.width;
		fontStruct[pe.charIndex].height_5 = pe.height;
		if (videoMode_180660 & 1)
		{
			fontStruct[pe.charIndex].width_4 *= 2;
			fontStruct[pe.charIndex].height_5 *= 2;
		}
	}
	Logger->info("LoadFixedFonts: patched {}/{} font(s) for type '{}', {} kept original.",
		patchedCount, numSprites, type, numSprites - patchedCount);
}

void LoadFixedMenuGraphics()
{
	char dataPath[MAX_PATH];
	uint8_t** tempPal = xadatapald0dat2.colorPalette_var28;
	switch (world_D41A0.terrain_2FECE.MapType)
	{
		case MapType_t::Day:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALD-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
		}
		break;
		case MapType_t::Night:
		{
			if (world_D41A0.terrain_2FECE.levelType_0x2FED2 & 2)
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALF-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			}
			else
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALN-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			}
			break;
		}
		case MapType_t::Cave:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALC-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
			break;
		}
	}

	const TColor* palette = (const TColor*)*xadatapald0dat2.colorPalette_var28;
	xadatapald0dat2.colorPalette_var28 = tempPal;
	const int paletteSize = 256;
	if (!palette)
	{
		Logger->warn("LoadFixedMenuGraphics: palette not loaded, skipping.");
		return;
	}
	uint8_t* datBase = HSPRD00DAT_BEGIN_BUFFER;
	bitmap_pos_struct2_t* tabBase = HSPRD00TAB_BEGIN_BUFFER;
	bitmap_pos_struct2_t* tabEnd = HSPRD00TAB_END_BUFFER;
	const int             numSprites = (int)(tabEnd - tabBase);

	if (!datBase || !tabBase || numSprites <= 0)
	{
		Logger->warn("LoadFixedMenuGraphics: buffers not loaded, skipping.");
		return;
	}
	int lastIdx = 0;
	for (int i = 0; i < numSprites; ++i)
		if (tabBase[i].data_0 > tabBase[lastIdx].data_0)
			lastIdx = i;
	uint8_t* p = HSPRD00DAT_BEGIN_BUFFER + tabBase[lastIdx].data_0;
	for (int row = 0; row < tabBase[lastIdx].height_5; )
	{
		uint8_t b = *p++;
		if (b == 0x00) { row++; continue; }
		if ((b & 0x80) == 0) p += b;
	}
	size_t datUsed = (size_t)(p - HSPRD00DAT_BEGIN_BUFFER);
	Logger->debug("LoadFixedMenuGraphics: DAT buffer used size: {} bytes.", datUsed);

	const size_t extraCapacity = 1024 * 1024;
	const size_t datCapacity = datUsed + extraCapacity;
	uint8_t* newDatBuffer = (uint8_t*)Malloc_83CD0(datCapacity);
	if (!newDatBuffer)
	{
		Logger->error("LoadFixedMenuGraphics: failed to allocate extended DAT buffer.");
		return;
	}
	memcpy(newDatBuffer, HSPRD00DAT_BEGIN_BUFFER, datUsed);
	FreeMem_83E80(HSPRD00DAT_BEGIN_BUFFER);
	HSPRD00DAT_BEGIN_BUFFER = newDatBuffer;
	datBase = newDatBuffer;

	MapType_t   mt = world_D41A0.terrain_2FECE.MapType;
	std::string datName = DatBaseName(mt);
	std::string mapFolder = MapTypeFolder(mt);
	std::string patchDir = GetSubDirectoryPath(fixedMenuGraphicsFolder.c_str(), mapFolder.c_str());
	if (patchDir.empty() || !DirExists(patchDir.c_str()))
	{
		Logger->debug("LoadFixedMenuGraphics: patch folder '{}' not found, nothing to do.", patchDir);
		return;
	}
	char patchDirBuf[512];
	strncpy(patchDirBuf, patchDir.c_str(), sizeof(patchDirBuf) - 1);
	patchDirBuf[sizeof(patchDirBuf) - 1] = '\0';
	dirsstruct files = getListDir(patchDirBuf);
	std::string prefix = std::string(datName) + "_";
	int patchedCount = 0;
	for (int f = 0; f < files.number; ++f)
	{
		std::string filename = files.dir[f];
		if (filename.size() <= prefix.size() + 4) continue;
		if (filename.rfind(prefix, 0) != 0)        continue;
		std::string ext = filename.substr(filename.size() - 4);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext != ".png") continue;
		std::string indexStr = filename.substr(prefix.size(),
			filename.size() - prefix.size() - 4);
		int spriteIndex = -1;
#ifndef __ANDROID__
		try
#endif
		{ spriteIndex = std::stoi(indexStr); }
#ifndef __ANDROID__
		catch (...) {
			Logger->warn("LoadFixedMenuGraphics: cannot parse index from '{}', skipped.", filename);
			continue;
		}
#endif
		if (spriteIndex < 0 || spriteIndex >= numSprites)
		{
			Logger->warn("LoadFixedMenuGraphics: index {} out of range [0,{}), skipped.",
				spriteIndex, numSprites);
			continue;
		}
		std::string fullPath = patchDir + "/" + filename;
		if (PatchSprite(tabBase, datBase, datUsed, datCapacity,
			spriteIndex, fullPath.c_str(), palette, paletteSize))
		{
			++patchedCount;
		}
	}
	if (patchedCount == 0)
	{
		Logger->debug("LoadFixedMenuGraphics: no sprites patched for map type '{}'.", mapFolder);
		return;
	}
	CreateIndexDatTab_9874D(
		HSPRD00TAB_BEGIN_BUFFER,
		HSPRD00TAB_END_BUFFER,
		HSPRD00DAT_BEGIN_BUFFER,
		posistruct5);
	Logger->info("LoadFixedMenuGraphics: patched {} sprite(s) for map type '{}'.",
		patchedCount, mapFolder);
}

//----- (00047160) --------------------------------------------------------
void LoadSpr_47160()//228160
{
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Day)
	{
		sprintf(xadatamsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRD0-0.DAT");
		sprintf(xadatamsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRD0-0.TAB");
		sprintf(xadatahsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRD0-0.DAT");
		sprintf(xadatahsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRD0-0.TAB");
	}
	else if (world_D41A0.terrain_2FECE.MapType == MapType_t::Night)
	{
		sprintf(xadatamsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRN0-0.DAT");
		sprintf(xadatamsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRN0-0.TAB");
		sprintf(xadatahsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRN0-0.DAT");
		sprintf(xadatahsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRN0-0.TAB");
	}
	else if (world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
	{
		sprintf(xadatamsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRC0-0.DAT");
		sprintf(xadatamsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRC0-0.TAB");
		sprintf(xadatahsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRC0-0.DAT");
		sprintf(xadatahsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRC0-0.TAB");
	}

	memcpy(pstr[psxadatamsprd00dat].path, xadatamsprd00dat.path, strlen(xadatamsprd00dat.path));
	memcpy(pstr[psxadatamsprd00tab].path, xadatamsprd00tab.path, strlen(xadatamsprd00tab.path));
	memcpy(pstr[psxadatahsprd00dat].path, xadatahsprd00dat.path, strlen(xadatahsprd00dat.path));
	memcpy(pstr[psxadatahsprd00tab].path, xadatahsprd00tab.path, strlen(xadatahsprd00tab.path));

	if (videoMode_180660 == 1)
	{
		DataFileIO::LoadFileArray_84250(psxadatamsprd00dat);
		filearray_2aa18c[filearrayindex_MSPRD00DATTAB] = { &MSPRD00TAB_BEGIN_BUFFER,&MSPRD00TAB_END_BUFFER,&MSPRD00DAT_BEGIN_BUFFER,&posistruct5 };
		if (memoryBufferAlloc_E9C3C)
			FreeMem_83E80(memoryBufferAlloc_E9C3C);
		memoryBufferAlloc_E9C3C = (uint8_t*)Malloc_83CD0(3000000);
		ptrMemoryBuffer_E9C3C = &memoryBufferAlloc_E9C3C[200000];
		help_VGA_type_resolution = 1;
	}
	else
	{
		DataFileIO::LoadFileArray_84250(psxadatahsprd00dat);//here is loading

		if(fixedMenuGraphics && !(videoMode_180660 & 1))
			LoadFixedMenuGraphics();

		filearray_2aa18c[filearrayindex_MSPRD00DATTAB] = { &HSPRD00TAB_BEGIN_BUFFER,&HSPRD00TAB_END_BUFFER,&HSPRD00DAT_BEGIN_BUFFER,&posistruct5 };
		if (memoryBufferAlloc_E9C3C)
			FreeMem_83E80(memoryBufferAlloc_E9C3C);
		memoryBufferAlloc_E9C3C = (uint8_t*)Malloc_83CD0(3000000);
		ptrMemoryBuffer_E9C3C = &memoryBufferAlloc_E9C3C[200000];
		CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_HFONT3DATTAB]);//2aa1d4
		help_VGA_type_resolution = 8;

		if (extendedFonts)
			LoadFixedFonts(2, (char*)"HFONT3");
	}
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_MSPRD00DATTAB]);//2aa1bc
	LoadTextureData(videoMode_180660, world_D41A0.terrain_2FECE.MapType, pdwScreenBuffer_351628);//ok
}

int debugcounter_236100 = 0;
//----- (00055100) --------------------------------------------------------
void ConvertObjectivePointers_55100(char a1)//236100
{
	signed int v2; // eax
	#if defined(x32_BIT_ENVIRONMENT)
		typedef int32_t xBITINT;
	#else
		typedef int64_t xBITINT;
	#endif
	xBITINT v3;
	signed int v4; // ebx
	xBITINT v5; // esi
	unsigned __int8 v7; // cl
	signed int i; // ebx
	unsigned __int8 v9; // al
	//type_entity_0x6E8E* v11; // ecx
	signed int j; // ebx

	if (a1 == 1)
		v2 = -1;
	else
		v2 = 1;

	//fix !!!!
	int8_t* ZeroPointer=0;
	v3 = v2 * ((int8_t*)world_D41A0.entities_0x6E8E- ZeroPointer);
	v4 = 0;
	v5 = v2 * ((int8_t*)world_D41A0.terrain_2FECE.entity_0x30311 - ZeroPointer);
	//fix

	while (v4 < world_D41A0.stageIndex_0x36E01)
	{
		if (world_D41A0.stages_0x3654C[v4].target_36552.dword)
		{
			v7 = world_D41A0.stages_0x3654C[v4].stages_3654C_byte0;
			if ((v7 < 5) && (v7 > 0))
			{
				if (!(world_D41A0.stages_0x3654C[v4].flags_3654D & 1))
				{
					if ((v7 == 1) || (v7 == 2) || (v7 == 4))
					{
						type_entity_0x30311* temp_0x30311 = world_D41A0.stages_0x3654C[v4].target_36552.ptr0x30311;
						if (v2 == -1)
						{
							int diff = world_D41A0.stages_0x3654C[v4].target_36552.ptr0x30311 - world_D41A0.terrain_2FECE.entity_0x30311;
							int sizediff = diff * sizeof(type_entity_0x30311);
							if (((char*)world_D41A0.stages_0x3654C[v4].target_36552.ptr0x30311 - (char*)world_D41A0.terrain_2FECE.entity_0x30311) % sizeof(type_entity_0x30311) > 0)
								allert_error();
							world_D41A0.stages_0x3654C[v4].target_36552.dword = sizediff;
						}
						else
						{
							int count = world_D41A0.stages_0x3654C[v4].target_36552.dword / sizeof(type_entity_0x30311);
							int countadd = world_D41A0.stages_0x3654C[v4].target_36552.dword % sizeof(type_entity_0x30311);
							if (countadd > 0)allert_error();
							world_D41A0.stages_0x3654C[v4].target_36552.ptr0x30311 = &world_D41A0.terrain_2FECE.entity_0x30311[count];
						}
#ifdef x32_BIT_ENVIRONMENT
						if (world_D41A0.stages_0x3654C[v4].target_36552.ptr0x30311 != (type_entity_0x30311*)((uint8_t*)temp_0x30311 + (xBITINT)v5))
							allert_error();  // only for x86
#endif
					}
				}
				else
				{
					type_entity_0x6E8E* temp_0x6E8E = world_D41A0.stages_0x3654C[v4].target_36552.ptr0x6E8E;
					if (v2 == -1)
					{
						int diff = world_D41A0.stages_0x3654C[v4].target_36552.ptr0x6E8E - world_D41A0.entities_0x6E8E;
						int sizediff = diff * sizeof(type_shadow_str_0x6E8E);
						if (((char*)world_D41A0.stages_0x3654C[v4].target_36552.ptr0x6E8E - (char*)world_D41A0.entities_0x6E8E) % sizeof(type_entity_0x6E8E) > 0)
							allert_error();
						world_D41A0.stages_0x3654C[v4].target_36552.dword = sizediff;
					}
					else
					{
						int count = world_D41A0.stages_0x3654C[v4].target_36552.dword / sizeof(type_shadow_str_0x6E8E);
						int countadd = world_D41A0.stages_0x3654C[v4].target_36552.dword % sizeof(type_shadow_str_0x6E8E);
						if (countadd > 0)allert_error();
						world_D41A0.stages_0x3654C[v4].target_36552.ptr0x6E8E = &world_D41A0.entities_0x6E8E[count];
					}
#ifdef x32_BIT_ENVIRONMENT
					if (world_D41A0.stages_0x3654C[v4].target_36552.ptr0x6E8E != (type_entity_0x6E8E*)((uint8_t*)temp_0x6E8E + (xBITINT)v3))
						allert_error();  // only for x86
#endif
				}
			}
		}
		v4++;
	}
	//2361fc
	for (i = 1; ; ++i)
	{
		if (i > world_D41A0.countStageVars_0x36E00)
			break;
		v9 = world_D41A0.StageVars2_0x365F4[i].index_0x3647A_0;
		if (v9 >= 3u && (v9 <= 5u || v9 >= 8u && v9 <= 9u))
		{
			if (!(world_D41A0.StageVars2_0x365F4[i].stage_0x3647A_1 & 2))
			{
				type_entity_0x6E8E* temp_0x6E8E = world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.pointer_0x6E8E;
				if (temp_0x6E8E >= Entities_EA3E4[0] && temp_0x6E8E < Entities_EA3E4[1000])
				{
					if (v2 == -1)
					{
						int diff = world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.pointer_0x6E8E - world_D41A0.entities_0x6E8E;
						int sizediff = diff * sizeof(type_shadow_str_0x6E8E);
						if (((char*)world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.pointer_0x6E8E - (char*)world_D41A0.entities_0x6E8E) % sizeof(type_entity_0x6E8E) > 0)
							allert_error();
						world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.dword = sizediff;
					}
					else
					{
						int count = world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.dword / sizeof(type_shadow_str_0x6E8E);
						int countadd = world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.dword % sizeof(type_shadow_str_0x6E8E);
						if (countadd > 0)allert_error();
						world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.pointer_0x6E8E = &world_D41A0.entities_0x6E8E[count];
					}
#ifdef x32_BIT_ENVIRONMENT
					if (world_D41A0.StageVars2_0x365F4[i].target_0x3647C_4.pointer_0x6E8E != (type_entity_0x6E8E*)((uint8_t*)temp_0x6E8E + (xBITINT)v3))
						allert_error();  // only for x86
#endif
				}
			}
		}
	}

	//23620a
	for (j = 1; j < 50; j++)
	{
		if (world_D41A0.dynamicLights_0x3664C[j].active_0)
		{
			type_entity_0x6E8E* temp_0x6E8E = world_D41A0.dynamicLights_0x3664C[j].event_A.pointer_0x6E8E;
			if (v2 == -1)
			{
				int diff = world_D41A0.dynamicLights_0x3664C[j].event_A.pointer_0x6E8E - world_D41A0.entities_0x6E8E;
				int sizediff = diff * sizeof(type_shadow_str_0x6E8E);
				if (((char*)world_D41A0.dynamicLights_0x3664C[j].event_A.pointer_0x6E8E - (char*)world_D41A0.entities_0x6E8E) % sizeof(type_entity_0x6E8E) > 0)
					allert_error();
				world_D41A0.dynamicLights_0x3664C[j].event_A.dword = sizediff;//0x36656
			}
			else
			{
				int count = world_D41A0.dynamicLights_0x3664C[j].event_A.dword / sizeof(type_shadow_str_0x6E8E);
				int countadd = world_D41A0.dynamicLights_0x3664C[j].event_A.dword % sizeof(type_shadow_str_0x6E8E);
				if (countadd > 0)allert_error();
				world_D41A0.dynamicLights_0x3664C[j].event_A.pointer_0x6E8E = &world_D41A0.entities_0x6E8E[count];//0x36656
			}
#ifdef x32_BIT_ENVIRONMENT
			if (world_D41A0.dynamicLights_0x3664C[j].event_A.pointer_0x6E8E != (type_entity_0x6E8E*)((uint8_t*)temp_0x6E8E + (xBITINT)v3))
				allert_error();  // only for x86
#endif
		}
	}
}

//----- (00057680) --------------------------------------------------------
void FixPointersAfterLoad_57680()//238680
{
	world_D41A0.dynamicLights_0x3664C[0].event_A.pointer_0x6E8E = nullptr;

	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
	{
		#if defined(x32_BIT_ENVIRONMENT)
			if ((uint32_t)Entities_EA3E4[indexx]->playerData_0xA4_164x == 0x2c75e28)
				Entities_EA3E4[indexx]->playerData_0xA4_164x = unk_F42B0x;
		#else
			if ((uint64_t)Entities_EA3E4[indexx]->playerData_0xA4_164x == 0x2c75e28)//0x014F82E8//0x2c75e28-(uint32_t)&D41A0_BYTESTR_0
				Entities_EA3E4[indexx]->playerData_0xA4_164x = unk_F42B0x;
		#endif
	}

	for (int v0 = 0; v0 < world_D41A0.NumberOfPlayers_0xe; v0++)
	{
		Entities_EA3E4[world_D41A0.players_0x2BDE[v0].playerIndex_0x00a_2BE4_11240]->playerData_0xA4_164x = &world_D41A0.players_0x2BDE[v0].playerData_0x3E6_2BE4_12228;
	}
	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
		if (Entities_EA3E4[indexx]->class_0x3F_63)
			Entities_EA3E4[indexx]->typeParams_0xA0_160x = &str_D7BD6[(Entities_EA3E4[indexx]->typeParams_0xA0_160x - world_D41A0.typeParamsBase_0x36DF6)+ 59];
}

//----- (000549A0) --------------------------------------------------------
void CopyPlayerSpellData_549A0(type_str_611* a1x, type_str_611* a2x)//2359a0
{
	a1x->availableSpells_0x3E9_1001x = a2x->availableSpells_0x3E9_1001x;
	a1x->SpellExperience_0x263_611x = a2x->SpellExperience_0x263_611x;
	a1x->SpellLevels_0x41D_1053z = a2x->SpellLevels_0x41D_1053z;
	a1x->spellSlotTypes_0x3B5_949x = a2x->spellSlotTypes_0x3B5_949x;
	a1x->selectedSubSpells_0x437_1079x = a2x->selectedSubSpells_0x437_1079x;
}

//----- (00049F90) --------------------------------------------------------
void RebuildEntityLists_49F90()//22af90
{
	int iy;
	signed int v2; // ebx
	type_entity_0x6E8E* indexx; // eax
	for (iy = 1; iy < 0x3e8; iy++)
	{
		if (Entities_EA3E4[iy]->class_0x3F_63 && Entities_EA3E4[iy]->flags_0xc_12_15.byte[1] & 4)
			ReleaseEntity_57F20(Entities_EA3E4[iy]);
	}
	world_D41A0.freeEntityCount_0x35 = -1;
	v2 = 999;
	world_D41A0.freeEntityCount2_0x11e6 = -1;
	do
	{
		indexx = Entities_EA3E4[v2];
		if (indexx->class_0x3F_63)
		{
			if (indexx->flags_0xc_12_15.byte[2] & 2)
			{
				world_D41A0.freeEntityCount2_0x11e6++;
				world_D41A0.freeEntities2_0x11EA[world_D41A0.freeEntityCount2_0x11e6] = Entities_EA3E4[v2];
			}
		}
		else
		{
			world_D41A0.freeEntityCount_0x35++;
			world_D41A0.freeEntities_0x246[world_D41A0.freeEntityCount_0x35] = Entities_EA3E4[v2];
		}
		v2--;
	} while (v2 > 0);
}

//----- (00055AB0) --------------------------------------------------------
void CreatePlayerSpellEntities_55AB0(type_str_0x2BDE* playStr)//236ab0
{
	for (int i = 0; i < 26; i++)
	{
		if (playStr->playerData_0x3E6_2BE4_12228.str_611.availableSpells_0x3E9_1001x.SpellIndex[spellIndex_D94FF[i]] || playStr->playerData_0x3E6_2BE4_12228.str_611.pickedUpSpells_0x403_1027x.SpellIndex[spellIndex_D94FF[i]])
		{
			if (!playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]])
			{
				type_entity_0x6E8E* tempEvent = CreateEntity_4A190(&Entities_EA3E4[playStr->playerIndex_0x00a_2BE4_11240]->position_0x4C_76, 15, spellIndex_D94FF[i]);
				if (tempEvent)
				{
					playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]] = tempEvent - world_D41A0.entities_0x6E8E;
					tempEvent->parentId_0x28_40 = Entities_EA3E4[playStr->playerIndex_0x00a_2BE4_11240] - world_D41A0.entities_0x6E8E;
					tempEvent->flags_0xc_12_15.byte[0] |= 1u;
					SetSpell_6D5E0(tempEvent, playStr->playerData_0x3E6_2BE4_12228.str_611.selectedSubSpells_0x437_1079x.SpellIndex[spellIndex_D94FF[i]]);
				}
			}
		}
		else
		{
			if (playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]])
			{
				playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]] = 0;
				ReleaseEntity_57F20(Entities_EA3E4[playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]]]);
			}
		}
	}
	if (playStr->playerData_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 != -1 && !playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[playStr->playerData_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105])
		playStr->playerData_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 = -1;
	if (playStr->playerData_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 != -1 && !playStr->playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[playStr->playerData_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107])
		playStr->playerData_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 = -1;
}

//----- (00071990) --------------------------------------------------------
void ReloadLevelTmaps_71990()//252990
{
	char i; // bl
	unsigned __int16 j; // si
	unsigned int v4; // ST04_4
	char v6_return; // [esp+4h] [ebp-4h]

	v6_return = 0;
	if (!tmapCache_E9C28)
		InitTmapsAndSprites_712F0();
	for(int i=0;i<504;i++)
		if (!world_D41A0.usedTmaps_0x39[i])
			ResetTmap_70D20(i);
	for (i = 2; i && !v6_return; i--)
	{
		for (j = 0; j < 504 && !v6_return; j++)
		{
			if (i == world_D41A0.usedTmaps_0x39[j] && !m_ptrLoadedSprites_F66F0x[j])
			{
				v4 = GetTmapGroupSize_70EF0(j);
				if (GetTmapCacheFreeSize_71E60(tmapCache_E9C28) <= v4)
				{
					if (GetTmapCacheFreeSize_71E60(tmapCache_E9C28) < 0x400)
						v6_return = 1;
				}
				else
				{
					InitTmaps(j);
				}
			}
		}
	}
}

//----- (00071930) --------------------------------------------------------
void MarkLoadedTmaps_71930()//252930
{
	for (int i=0; i < 508; i++) world_D41A0.usedTmaps_0x39[i] = 0;
	for (int i=0; i < 504; i++) {
		if (m_ptrLoadedSprites_F66F0x[i])
		{
			world_D41A0.usedTmaps_0x39[i] = 1;
			if (m_LevelSpriteList_F5340[i])
				world_D41A0.usedTmaps_0x39[i]++;
		}
	}	
}

//----- (00049830) --------------------------------------------------------
void SetStagetagForTermod_49830(Type_Level_2FECE* terrain)//22a830 //set v1x->word_12
{
	type_entity_0x30311* entity = &terrain->entity_0x30311[1];
	do
	{
		if (entity->DisId == -1 && entity->type_0x30311 == 0x0A)
		{
			if ((entity->subtype_0x30311 == 0x1C) || (entity->subtype_0x30311 == 0x1D) || (entity->subtype_0x30311 == 0x1F) || (entity->subtype_0x30311 == 0x32) || (entity->subtype_0x30311 == 0x50))
				entity->stageTag_12 = 1;
		}
		entity++;
	} while (entity < &terrain->entity_0x30311[1200]);
}

//----- (00046F80) --------------------------------------------------------
void LoadTextureData(__int16 vgaTypeResolution, MapType_t MapType, uint8_t* textureBuffer)//227f80
{
	MapType_t v0; // al
	//char* v1;
	//int v2; // esi
	int32_t file2lenght; // eax
	int32_t file1Lenght;
	//char* filearray1;
	//int v4; // eax
	//char* v5; // ebx
	//unsigned int v6; // esi
	//bitmap_pos_struct2_t* v7x; // esi
	//int v8; // [esp+0h] [ebp-2h]
	//int v9; // [esp+4h] [ebp+2h]
	//int v10; // [esp+8h] [ebp+6h]
	//int v11; // [esp+Ch] [ebp+Ah]
	//__int16 v12; // [esp+10h] [ebp+Eh]
	//char v13; // [esp+40h] [ebp+3Eh]
	//int v14; // [esp+80h] [ebp+7Eh]

	char dataPath[MAX_PATH];
	char tabPath[MAX_PATH];

	FreeWebSprites_47130();
	v0 = MapType;
	switch (v0)
	{
	case MapType_t::Night:
			{
				if (vgaTypeResolution == 1)
				{
					sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/MWEBN0-0.DAT");
					sprintf(tabPath, "%s/%s", cdDataPath.c_str(), "DATA/MWEBN0-0.TAB");
				}
				else
				{
					sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/HWEBN0-0.DAT");
					sprintf(tabPath, "%s/%s", cdDataPath.c_str(), "DATA/HWEBN0-0.TAB");
				}
				break;
			}
	case MapType_t::Cave:
			{
				if (vgaTypeResolution == 1)
				{
					sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/MWEBC0-0.DAT");
					sprintf(tabPath, "%s/%s", cdDataPath.c_str(), "DATA/MWEBC0-0.TAB");
				}
				else
				{
					sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/HWEBC0-0.DAT");
					sprintf(tabPath, "%s/%s", cdDataPath.c_str(), "DATA/HWEBC0-0.TAB");
				}
				break;
			}
	case MapType_t::Day:
		{
			if (vgaTypeResolution == 1)
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/MWEBD0-0.DAT");
				sprintf(tabPath, "%s/%s", cdDataPath.c_str(), "DATA/MWEBD0-0.TAB");
			}
			else
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/HWEBD0-0.DAT");
				sprintf(tabPath, "%s/%s", cdDataPath.c_str(), "DATA/HWEBD0-0.TAB");
			}
			break;
		}
	}

	file1Lenght = DataFileIO::ReadFileAndDecompress(dataPath, &textureBuffer);
	if (file1Lenght > 0)
	{
		file1Lenght = (file1Lenght + 8) & 0xFFF8;
		//v14 = v4;
		uint8_t* x = &textureBuffer[file1Lenght];
		file2lenght = DataFileIO::ReadFileAndDecompress(tabPath, &x); //fix it
		//v5 = file2lenght;
		if (file2lenght > 0)
		{
			//v6 = file2lenght+file1Lenght;
			//filearray1 = (char*)sub_83CD0_malloc2(file2lenght + file1Lenght);
			//webSpritesData_D4188 = (uint8_t*)filearray1;
			webSpritesData_D4188 = (bitmap_pos_struct2_t*)Malloc_83CD0(file2lenght + file1Lenght);
			if (webSpritesData_D4188)
			{
				qmemcpy(webSpritesData_D4188, (void*)textureBuffer, file2lenght + file1Lenght);
				webSpritesTab_D418C = (bitmap_pos_struct2_t*)((int8_t*)webSpritesData_D4188+file1Lenght);
				//webSpritesTab_D418C = &webSpritesData_D4188[file1Lenght / sizeof(bitmap_pos_struct2_t)];
				webSpritesTabEnd_D4190 = (bitmap_pos_struct2_t*)((int8_t*)webSpritesData_D4188+file2lenght+file1Lenght);
				//webSpritesTabEnd_D4190 = &webSpritesData_D4188[(file2lenght + file1Lenght) / sizeof(bitmap_pos_struct2_t)];
				if (vgaTypeResolution & 1)
					CreateIndexDatTabDoubled_98709(webSpritesTab_D418C, webSpritesTabEnd_D4190, (uint8_t*)webSpritesData_D4188, webSprites_D4188);
				else
					CreateIndexDatTab_9874D(webSpritesTab_D418C, webSpritesTabEnd_D4190, (uint8_t*)webSpritesData_D4188, webSprites_D4188);
			}
		}
	}
}

//----- (0006D5E0) --------------------------------------------------------
void SetSpell_6D5E0(type_entity_0x6E8E* entity, int spellId)//24e5e0
{
	// 2339AD call sub_6D5E0 with Entities[SpellEnabled[spell]]: the slot can hold a non-spell entity by then (a building,
	// record11 turn 7421), its model is past the table and the original reads byte_DA818 + model*50h, the credits
	// pointers (24E5EC movsx esi,byte ptr [ebx+40h])
	if (entity->model_0x40_64 >= NUMBER_OF_SPELLS)
		spellTableOverrun[entity - world_D41A0.entities_0x6E8E] = true;
	int locSpellId = (int8_t)spellId;//24E604 movsx esi,al
	if (locSpellId > SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].levelCount_0 - 1)
		locSpellId = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].levelCount_0 - 1;
	// 240890 UpdateCastleLevel_5F890 passes the wizard, its word_0x2C_44 - 1 can be negative (record038 turn 6754: -37) and the original
	// reads before byte_DA818 (a pointer of another table); 0 here and in DOSBox (24E614)
	if (locSpellId < 0)
		locSpellId = 0;
	if (entity->stage_0x2E_46)
	{
		entity->param2_0x2C_44 = locSpellId + 1;
	}
	else
	{
		entity->levelPar3_0x46_70 = locSpellId;
		entity->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].subSpellIndex_2;
		entity->maxStage_0x30_48 = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].maxStage_0x18;
		entity->poseIndex_0x3B_59 = (SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].fontType_0x1B & 1) == 0;
		entity->counter_0x3C_60 = 0;
		entity->fontTypeIndex_0x3D_61 = 0;
		//fix
		entity->manaRegen_0x88_136 = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].maxManaLimit_A;
		//fix
		int mana = GetSpellManaCost_6D710(Entities_EA3E4[entity->parentId_0x28_40], entity->model_0x40_64, locSpellId);
		entity->maxMana_0x8C_140 = mana;
		if (entity->maxStage_0x30_48)
			mana /= entity->maxStage_0x30_48;
		entity->mana_0x90_144 = mana;
		if (gameState_D41A4.OptionsSettingFlag_24 & 0x20)
		{
			entity->manaRegen_0x88_136 = 0;
			entity->mana_0x90_144 = 1;
		}
	}
}

//----- (000712F0) --------------------------------------------------------
void InitTmapsAndSprites_712F0()//2522f0
{
	tmapCache_E9C28 = InitTmapCache_71B40(gameState_D41A4.dword_0xE6_heapsize_230, 504, (type_x_DWORD_E9C28_str*)gameState_D41A4.pointer_0xE2_heapbuffer_226);
	if (tmapCache_E9C28)
		animations_E9C08x = AllocateAnimations_72120(0x1F8u);
	OpenTmaps_70A60();
	SetTmapsFile_71A70(world_D41A0.terrain_2FECE.MapType);
	memset(m_ptrLoadedSprites_F66F0x, 0, 504 * sizeof(Type_Sprite**));
	memset(str_F5F10, 0, 504*sizeof(Type_Sprite**));
	memset(tmapGroupLastUse_F5730, 0, 2016);
	BuildLevelSpriteList_71890();
	LoadLevelSpriteTmaps_718F0();
}

//----- (00071890) --------------------------------------------------------
void BuildLevelSpriteList_71890()//252890
{
	//char result; // al
	int v0y = 0;
	memset(m_LevelSpriteList_F5340, 0, 504);
    while (spriteDefsTable_D951C[v0y].speed_6 || spriteDefsTable_D951C[v0y].worldSize_8)
    {
        if (spriteDefsTable_D951C[v0y].levelSpriteGroup_11 == -1)
        {
            m_LevelSpriteList_F5340[spriteDefsTable_D951C[v0y].textureIdx_0] = spriteDefsTable_D951C[v0y].levelSpriteGroup_11;
        }
        v0y++;
    }
}

//----- (000718F0) --------------------------------------------------------
void LoadLevelSpriteTmaps_718F0()//2528f0
{
	unsigned __int16 v0; // bx
	v0 = 0;
	do
	{
		if (m_LevelSpriteList_F5340[v0])
		{
			InitTmaps(v0);
		}
		v0++;
	} while (v0 < 504);
}

//----- (00071A70) --------------------------------------------------------
void SetTmapsFile_71A70(MapType_t a1)//252a70
{
	FILE* v1; // edx
	//unsigned __int8 result; // al

	v1 = tmapsFile_DB73C;
	//result = a1;
	switch (a1)
	{
	case MapType_t::Day:
	{
		v1 = tmaps0File_DB740;
		break;
	}
	case MapType_t::Night:
	{
		v1 = tmaps1File_DB744;
		break;
	}
	case MapType_t::Cave:
	{
		v1 = tmaps2File_DB748;
		break;
	}
	}
	tmapsFile_DB73C = v1;
	tmapsMapType = a1;
	//return result;
}
// DB73C: using guessed type int tmapsFile_DB73C;
// DB740: using guessed type int tmaps0File_DB740;
// DB744: using guessed type int tmaps1File_DB744;
// DB748: using guessed type int tmaps2File_DB748;

//----- (00072120) --------------------------------------------------------
type_E9C08* AllocateAnimations_72120(unsigned __int16 a1)//253120
{
	unsigned __int16 v1; // di
	type_E9C08* v2x; // esi
	type_E9C08* v3x; // ebx
	type_animations1* v4; // eax
	//int v5; // eax

	v1 = a1;
	SetAllocationTag_83CC0(9);
	v2x = (type_E9C08*)Malloc_83CD0(sizeof(type_E9C08));
	v3x = v2x;
	if (v2x && (v4 = (type_animations1*)Malloc_83CD0(sizeof(type_animations1) * a1), (v2x->animations_2 = v4) != 0))
	{
		v2x->count_0 = a1;
		while (--v1 != 0xffff)
		{
			//v5 = 7 * v1;
			//*(x_DWORD *)(v2x->dword_2 + 4 * v5 + 4) = 0;
			//*(x_DWORD *)(v2x->dword_2 + 4 * v5) = 0;
			v2x->animations_2[v1].Sprite_4 = 0;
			v2x->animations_2[v1].active_0 = 0;
		}
	}
	else if (v2x)
	{
		FreeMem_83E80((uint8_t*)v2x);
		v3x = 0;
	}
	SetAllocationTag_83CC0(10);
	return v3x;
}

//----- (00071B40) --------------------------------------------------------
type_x_DWORD_E9C28_str* InitTmapCache_71B40(int a1, unsigned __int16 a2, type_x_DWORD_E9C28_str* a3y)//252b40
{
	int v5; // eax
	int v6; // edx
	type_x_DWORD_E9C28_str* result; // esi
	//type_x_DWORD_E9C28_str* v10y; // ebx
	uint8_t* v11x; // eax
	uint8_t* v12x; // eax
	uint8_t* v13x; // eax
	unsigned __int16 index = a2;
	if (a3y)
	{
		v5 = sizeof(subtype_x_DWORD_E9C28_str*) * a2 + (sizeof(subtype_x_DWORD_E9C28_str) * a2) + sizeof(type_x_DWORD_E9C28_str);
		v6 = a1 - v5;
		if (a1 == v5)
			return 0;
		a3y->usedCount_20 = 0;
		a3y->allocMode_24 = 2;
		a3y->capacity_0 = v6;
		a3y->freeSize_4 = v6;
		result = a3y;
		a3y->maxEntries_22 = a2;
		a3y->entries_8 = a3y->data;
		a3y->entryPtrs_12x = (subtype_x_DWORD_E9C28_str**)&a3y->data[a2];
		a3y->buffer_16x = (uint32_t*)((uint8_t*)a3y->data+((sizeof(subtype_x_DWORD_E9C28_str) * a2) + sizeof(subtype_x_DWORD_E9C28_str*) * a2));
		while (--index != 0xffff)
			a3y->entries_8[index].size_4 = 0;
	}
	else
	{
		result = (type_x_DWORD_E9C28_str*)Malloc_83CD0(26);
		//result = v10y;
		if (!result
			|| (v11x = (uint8_t*)Malloc_83CD0(a1), (result->buffer_16x = (uint32_t*)v11x) == 0)
			|| (v12x = (uint8_t*)Malloc_83CD0(sizeof(subtype_x_DWORD_E9C28_str) * a2), (result->entries_8 = (subtype_x_DWORD_E9C28_str*)v12x) == 0)
			|| (v13x = (uint8_t*)Malloc_83CD0(sizeof(subtype_x_DWORD_E9C28_str*) * a2), (result->entryPtrs_12x = (subtype_x_DWORD_E9C28_str**)v13x) == 0))
		{
			if (result)
			{
				if (result->buffer_16x)
				{
					if (result->entries_8)
						FreeMem_83E80((uint8_t*)result->entries_8);
					FreeMem_83E80((uint8_t*)result->buffer_16x);
				}
				FreeMem_83E80((uint8_t*)result);
			}
			exit(1);
		}
		result->usedCount_20 = 0;
		result->allocMode_24 = 1;
		result->maxEntries_22 = a2;
		result->capacity_0 = a1;
		result->freeSize_4 = a1;
		while (--index != 0xffff)
			result->entries_8[index].size_4 = 0;
	}
	return result;
}

//----- (0006D710) --------------------------------------------------------
int GetSpellManaCost_6D710(type_entity_0x6E8E* event, uint8 spellIndex, uint8 subSpellIndex)//24e710
{
	bool add3000 = false;
	int result = SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[subSpellIndex].manaCost_6;
	if (spellIndex == 2 && event > Entities_EA3E4[0])
	{
		type_entity_0x6E8E* entity2 = Entities_EA3E4[event->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
		if (entity2 <= Entities_EA3E4[0])
		{
			if (event->playerData_0xA4_164x->castleBuilding_0x1BE_446)
				add3000 = 3000;
			if (add3000)
				result += 3000;
			return result;
		}
		switch (entity2->param_0x10_16)
		{
		case 0:
			result = 1000;
			break;
		case 1:
			result = 10000;
			break;
		case 2:
			result = 20000;
			break;
		case 3:
			result = 40000;
			break;
		case 4:
			result = 80000;
			break;
		case 5:
			result = 160000;
			break;
		case 6:
			result = 320000;
			break;
		default:
			result = 300000000;
			break;
		}
		if (entity2->param_0x10_16 >= 7)
		{
			if (add3000)
				result += 3000;
			return result;
		}
		switch (subSpellIndex)
		{
			case 1:
				result = ((320 * result) - (my_sign32(320 * result) << 8) + my_sign32(320 * result)) >> 8;
				break;

			case 2:
				result = ((384 * result) - (my_sign32(384 * result) << 8) + my_sign32(384 * result)) >> 8;
				break;
		}
		if (entity2->param_0x10_16)
		{
			if (add3000)
				result += 3000;
			return result;
		}
		if (event->playerData_0xA4_164x->castleBuilding_0x1BE_446)
			add3000 = true;
		if (add3000)
			result += 3000;
		return result;
	}
	return result;
}

//----- (00083CC0) --------------------------------------------------------
void SetAllocationTag_83CC0(char a1)//264cc0
{
	x_BYTE_E29E8 = a1; // FIXME: Can this be removed? Only written, never read.
}
// E29E8: using guessed type char x_BYTE_E29E8;


//----- (00011EE0) --------------------------------------------------------
void InitStageVars_11EE0()//1f2ee0
{
	int index2; // ax
	int index; // eax
	world_D41A0.countStageVars_0x36E00 = 0;
	memset(world_D41A0.StageVars2_0x365F4, 0, sizeof(type_str_0x3647Ac) * 11);
	index2 = 0xA;
	while (index2 > 0 && !(world_D41A0.terrain_2FECE.StageVars_0x3647A[index2].index_0x3647A_0 & 0xF))
		index2--;
	world_D41A0.countStageVars_0x36E00 = index2;
	for (index = 1; index <= world_D41A0.countStageVars_0x36E00; ++index)
	{
		world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0 = world_D41A0.terrain_2FECE.StageVars_0x3647A[index].index_0x3647A_0 & 0xF;
		if (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0)
		{
			if (world_D41A0.terrain_2FECE.StageVars_0x3647A[index].index_0x3647A_0 & 0x80)
				world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 |= 1u;
			if (world_D41A0.terrain_2FECE.StageVars_0x3647A[index].index_0x3647A_0 & 0x40)
				world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 |= 2u;
			if (world_D41A0.terrain_2FECE.StageVars_0x3647A[index].index_0x3647A_0 & 0x10)
				world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 |= 0x20u;
			if (world_D41A0.terrain_2FECE.StageVars_0x3647A[index].index_0x3647A_0 & 0x20)
				world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 |= 0x40u;
			switch (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0)
			{
			case 1:
			case 2:
				world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.axis.x = world_D41A0.terrain_2FECE.StageVars_0x3647A[index].target_0x3647C_4.axis.x << 8;
				world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.axis.y = world_D41A0.terrain_2FECE.StageVars_0x3647A[index].target_0x3647C_4.axis.y << 8;
				break;
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
				world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.axis.x = 0;
				if (world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 & 2)
					world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.axis.x =
					world_D41A0.terrain_2FECE.entity_0x30311[world_D41A0.terrain_2FECE.StageVars_0x3647A[index].target_0x3647C_4.axis.x].subtype_0x30311;
				break;
			case 6:
			case 7:
				world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.axis.x = world_D41A0.terrain_2FECE.StageVars_0x3647A[index].target_0x3647C_4.axis.x;
				break;
			default:
				break;
			}
			world_D41A0.StageVars2_0x365F4[index].entityIndex_0x3647A_2._axis_2d.x = world_D41A0.terrain_2FECE.StageVars_0x3647A[index].stage_0x3647A_1;
		}
	}
}

//----- (00012100) --------------------------------------------------------
void SetStageVarForEntity_12100(type_entity_0x30311* entity, type_entity_0x6E8E* event, char a3)//1f3100
{
	signed __int16 v3; // bx
	unsigned __int8 v4; // cl

	v3 = 1;
	v4 = 0;
	while (v3 <= world_D41A0.countStageVars_0x36E00 && !v4)
	{
		if (world_D41A0.StageVars2_0x365F4[v3].index_0x3647A_0
			&& !(world_D41A0.StageVars2_0x365F4[v3].stage_0x3647A_1 & 1)
			&& entity - world_D41A0.terrain_2FECE.entity_0x30311 ==
			world_D41A0.terrain_2FECE.StageVars_0x3647A[v3].entityIndex_0x3647A_2.word)
		{
			v4 = v3;
		}
		v3++;
	}
	if (!v4)
	{
		for (int index = 1; index <= world_D41A0.countStageVars_0x36E00; index++)
		{
			if (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0//*v7
				&& (world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 & 1)
				&& world_D41A0.terrain_2FECE.entity_0x30311[world_D41A0.terrain_2FECE.StageVars_0x3647A[index].entityIndex_0x3647A_2.word].subtype_0x30311 ==
				entity->subtype_0x30311)
			{
				v4 = index;
				break;
			}
		}
	}
	if (v4)
	{
		if (a3)
			event->nextStage_0x4A_74 = v4;
		else
			AdvanceStage_12330(event, v4);
	}

	for (int index = 1; index <= world_D41A0.countStageVars_0x36E00; index++)
	{
		if (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0)
		{
			switch (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0)
			{
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
				if (!(world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 & 2)
					&& world_D41A0.terrain_2FECE.StageVars_0x3647A[index].target_0x3647C_4.axis.x ==
					entity - world_D41A0.terrain_2FECE.entity_0x30311)
				{
					//fix it
					world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.pointer_0x6E8E = event;
					//fix it
					world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 &= 0xFBu;
				}
				break;
			default:
				break;
			}
		}
	}
}

/*
//----- (00011EE0) --------------------------------------------------------
void InitStageVarsNew_11EE0()//1f2ee0
{
	//uint8_t* v1; // ST00_4
	signed __int16 v2; // ax
	//uint8_t* v3; // esi
	signed int result; // eax
	//int v5; // edx
	//int v6; // edi
	char v7; // ST14_1
	//uint8_t* v8; // edx
	//int v9; // edx
	//int v10; // edx
	//int v11; // edx
	//int v12; // edx
	//int v13; // ecx
	//__int16 v14; // di
	//uint8_t* v15; // edx
	//int v16; // edx
	//uint8_t* v17; // ecx

	//a1=196302

	x_D41A0_BYTEARRAY_0_to_x_D41A0_BYTESTR_0();//only for rewriting byteaaray0
	D41A0_BYTESTR_0.byte_0x36E00 = 0;
	//memset(D41A0_BYTESTR_0.array_0x365F4, 0, 88);
	for (int i = 0; i < 0xb; i++)//fix 0x1 to real count
	{
		memset(&D41A0_BYTESTR_0.array_0x365F4[i], 0x0,0x8);
	}

	v2 = 10;
	while (v2 > 0 && !(D41A0_BYTESTR_0.str_2FECE.array_0x3647A[v2].byte_0 & 0xF))
		v2--;
	D41A0_BYTESTR_0.byte_0x36E00 = v2;
	//196302+26028=
	for (result = 1; (signed __int16)result <= D41A0_BYTESTR_0.byte_0x36E00; ++result)
	{
		//v5 = 8 * (signed __int16)result;
		//v6 = (int)a1 + 8 * (signed __int16)result;//array_2FECE
		v7 = D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].byte_0 & 0xF;
		//v8 = x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result;

		// *(0x365F2 + x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 2) = v7;
		D41A0_BYTESTR_0.array_0x365F4[result].byte_0 = v7;
		if (v7)
		{
			if (D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].byte_0 < 0)
				D41A0_BYTESTR_0.array_0x365F4[result].byte_1 |= 1u;
			//v9 = 8 * (signed __int16)result;
			if (D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].byte_0 & 0x40)
				D41A0_BYTESTR_0.array_0x365F4[result].byte_1 |= 2u;
			//v10 = 8 * (signed __int16)result;
			if (D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].byte_0 & 0x10)
				D41A0_BYTESTR_0.array_0x365F4[result].byte_1 |= 0x20u;
			//v11 = 8 * (signed __int16)result;
			if (D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].byte_0 & 0x20)
				D41A0_BYTESTR_0.array_0x365F4[result].byte_1 |= 0x40u;
			switch (D41A0_BYTESTR_0.array_0x365F4[result].byte_0)
			{
			case 1:
			case 2:
				//v12 = 8 * (signed __int16)result;
				//v13 = (int)a1 + 8 * (signed __int16)result;
				//v14 = D41A0_BYTESTR_0.array_0x3647A[result].word_4;
				//v15 = x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result;
D41A0_BYTESTR_0.array_0x365F4[result].word_4 = D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].word_4 << 8;
D41A0_BYTESTR_0.array_0x365F4[result].word_6 = D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].word_6 << 8;
break;
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
				//v16 = 8 * (signed __int16)result;
				//v17 = x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result;
				//LOBYTE(v16) = *(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 0x365F5);
				D41A0_BYTESTR_0.array_0x365F4[result].word_4 = 0;
				if (D41A0_BYTESTR_0.array_0x365F4[result].byte_1 & 2)
					D41A0_BYTESTR_0.array_0x365F4[result].word_4 =
					D41A0_BYTESTR_0.str_2FECE.array_0x3030E[D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].word_4].word_5;
				// *(x_WORD *)(0x3030E+ 20 * D41A0_BYTESTR_0.array_0x36479[result].word_5 + x_D41A0_BYTEARRAY_0+ 5);
				break;
			case 6:
			case 7:
				D41A0_BYTESTR_0.array_0x365F4[result].word_4 = D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].word_4;
				break;
			default:
				break;
			}
			D41A0_BYTESTR_0.array_0x365F4[result].byte_2 = D41A0_BYTESTR_0.str_2FECE.array_0x3647A[result].byte_1;
		}
	}

	x_D41A0_BYTESTR_0_to_x_D41A0_BYTEARRAY_0();//only for rewriting byteaaray0
	//x_D41A0_BYTEARRAY_0 = (uint8_t*)v3;
	//return result;
}
// 365F8: using guessed type int sub_365F8();
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
*/
/*
//----- (00012100) --------------------------------------------------------
void SetStageVarForEntityNew_12100(uint8_t* a1, uint8_t* a2, char a3)//1f3100
{
	signed __int16 v3; // bx
	unsigned __int8 v4; // cl
	//uint8_t* v5; // esi
	signed __int16 v6; // bx
	//uint8_t* v7; // edx
	signed __int16 v8; // bx
	int i; // ecx
	//__int16 result; // ax
	int index_v5x;
	int index_v7x;

	x_D41A0_BYTEARRAY_0_to_x_D41A0_BYTESTR_0();//only for rewriting byteaaray0

	v3 = 1;
	v4 = 0;
	//v5 = &x_D41A0_BYTEARRAY_0[0x365FC];
	index_v5x=0;
	while (v3 <= D41A0_BYTESTR_0.byte_0x36E00 && !v4)
	{
		//D41A0_BYTESTR_0.array_0x365F4[1+ index_v5x].byte_0
		if (D41A0_BYTESTR_0.array_0x365F4[1 + index_v5x].byte_0
			&& !(D41A0_BYTESTR_0.array_0x365F4[1 + index_v5x].byte_1 & 1)
			&& (a1 - (&x_D41A0_BYTEARRAY_0[0x30331])) / 20 == //fix it x_D41A0_BYTEARRAY_0
			D41A0_BYTESTR_0.str_2FECE.array_0x3647A[v3].word_2)
		{
			v4 = v3;
		}
		v3++;
		index_v5x++;
	}
	if (!v4)
	{
		v6 = 1;
		//v7 = &x_D41A0_BYTEARRAY_0[0x365FC];
		index_v7x = 0;
		while (v6 <= D41A0_BYTESTR_0.byte_0x36E00 && !v4)
		{
			if (D41A0_BYTESTR_0.array_0x365F4[1 + index_v7x].byte_0
				&& D41A0_BYTESTR_0.array_0x365F4[1 + index_v7x].byte_1 & 1
				&& 	*(_WORD *)(&x_D41A0_BYTEARRAY_0[0x30313//fix it x_D41A0_BYTEARRAY_0
					+ 20 * D41A0_BYTESTR_0.str_2FECE.array_0x3647A[v6].word_2]) ==
				*(_WORD *)(a1 + 2))
			{
				v4 = v6;
			}
			v6++;
			index_v7x ++;
		}
	}
	if (v4)
	{
		if (a3)
			*(x_WORD *)(a2 + 74) = v4;
		else
			AdvanceStage_12330(a2, v4);
	}
	v8 = 1;
	//for (i = &x_D41A0_BYTEARRAY_0[0x365FC]; ; i += 8)
	for (i = 0; ; i ++)
	{
		//result = D41A0_BYTESTR_0.byte_0x36E00;
		if (v8 > D41A0_BYTESTR_0.byte_0x36E00)
			break;
		if (D41A0_BYTESTR_0.array_0x365F4[1 + i].byte_0)
		{
			switch (D41A0_BYTESTR_0.array_0x365F4[1 + i].byte_0)
			{
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
				if (!(D41A0_BYTESTR_0.array_0x365F4[1 + i].byte_1 & 2)
					&& *(uint16_t*)(8 * v8 + &x_D41A0_BYTEARRAY_0[0x3647E]) ==
					(a1	- (&x_D41A0_BYTEARRAY_0[0x30311]))/ 20)
				{
					D41A0_BYTESTR_0.array_0x365F4[1 + i].word_4 = (uint32_t)a2;//must test it
					D41A0_BYTESTR_0.array_0x365F4[1 + i].word_6 = (uint32_t)a2>>16;//must test it
					D41A0_BYTESTR_0.array_0x365F4[1 + i].byte_1 &= 0xFBu;
				}
				break;
			default:
				break;
			}
		}
		v8++;
	}
	x_D41A0_BYTESTR_0_to_x_D41A0_BYTEARRAY_0();//only for rewriting byteaaray0
	//return result;
}
// 365F8: using guessed type int sub_365F8();
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
*/

//----- (000122A0) --------------------------------------------------------
void AdvanceEntityStage_122A0(type_entity_0x6E8E* event)//1f32e0
{
	if (event->nextStage_0x4A_74)
		AdvanceStage_12330(event, event->nextStage_0x4A_74);
}

//----- (000122C0) --------------------------------------------------------
void MarkStageVarsForIndex_122C0(__int16 a1)//1f32c0
{
	for (signed __int16 index = 1; index <= world_D41A0.countStageVars_0x36E00; index++)
	{
		if (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0 == 7 && a1 == world_D41A0.StageVars2_0x365F4[index].target_0x3647C_4.axis.x)
			world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 |= 0x18u;
	}
}

//----- (00012330) --------------------------------------------------------
void AdvanceStage_12330(type_entity_0x6E8E* event, __int16 a2)//1f3330
{
	char v3; // dl
	unsigned __int8 v7; // dh
	char v8; // cl
	v3 = 1;
	if (!a2)
	{
		v3 = 0;
	}
	else
	{
		v8 = world_D41A0.StageVars2_0x365F4[a2].entityIndex_0x3647A_2._axis_2d.y & 3;
		world_D41A0.StageVars2_0x365F4[a2].entityIndex_0x3647A_2._axis_2d.y++;
		v7 = world_D41A0.StageVars2_0x365F4[a2].stage_0x3647A_1 & 0x60;
		switch (v7) {
		case 0x20u:
		{
			if (v8 != 3)
				break;
			v3 = 0;
			break;
		}
		case 0x40u:
		{
			if ((v8 & 1) == 0)
				break;
			v3 = 0;
			break;
		}
		case 0x60u:
		{
			if ((v8 & 3) == 0)
				break;
			v3 = 0;
			break;
		}
		}
	}
	if (!v3)
	{
		ClearEntityStage_12470(event, 8 * event->model_0x40_64 + 1);
		return;
	}
	event->actionIndex_0x45_69 = 8 * event->model_0x40_64 + 7;
	event->StageVar1_0x48_72 = a2;
	event->nextStage_0x4A_74 = 0;
	event->StageVar2_0x49_73 = world_D41A0.StageVars2_0x365F4[a2].index_0x3647A_0;
	if (event->StageVar2_0x49_73 == 6)
		event->nextStage_0x4A_74 = world_D41A0.StageVars2_0x365F4[a2].target_0x3647C_4.axis.x;
}

//----- (00012410) --------------------------------------------------------
void AdvanceStageOrSetAction_12410(type_entity_0x6E8E* event, char actionIndex)//1f3410
{
	bool run = false;
	if (world_D41A0.StageVars2_0x365F4[event->StageVar1_0x48_72].entityIndex_0x3647A_2._axis_2d.x)
		AdvanceStage_12330(event, world_D41A0.StageVars2_0x365F4[event->StageVar1_0x48_72].entityIndex_0x3647A_2._axis_2d.x);
	else
		run = true;
	if (run)
		ClearEntityStage_12470(event, actionIndex);
}

//----- (00012470) --------------------------------------------------------
void ClearEntityStage_12470(type_entity_0x6E8E* event, char actionIndex)//1f3470
{
	event->StageVar2_0x49_73 = 0;
	event->StageVar1_0x48_72 = 0;
	event->nextStage_0x4A_74 = 0;
	event->actionIndex_0x45_69 = actionIndex;
}

//----- (00012500) --------------------------------------------------------
void UpdateEntityStage_12500(type_entity_0x6E8E* entity)//1f3500
{
	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x1f3504, CommandLineParams.DoDebugafterload(), 0xd7);
	}
	if ((entity->actionIndex_0x45_69 & 7) < 4u || (entity->actionIndex_0x45_69 & 7) > 5u)
	{
		switch (entity->StageVar2_0x49_73)
		{
			case 0xA:
				if ((entity->actionIndex_0x45_69 & 7) != 2 && (entity->actionIndex_0x45_69 & 7) != 6)
					AdvanceStage_12330(entity, entity->StageVar1_0x48_72);
				break;
			case 0xD:
			case 0xE:
			case 0x10:
			case 0x11:
				if ((entity->actionIndex_0x45_69 & 7) != 2 && (entity->actionIndex_0x45_69 & 7) != 6)
					entity->actionIndex_0x45_69 = 8 * entity->model_0x40_64 + 7;
				break;
			case 0xF:
				if (entity->actionIndex_0x45_69 & 7)
					AdvanceStage_12330(entity, entity->StageVar1_0x48_72);
				break;
			default:
				switch (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].index_0x3647A_0)
				{
					case 1:
						//1F35DE movsx edx,[ecx+4]; movsx eax,[ebx+4Ch]; sub edx,eax: 32-bit difference
						if (abs((int16_t)world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].target_0x3647C_4.axis.x - (int16_t)entity->position_0x4C_76.x) <= 2048)
							if (abs((int16_t)world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].target_0x3647C_4.axis.y - (int16_t)entity->position_0x4C_76.y) <= 2048)
								AdvanceStageOrSetAction_12410(entity, 8 * entity->model_0x40_64 + 1);
						break;
					case 3:
						if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].stage_0x3647A_1 & 4)
						{
							entity->nextStage_0x4A_74 = 0;
							if ((entity->actionIndex_0x45_69 & 7) != 2 && (entity->actionIndex_0x45_69 & 7) != 6)
								AdvanceStageOrSetAction_12410(entity, 8 * entity->model_0x40_64 + 1);
						}
						else
						{
							if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].stage_0x3647A_1 & 2)
								if (entity->nextStage_0x4A_74)
									if (Entities_EA3E4[entity->nextStage_0x4A_74]->life_0x8 < 0 || Entities_EA3E4[entity->nextStage_0x4A_74]->flags_0xc_12_15.byte[1] & 4)
										entity->nextStage_0x4A_74 = 0;
						}
						break;
					case 4:
					case 5:
					case 8:
					case 9:
						if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].stage_0x3647A_1 & 4)
							AdvanceStageOrSetAction_12410(entity, 8 * entity->model_0x40_64 + 1);
						else if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].stage_0x3647A_1 & 2)
						{
							if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].index_0x3647A_0 >= 4u
								&& world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].index_0x3647A_0 <= 5u
								&& entity->nextStage_0x4A_74 != 0
								&& (Entities_EA3E4[entity->nextStage_0x4A_74]->life_0x8 < 0 || (Entities_EA3E4[entity->nextStage_0x4A_74]->flags_0xc_12_15.byte[1] & 4)))
								entity->nextStage_0x4A_74 = 0;
						}
						else
						{
							if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].index_0x3647A_0 == 9)
							{
								//1F369A mov esi,[ecx+4]: the target entity; 1F36A5 movsx edx,[esi+4Ch]; movsx eax,[ebx+4Ch]; sub edx,eax
								type_entity_0x6E8E* target = world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].target_0x3647C_4.pointer_0x6E8E;
								if (target >= Entities_EA3E4[0] && target < Entities_EA3E4[1000])//fix
									if (abs((int16_t)target->position_0x4C_76.x - (int16_t)entity->position_0x4C_76.x) <= 3072)
										if (abs((int16_t)target->position_0x4C_76.y - (int16_t)entity->position_0x4C_76.y) <= 3072)
											AdvanceStageOrSetAction_12410(entity, 8 * entity->model_0x40_64 + 1);
							}
						}
						break;
					case 6:
						entity->nextStage_0x4A_74--;
						if (!entity->nextStage_0x4A_74)
							AdvanceStageOrSetAction_12410(entity, 8 * entity->model_0x40_64 + 1);
						break;
					case 7:
						if (world_D41A0.StageVars2_0x365F4[entity->StageVar1_0x48_72].stage_0x3647A_1 & 0x18)
						{
							CheckStageVarsComplete_12870();
							AdvanceStageOrSetAction_12410(entity, 8 * entity->model_0x40_64 + 1);
						}
						break;
				}
				break;
		}
	}
}

int debugcount_sub_12780 = 0;
//----- (00012780) --------------------------------------------------------
void UpdateStageVars_12780()//1f3780
{
	signed __int16 v0; // bx
	//int resultx2;
	signed __int16 v2; // dx
	char v3; // cl
	type_entity_0x6E8E* v4x; // ecx
	char v5; // dh

	//adress 1f3783
	if (CommandLineParams.DoDebugSequences()) {
		//add_compare(0x1F3783, CommandLineParams.DoDebugafterload());
	}
	/*if (debugcount_sub_12780 >= 2)
	{
		debugcount_sub_12780++;
		debugcount_sub_12780--;
	}
	debugcount_sub_12780++;*/

	v0 = 1;
	while (v0 <= world_D41A0.countStageVars_0x36E00)
	{
		/*if (!world_D41A0.StageVars2_0x365F4[v0].index_0x3647A_0)
		{
			v0++;
			break;
		}*/
		v2 = 0;
		switch (world_D41A0.StageVars2_0x365F4[v0].index_0x3647A_0)
		{
		case 3:
		case 4:
		case 5:
		case 8:
		case 9:
			v3 = world_D41A0.StageVars2_0x365F4[v0].stage_0x3647A_1;
			if (v3 & 4)
			{
				v2 = 1;
			}
			else if (v3 & 2)
			{
				if (!gameState_D41A4.creatureLists_38403x[world_D41A0.StageVars2_0x365F4[v0].target_0x3647C_4.axis.x])//fix it
					v2 = 1;
			}
			else
			{
				v4x = world_D41A0.StageVars2_0x365F4[v0].target_0x3647C_4.pointer_0x6E8E;
				if (v4x >= Entities_EA3E4[0] && v4x < Entities_EA3E4[1000])//fix
				{
					if (v4x && (v4x->life_0x8 < 0 || v4x->flags_0xc_12_15.byte[1] & 4))
						v2 = 1;
				}
			}
			if (v2)
				world_D41A0.StageVars2_0x365F4[v0].stage_0x3647A_1 |= 4u;
			//v0++;
			break;
		case 7:
			v5 = world_D41A0.StageVars2_0x365F4[v0].stage_0x3647A_1;
			if (v5 & 0x18)
			{
				if (v5 & 0x10)
					world_D41A0.StageVars2_0x365F4[v0].stage_0x3647A_1 = v5 & 0xEF;
				else
					world_D41A0.StageVars2_0x365F4[v0].stage_0x3647A_1 = v5 & 0xF7;
			}
			//v0++;
			break;
			/*default:
				v0++;
				break;*/
		}
		v0++;
	}
}

//----- (00012870) --------------------------------------------------------
void CheckStageVarsComplete_12870()//1f3870
{
	int index = 1;
	while (index <= world_D41A0.countStageVars_0x36E00)
	{
		if (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0)
		{
			switch (world_D41A0.StageVars2_0x365F4[index].index_0x3647A_0)
			{
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
				if (world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 & 4)
				{
					if (world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 & 2)
						world_D41A0.StageVars2_0x365F4[index].stage_0x3647A_1 &= 0xFB;
				}
				break;
			default:
				break;
			}
		}
		index++;
	}
}

// D4C56: using guessed type int x_DWORD_D4C56;
int debugcounter_0022B25D = 0;
//----- (0004A1E0) --------------------------------------------------------
void ResetLevelState_4A1E0(int a1, char a2)//22b1e0
{
	if (!a1)
	{
		world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2 = 0;
		memset(world_D41A0.creatureStats_0x364D2.spawnCounts_0x364D6, 0, 104);
		ClearUsedTmapFlags_716A0();
		for (int v3x = 1; v3x < 0x4b0; v3x++)
		{
			if (CommandLineParams.DoDebugSequences()) {
				//add_compare(0x22b21a, CommandLineParams.DoDebugafterload());//0x9ac
			}
			LoadEntitySprites_716C0(world_D41A0.terrain_2FECE.entity_0x30311[v3x].type_0x30311, world_D41A0.terrain_2FECE.entity_0x30311[v3x].subtype_0x30311, 0xFFFFu);
		}
		ReloadSpritesForLevel_71780();
	}
	RebuildEntityLists_49F90();
	MarkStageVarsForIndex_122C0(a1);//adress 22b257
	for (int iy = 1; iy < 0x4b0; iy++)
	{

		/*if (debugcounter_0022B25D >= 0x55)
		{
			debugcounter_0022B25D++;
			debugcounter_0022B25D--;
		}
		debugcounter_0022B25D++;*/

		//adress 22b26e
		if (CommandLineParams.DoDebugSequences()) {
			//add_compare(0x22b268, CommandLineParams.DoDebugafterload(),0x54);//0x9ac
		}
		if (world_D41A0.terrain_2FECE.entity_0x30311[iy].type_0x30311 && world_D41A0.terrain_2FECE.entity_0x30311[iy].DisId == a1)
		{//adress 22b278
			SpawnLevelEntity_4A310(&world_D41A0.terrain_2FECE.entity_0x30311[iy]);
			if (world_D41A0.terrain_2FECE.entity_0x30311[iy].type_0x30311 == 0x05)
			{
				if (world_D41A0.terrain_2FECE.entity_0x30311[iy].subtype_0x30311 < 0xC || world_D41A0.terrain_2FECE.entity_0x30311[iy].subtype_0x30311 > 0x0F && world_D41A0.terrain_2FECE.entity_0x30311[iy].subtype_0x30311 != 0x16)
					world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2++;
			}
			if (a2)
				world_D41A0.terrain_2FECE.entity_0x30311[iy].type_0x30311 = 0;
		}
	}
	CheckStageVarsComplete_12870();
	world_D41A0.freeEntityCount2_0x11e6 = -1;
}

//----- (0004A310) --------------------------------------------------------
void SpawnLevelEntity_4A310(type_entity_0x30311* entity)//22b310
{
	type_entity_0x6E8E* indexx; // eax
	type_entity_0x6E8E* v2x; // edx
	type_entity_0x6E8E* v3x; // ebx
	uint8_t v4; // al
	int v5; // eax
	__int16 v6; // di
	uint8_t v8; // al
	int v9; // eax
	uint8_t v10; // al
	axis_3d v11x; // [esp+0h] [ebp-8h]

	if (!entityTypeTables_D4C48[entity->type_0x30311].models_14[entity->subtype_0x30311].enabled_10)
		return;
	v11x.x = (entity->axis2d_4.x << 8) + 128;
	v11x.y = (entity->axis2d_4.y << 8) + 128;
	v11x.z = GetTerrainAlt_10C40(&v11x);//tady to chce pridat jeste 5012 804d80da5012
	indexx = CreateEntity_4A190(&v11x, entity->type_0x30311, entity->subtype_0x30311);
	v2x = indexx;
	v3x = indexx;
	if (!indexx)
		return;
	switch (indexx->class_0x3F_63)
	{
	case 5:
		if (indexx->model_0x40_64 == 0x16)
		{
			indexx->levelPar3_0x46_70 = entity->par1_14 & 0xff;
			AddFlyingManaSegments_4CB60(indexx);
		}
		SetStageVarForEntity_12100(entity, v3x, v3x->model_0x40_64 == 0x9);
		LinkObjectiveToEntity_58DA0(entity, v3x);
		return;
	case 0xA:
		v4 = indexx->model_0x40_64;
		if (v4 < 0x22u)
		{
			if (v4 < 0xBu)
			{
				if (v4 < 4u)
				{
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
				if (v4 <= 4u)
				{
					v2x->id_0x1A_26 = entity->stageTag_12;
					SetEntityShiftRot_49EA0(v2x, entity->par0_10 << 8, entity->par0_10 << 8);
					CopyMaxLifeToLife_49A20(v3x);
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
				if (v4 != 0x9)
				{
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
			}
			else if (v4 > 0xBu)
			{
				if (v4 < 0x11u)
				{
					if (v4 != 0xF)
					{
						LinkObjectiveToEntity_58DA0(entity, v3x);
						return;
					}
				}
				else if (v4 > 0x11 && v4 != 0x16)
				{
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
			}
		}
		else
		{
			if (v4 <= 0x22)
			{
				v2x->axis_0x9A_154x.x = (entity->par2_16 << 8) + 128;
				v2x->axis_0x9A_154x.y = (entity->par1_14 << 8) + 128;
				LinkObjectiveToEntity_58DA0(entity, v3x);
				return;
			}
			if (v4 < 0x43u)
			{
				if (v4 < 0x36u)
				{
					if (v4 == 0x2D)
					{
						InitBuildingEntity_49A30(v2x, entity->par1_14);
						v3x->xtype_0x41_65 = entity->par2_16;
					}
					LinkObjectiveToEntity_58DA0(entity, v3x);
				}
				else if (v4 <= 0x36u)
				{
					v3x->param_0x10_16 = (entity->stageTag_12 << 8) * (entity->stageTag_12 << 8);
					v5 = 8 * entity->stageTag_12 + 16;
					v3x->maxLife_0x4 = v5;
					if (v5 < 128)
						v3x->maxLife_0x4 = 128;
					CopyMaxLifeToLife_49A20(v3x);
					LinkObjectiveToEntity_58DA0(entity, v3x);
				}
				else
				{
					if (v4 < 0x3Du)
					{
						LinkObjectiveToEntity_58DA0(entity, v3x);
						return;
					}
					if (v4 <= 0x3Eu)
						goto LABEL_49;
					LinkObjectiveToEntity_58DA0(entity, v3x);
				}
				return;
			}
			if (v4 > 0x43u)
			{
				if (v4 >= 0x53u)
				{
					if (v4 <= 0x53u)
					{
						v2x->axis_0x9A_154x.x = entity->par0_10;
						LinkObjectiveToEntity_58DA0(entity, v3x);
					}
					else
					{
						if (v4 <= 0x55u)
						{
							v6 = v2x->position_0x4C_76.y;
							v2x->position_0x4C_76.x -= 128;
							v2x->position_0x4C_76.y = v6 - 128;
							v2x->axis_0x9A_154x.x = entity->par0_10;
							v2x->position_0x4C_76.z = entity->par3_18;
						}
						LinkObjectiveToEntity_58DA0(entity, v3x);
					}
					return;
				}
				if (v4 != 0x47)
				{
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
			}
		}
		v3x->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(v3x->model_0x40_64)].subspell[entity->par1_14].subSpellIndex_2;
		v8 = v3x->model_0x40_64;
		if (v8 >= 0x11u)
		{
			if (v8 <= 0x11u)
			{
				v9 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(v3x->model_0x40_64)].subspell[entity->par1_14].life_0x1A;
			}
			else
			{
				if (v8 >= 0x43u)
				{
					if (v8 > 0x43u && v8 != 0x47)
					{
						LinkObjectiveToEntity_58DA0(entity, v3x);
					}
				LABEL_65:
					v9 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(v3x->model_0x40_64)].subspell[entity->par1_14].life_0x1A;
				LABEL_69:
					v3x->life_0x8 = v9;
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
				if (v8 != 0x16)
				{
					LinkObjectiveToEntity_58DA0(entity, v3x);
					return;
				}
				v9 = 8 * SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(v3x->model_0x40_64)].subspell[entity->par1_14].life_0x1A;
			}
			v3x->maxLife_0x4 = v9;
			goto LABEL_69;
		}
		if (v8 >= 0xBu)
		{
			if (v8 > 0xBu && v8 != 0xF)
			{
				LinkObjectiveToEntity_58DA0(entity, v3x);
				return;
			}
			goto LABEL_65;
		}
		if (v8 != 0x9)
		{
			LinkObjectiveToEntity_58DA0(entity, v3x);
			return;
		}
		v3x->maxLife_0x4 = SPELLS_BEGIN_BUFFER_str[GetSpellIndex_6E020(v3x->model_0x40_64)].subspell[entity->par1_14].life_0x1A;
		LinkObjectiveToEntity_58DA0(entity, v3x);
		return;
	case 0xB:
		indexx->id_0x1A_26 = entity->stageTag_12;
		if (indexx->model_0x40_64 == 0x20)
			indexx->levelPar3_0x46_70 = entity->par1_14;
		else
			SetEntityShiftRot_49EA0(indexx, entity->par0_10 << 8, 4096);
		CopyMaxLifeToLife_49A20(v3x);
		v3x->flags_0xc_12_15.byte[0] |= 1u;
		LinkObjectiveToEntity_58DA0(entity, v3x);
		return;
	case 0xC:
	case 0xF:
		indexx->actionIndex_0x45_69 += entity->stageTag_12;
		if (entity->stageTag_12 < 3u)
		{
			LinkObjectiveToEntity_58DA0(entity, v3x);
			return;
		}
		indexx->actionIndex_0x45_69 = 253;
		LinkObjectiveToEntity_58DA0(entity, indexx);
		return;
	case 0xE:
		v10 = indexx->model_0x40_64;
		if (v10 < 1u)
		{
			LinkObjectiveToEntity_58DA0(entity, v3x);
			return;
		}
		if (v10 <= 1u)
		{
		LABEL_49:
			v2x->levelPar3_0x46_70 = entity->par1_14;
			v2x->param_0x10_16 = entity->par2_16;
			LinkObjectiveToEntity_58DA0(entity, v3x);
		}
		else
		{
			if (v10 == 2)
			{
				v2x->param2_0x2C_44 = entity->par1_14;
				v2x->targetId_0x96_150 = entity->par3_18;
				LinkObjectiveToEntity_58DA0(entity, v3x);
				return;
			}
			LinkObjectiveToEntity_58DA0(entity, v3x);
		}
		return;
	default:
	{
		LinkObjectiveToEntity_58DA0(entity, v3x);
		return;
	}
	}
}

//----- (0004EB50) --------------------------------------------------------
type_entity_0x6E8E* ArriveCheckpoint_4EB50(axis_3d* position)//22fb50
{
	type_entity_0x6E8E* tempevent; // eax
	if (GetFreeEntityCount_4A810() < 32)
		return 0;
	tempevent = NewEvent_4A050();
	if (!tempevent)
		return 0;
	tempevent->actionIndex_0x45_69 = 0x40;
	tempevent->class_0x3F_63 = 0xA;
	tempevent->model_0x40_64 = 0x3B;
	tempevent->rand_0x14_20 = 9377 * tempevent->rand_0x14_20 + 9439;
	tempevent->maxLife_0x4 = tempevent->rand_0x14_20 % 0x64u + 800;
	tempevent->flags_0xc_12_15.byte[0] = (tempevent->flags_0xc_12_15.byte[0] & 0xF6) | 1;
	tempevent->rand_0x14_20 = 9377 * tempevent->rand_0x14_20 + 9439;
	tempevent->actSpeed_0x82_130 = tempevent->rand_0x14_20 % 0x11u;
	tempevent->position_0x4C_76 = *position;
	CopyMaxLifeToLife_49A20(tempevent);
	return tempevent;
}

//----- (00050A90) --------------------------------------------------------
type_entity_0x6E8E* AddSwitchXX_50A90(axis_3d* position, char a2, char a3)//231a90 - add spell
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->class_0x3F_63 = 0xB;
		event->model_0x40_64 = a2;
		event->actionIndex_0x45_69 = a3;
		event->flags_0xc_12_15.byte[0] &= 0xF6u;
		event->param_0x10_16 = 0;
		event->flags_0xc_12_15.byte[0] |= 1;
		event->position_0x4C_76 = *position;
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (00050AE0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch00_50AE0(axis_3d* position)//231ae0
{
	return AddSwitchXX_50A90(position, 0, 0);
}

//----- (00050B00) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch01_50B00(axis_3d* position)//231b00
{
	return AddSwitchXX_50A90(position, 1, 1);
}

//----- (00050B20) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch02_50B20(axis_3d* position)//231b20
{
	return AddSwitchXX_50A90(position, 2, 2);
}

//----- (00050B40) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch03_50B40(axis_3d* position)//231b40
{
	return AddSwitchXX_50A90(position, 3, 3);
}

//----- (00050B60) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch04_50B60(axis_3d* position)//231b60
{
	return AddSwitchXX_50A90(position, 4, 4);
}

//----- (00050B80) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch32_50B80(axis_3d* a1x)//231b80
{
	return AddSwitchXX_50A90(a1x, 32, 32);
}

//----- (00050C10) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch12_50C10(axis_3d* position)//231c10
{
	return AddSwitchXX_50A90(position, 12, 12);
}

//----- (00050C30) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch13_50C30(axis_3d* a1x)//231c30
{
	return AddSwitchXX_50A90(a1x, 13, 13);
}

//----- (00050C50) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch14_50C50(axis_3d* position)//231c50
{
	return AddSwitchXX_50A90(position, 14, 14);
}

//----- (00050C70) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch15_50C70(axis_3d* position)//231c70
{
	return AddSwitchXX_50A90(position, 15, 15);
}

//----- (00050C90) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch16_50C90(axis_3d* position)//231c90
{
	return AddSwitchXX_50A90(position, 16, 16);
}

//----- (00050CB0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch17_50CB0(axis_3d* position)//231cb0
{
	return AddSwitchXX_50A90(position, 17, 17);
}

//----- (00050CD0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch18_50CD0(axis_3d* position)//231cd0
{
	return AddSwitchXX_50A90(position, 18, 18);
}

//----- (00050CF0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch19_50CF0(axis_3d* position)//231cf0
{
	return AddSwitchXX_50A90(position, 19, 19);
}

//----- (00050D10) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch20_50D10(axis_3d* position)//231d10
{
	return AddSwitchXX_50A90(position, 20, 20);
}

//----- (00050D30) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch21_50D30(axis_3d* position)//231d30
{
	return AddSwitchXX_50A90(position, 21, 21);
}

//----- (00050D50) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch22_50D50(axis_3d* position)//231d50
{
	return AddSwitchXX_50A90(position, 22, 22);
}

//----- (00050D70) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch23_50D70(axis_3d* position)//231d70
{
	return AddSwitchXX_50A90(position, 23, 23);
}

//----- (00050D90) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch24_50D90(axis_3d* position)//231d90
{
	return AddSwitchXX_50A90(position, 24, 24);
}

//----- (00050DB0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch25_50DB0(axis_3d* a1x)//231db0
{
	return AddSwitchXX_50A90(a1x, 25, 25);
}

//----- (00050DD0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch26_50DD0(axis_3d* position)//231dd0
{
	return AddSwitchXX_50A90(position, 26, 26);
}

//----- (00050DF0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch27_50DF0(axis_3d* position)//231df0
{
	return AddSwitchXX_50A90(position, 27, 27);
}

//----- (00050E10) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch28_50E10(axis_3d* position)//231e10
{
	return AddSwitchXX_50A90(position, 28, 28);
}

//----- (00050E30) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch29_50E30(axis_3d* position)//231e30
{
	return AddSwitchXX_50A90(position, 29, 29);
}

//----- (00050E50) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch33_50E50(axis_3d* position)//231e50
{
	return AddSwitchXX_50A90(position, 33, 33);
}

//----- (00050E70) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch34_50E70(axis_3d* position)//231e70
{
	return AddSwitchXX_50A90(position, 34, 34);
}

//----- (00050E90) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch35_50E90(axis_3d* position)//231e90
{
	return AddSwitchXX_50A90(position, 35, 35);
}

//----- (00050EB0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch36_50EB0(axis_3d* position)//231eb0
{
	return AddSwitchXX_50A90(position, 36, 36);
}

//----- (00050ED0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch37_50ED0(axis_3d* position)//231ed0
{
	return AddSwitchXX_50A90(position, 37, 37);
}

//----- (00050EF0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch38_50EF0(axis_3d* position)//231ef0
{
	return AddSwitchXX_50A90(position, 38, 38);
}

//----- (00050F10) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch39_50F10(axis_3d* position)//231f10
{
	return AddSwitchXX_50A90(position, 39, 39);
}

//----- (00050F30) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch40_50F30(axis_3d* position)//231f30
{
	return AddSwitchXX_50A90(position, 40, 40);
}

//----- (00050F50) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch41_50F50(axis_3d* position)//231f50
{
	return AddSwitchXX_50A90(position, 41, 41);
}

//----- (00050F70) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch42_50F70(axis_3d* position)//231f70
{
	return AddSwitchXX_50A90(position, 42, 42);
}

//----- (00050F90) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch43_50F90(axis_3d* position)//231f90
{
	return AddSwitchXX_50A90(position, 43, 43);
}

//----- (00050FB0) --------------------------------------------------------
type_entity_0x6E8E* AddCheckpoint44_50FB0(axis_3d* position)//231fb0
{
	return AddSwitchXX_50A90(position, 44, 44);
}

//----- (00050FD0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch30_50FD0(axis_3d* position)//231fd0
{
	return AddSwitchXX_50A90(position, 30, 30);
}

//----- (00050FF0) --------------------------------------------------------
type_entity_0x6E8E* AddSwitch31atyp_50FF0(axis_3d* position)//231ff0
{
	type_entity_0x6E8E* event = 0;
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && !(gameState_D41A4.setting_38545 & 8))
	{
		event = AddSwitchXX_50A90(position, 31, 31);
		if (event)
		{
			world_D41A0.tempObjective_0x36E02 = 1;
			world_D41A0.gameFlags_0x36E0B |= 1;
		}
	}
	return event;
}

//----- (000533B0) --------------------------------------------------------
char LevelDecompress_533B0(int16 levelIndex, Type_Level_2FECE* levelData, std::string customLevelPath)//2343b0
{
	if (levelIndex < 1000)
	{
		std::string levelDataPath = LevelsDirectory() + "/LEVELS.DAT";
		FILE* levelsdatfile = DataFileIO::CreateOrOpenFile(levelDataPath.c_str(), 512);
		if (levelsdatfile == NULL)
		{
			levelDataPath = GetSubDirectoryFile(cdFolder.c_str(), "LEVELS", "LEVELS.DAT");
			levelsdatfile = DataFileIO::CreateOrOpenFile(levelDataPath.c_str(), 512);
			if (levelsdatfile == NULL)
				return 0;
		}
		levelDataPath = LevelsDirectory() + "/LEVELS.TAB";
		FILE* levelstabfile = DataFileIO::CreateOrOpenFile(levelDataPath.c_str(), 512);
		if (levelstabfile == NULL)
		{
			levelDataPath = GetSubDirectoryFile(cdFolder.c_str(), "LEVELS", "LEVELS.TAB");
			levelstabfile = DataFileIO::CreateOrOpenFile(levelDataPath.c_str(), 512);
			if (levelstabfile == NULL)
			{
				DataFileIO::Close(levelsdatfile);
				return 0;
			}
		}
		DataFileIO::Read(levelstabfile, smalltitBuffer_E9C38, 4000);
		DataFileIO::Close(levelstabfile);

		if (DataFileIO::FileLengthBytes(levelsdatfile))
		{
			DataFileIO::Seek(levelsdatfile, ((uint32*)smalltitBuffer_E9C38)[levelIndex], 0);
			DataFileIO::Read(levelsdatfile, (uint8_t*)smalltitBuffer_E9C38, ((uint32*)smalltitBuffer_E9C38)[levelIndex + 1] - ((uint32*)smalltitBuffer_E9C38)[levelIndex]);
			if (DataFileRNC::Decompress((uint8_t*)smalltitBuffer_E9C38, (uint8_t*)smalltitBuffer_E9C38) < 0)
			{
				Logger->error("ERROR decompressing LEVELS.DAT\n");
				return 0;
			}
			Type_CompressedLevel_2FECE shadow_2FECE;
			qmemcpy(&shadow_2FECE, (Type_CompressedLevel_2FECE*)(const void*)smalltitBuffer_E9C38, sizeof(Type_CompressedLevel_2FECE));//0x6604
			memset((Type_CompressedLevel_2FECE*)smalltitBuffer_E9C38, 0, sizeof(Type_CompressedLevel_2FECE));//0x6604
			DecompressLevel_2FECE(&shadow_2FECE, levelData);
		}
		DataFileIO::Close(levelsdatfile);

		//if exist editor generated level
		if (customLevelPath.length() > 0)
		{
			FILE* file = fopen(customLevelPath.c_str(), "rb");
			if (file)
			{
				Type_CompressedLevel_2FECE shadow_2FECE;
				fread(&shadow_2FECE, sizeof(Type_CompressedLevel_2FECE), 1, file);
				/*for (int i = 0; i < sizeof(type_shadow_str_2FECE); i++)
					if(((int8_t*)&shadow_2FECE)[i]!=((int8_t*)&D41A0_BYTESTR_0.terrain_2FECE)[i])
						allert_error();*/
				DecompressLevel_2FECE(&shadow_2FECE, &world_D41A0.terrain_2FECE);
			}
			fclose(file);
		}
		//if exist editor generated level

		LevelInit_56C00(levelData);
		SetLevelId_53590(levelData);
	}
	return 1;
}

//----- (00053770) --------------------------------------------------------
char TestOpenMovieGam_53770(uint16_t gamnumber)//234770
{
	FILE* gamxfile; // eax
	sprintf(printbuffer, "%s/GAM%05d.DAT", "MOVIE", gamnumber);
	gamxfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (gamxfile == NULL)
		return 0;
	DataFileIO::Close(gamxfile);
	return 1;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);

//----- (00053950) --------------------------------------------------------
char TestOpenMovieMap_53950(uint16_t mapnumeber)//234950
{
	FILE* mapxfile; // eax
	sprintf(printbuffer, "%s/MAP%05d.DAT", "MOVIE", mapnumeber);
	mapxfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (mapxfile == NULL)
		return 0;
	DataFileIO::Close(mapxfile);
	return 1;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);

//----- (000539A0) --------------------------------------------------------
void LoadBldgPrm_539A0()//2349a0
{
	char bldPath[MAX_PATH];

	sprintf(bldPath, "%s/%s", cdDataPath.c_str(), "DATA/BLDGPRM.DAT\0");

	FILE* bldgprmfile = DataFileIO::CreateOrOpenFile(bldPath, 512);
	if (bldgprmfile)
	{
		DataFileIO::Read(bldgprmfile, (uint8_t*)&buildingParams_D93C0[0], 76 * sizeof(type_D93C0_bldgprmbuffer));
		memcpy(&buildingParams_D93C0[76], frameColors_D94F0[0], sizeof(type_D93C0_bldgprmbuffer));//0x2AA4F0 index 76 = byte_D94F0
		DataFileIO::Close(bldgprmfile);
	}
}
// D93C0: using guessed type __int16 x_WORD_D93C0_bldgprmbuffer[];

//----- (00053A40) --------------------------------------------------------
void RecordOrPlayMovieInput_53A40(Type_PlayerInput_0x6E3E* a1x)//234a40
{
	char v1; // ah
	//FILE* moviemvidatfile; // eax
	//int v3; // edx
	//int v4; // edx
	char v5; // al
	//FILE* v6; // ecx
	char v7; // dh
	//FILE* v8; // edi
	//FILE* moviemvidatfile2; // eax
	//int v10; // edx
	//int v11; // edx
	char v12; // ch
	//FILE* v13; // eax
	//char v14; // [esp+0h] [ebp-48h]
	char v15; // [esp+40h] [ebp-8h]
	char v16; // [esp+44h] [ebp-4h]

	v1 = gameState_D41A4.setting_byte1_22;
	if (v1 & 4)
	{
		v15 = 0;
		if (!gameState_D41A4.moviemvidatfile_byte4_35 && !((a1x - &world_D41A0.playerInputs_0x6E3E[0]) / sizeof(Type_PlayerInput_0x6E3E)))
		{
			sprintf(printbuffer, "%s/MVI%05d.DAT", "MOVIE", gameState_D41A4.moviemvinumber_byte4_39);
			//moviemvidatfile = sub_98817_open(printbuffer, 512);
			//v3 = (int)x_D41A0_BYTEARRAY_4;
			gameState_D41A4.moviemvidatfile_byte4_35 = DataFileIO::CreateOrOpenFile(printbuffer, 512);
			if (gameState_D41A4.moviemvidatfile_byte4_35 == NULL || !SaveSMAPSLEVmovie_54D30(gameState_D41A4.moviemvinumber_byte4_39))
				v15 = 1;
			if (v15)
			{
				//v4 = (int)x_D41A0_BYTEARRAY_4;
				v5 = gameState_D41A4.setting_byte1_22;
				gameState_D41A4.moviemvidatfile_byte4_35 = 0;
				gameState_D41A4.setting_byte1_22 = v5 & 0xFB;
			}
			else
			{
				StartMovieRecording_53C70();
			}
		}
		if (v15)
			goto LABEL_21;
		//v6 = gameState_D41A4.moviemvidatfile_byte4_33;
		if (!gameState_D41A4.moviemvidatfile_byte4_35)
			goto LABEL_21;
		if (a1x->PlayerAction_byte0 != 2 && DataFileIO::Read(gameState_D41A4.moviemvidatfile_byte4_35, (uint8_t*)a1x, 10) == 10)
		{
			v7 = a1x->PlayerAction_byte0;
			if (a1x->PlayerAction_byte0 == 13)
			{
				a1x->PlayerAction_byte0 = 0;
				goto LABEL_21;
			}
			if (v7 != 2 && v7 != 29 && !HasSkipInput_473E0())
				goto LABEL_21;
		}
		v15 = 1;
	LABEL_21:
		if (v15)
		{
			CloseMovie_53CC0();
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] = 8;
			StopMovieRecording_53CA0();
			a1x->PlayerAction_byte0 = 0;
		}
		return;
	}
	if (v1 & 2)
	{
		//v8 = gameState_D41A4.moviemvidatfile_byte4_33;
		v16 = 0;
		if (!gameState_D41A4.moviemvidatfile_byte4_35 && !((a1x - &world_D41A0.playerInputs_0x6E3E[0]) / sizeof(Type_PlayerInput_0x6E3E)))
		{
			sprintf(printbuffer, "%s/MVI%05d.DAT", "MOVIE", gameState_D41A4.moviemvinumber_byte4_39);
			//moviemvidatfile2 = sub_98817_open(printbuffer, 546);
			//v10 = (int)x_D41A0_BYTEARRAY_4;
			gameState_D41A4.moviemvidatfile_byte4_35 = DataFileIO::CreateOrOpenFile(printbuffer, 546);
			if (gameState_D41A4.moviemvidatfile_byte4_35 == NULL || !SaveSMAPSLEVmovie2_54F00(gameState_D41A4.moviemvinumber_byte4_39))
				v16 = 1;
			if (v16)
			{
				//v11 = (int)x_D41A0_BYTEARRAY_4;
				v12 = gameState_D41A4.setting_byte1_22;
				gameState_D41A4.moviemvidatfile_byte4_35 = 0;
				gameState_D41A4.setting_byte1_22 = v12 & 0xFD;
			}
			else
			{
				StartMovieRecording_53C70();
			}
		}
		if (!v16)
		{
			//v13 = gameState_D41A4.moviemvidatfile_byte4_33;
			if (gameState_D41A4.moviemvidatfile_byte4_35)
			{
				if (DataFileIO::WriteFile_98CAA(gameState_D41A4.moviemvidatfile_byte4_35, (uint8_t*)a1x, 10) != 10)
					CloseMovie_53CC0();
				if (a1x->PlayerAction_byte0 == 12)
					a1x->PlayerAction_byte0 = 0;
			}
		}
	}
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00053C70) --------------------------------------------------------
void StartMovieRecording_53C70()//234c70
{
	//int result; // eax
	//int v1; // edx

	//result = (int)x_D41A0_BYTEARRAY_4;
	gameState_D41A4.FrameTimingIndex_26 = 0;
	//v1 = gameState_D41A4.setting_byte1_22;
	gameState_D41A4.setting_30 = 0;
	gameState_D41A4.setting_byte1_22 |= 0x1208000;
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00053CA0) --------------------------------------------------------
void StopMovieRecording_53CA0()//234ca0
{
	//int result; // eax

	//result = (int)x_D41A0_BYTEARRAY_4;
	gameState_D41A4.setting_byte1_22 &= 0xFEDF7FFF;
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00053CC0) --------------------------------------------------------
void CloseMovie_53CC0()//234cc0
{
	//int result; // eax
	//char v1; // dl

	//result = (int)x_D41A0_BYTEARRAY_4;
	if (gameState_D41A4.moviemvidatfile_byte4_35)
	{
		DataFileIO::Close(gameState_D41A4.moviemvidatfile_byte4_35);
		//result = (int)x_D41A0_BYTEARRAY_4;
		//v1 = gameState_D41A4.setting_byte1_22;
		gameState_D41A4.moviemvidatfile_byte4_35 = 0;
		gameState_D41A4.setting_byte1_22 &= 0xD9;
	}
	//return result;
}
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00053D10) --------------------------------------------------------
uint8_t CreateNetherSubdir_53D10(const std::string& gameDir, const std::string& subDir)
{
	char result; // al
	if ((signed __int16)FileAccess_53CF0(gameDir.c_str()) <= -1 && mymkdir(gameDir.c_str()))//234D6A - 234CF0 | 279D30
		return 2;
	std::string fullDir = std::string(gameDir) + "/" + std::string(subDir);
	if ((FileAccess_53CF0(fullDir.c_str()) & 0x8000u) != 0 && mymkdir(fullDir.c_str()))
		result = 2;
	else
		result = 3;
	return result;
}
// 8C1E8: using guessed type x_DWORD dos_getdiskfree(x_DWORD, x_DWORD);
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 98D30: using guessed type x_DWORD mkdir(x_DWORD);
// 53D10: using guessed type char var_98[144];

//----- (00053E60) --------------------------------------------------------
// 988DA: using guessed type x_DWORD filelength(x_DWORD);

//----- (00054200) --------------------------------------------------------
char CreateUserDirectories_54200()//235200
{
	int8_t v0l; // bx
	int8_t v0h; // bx
	//int v1; // eax
	//int v2; // edx
	//int v3; // eax
	//int v4; // edx
	//int v5; // eax
	//int v6; // edx
	//int v7; // eax
	//int v8; // edx
	//char* buffer; // [esp+0h] [ebp-2h]
	//char v11; // [esp+40h] [ebp+3Eh]
	//int v12; // [esp+80h] [ebp+7Eh]

	// fix if begin
	//v2 = 0;
	//v4 = 0;
	//v6 = 0;
	v0h = 0;
	//v8 = 0;

	readbuffer[0] = 0x0;
	// end

	if (PackedGameData())//regression tests without the game: no TMAPS to install into CDATA
		return 1;
	printbuffer[0] = 0;
	outtext("Checking Setup Version ..");//23521B - 29EBED
	v0l = 1;
	std::string versionPath = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "VERSION.DAT");//235250 - 26F3D5
	DataFileIO::ReadFileAndDecompress(versionPath.c_str(), &readbuffer);//235260 - 234E60
	if (readbuffer[0] != 60)
		v0h = 1;
	outtext("\n");//235277 - 29EBED
	std::string cDataTmapsPath0 = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS0-0.DAT");//2352A8 - 26F3D5
	std::string dataTmapsPath0 = GetSubDirectoryFile(cdFolder.c_str(), "DATA", "TMAPS0-0.DAT");//2352BE - 26F3D5
	if (v0h || IsEitherFileMissing_53EF0(cDataTmapsPath0.c_str(), dataTmapsPath0.c_str()))
	{
		skipScreen_D41AD = 0;//fix can not exit from setup
		//fix it - whne file not exist
		outtext("Creating Setup Directories 1 ..");//2352E4 -29EBED
		textPosition_F4720 = gettextposition();//2352EC - 29E953

		std::string cDataTmapsPath = GetSubDirectoryPath(gameFolder.c_str(), "CDATA");//23531A - 26F3D5
		std::string dataTmapsPath = GetSubDirectoryPath(cdFolder.c_str(), "DATA");//23531A - 26F3D5
		if (LoadFilesFromCDAndGameData(dataTmapsPath.c_str(), cDataTmapsPath.c_str(), "TMAPS0-0"))// this needs to create something // 235330 -234F80 // create tmaps00 tab / data
			v0l = 0;
		outtext("\n");//235343 - 29EBED
	}
	if (v0l)
	{
		std::string cDataTmapsPath1 = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS1-0.DAT");//23537C - 26F3D5
		std::string dataTmapsPath1 = GetSubDirectoryFile(cdFolder.c_str(), "DATA", "TMAPS1-0.DAT");//235392 - 26F3D5
		if (v0h || IsEitherFileMissing_53EF0(cDataTmapsPath1.c_str(), dataTmapsPath1.c_str()))
		{
			//fix it - whne file not exist
			outtext("Creating Setup Directories 2 ..");
			textPosition_F4720 = gettextposition(/*v3, v4, v0*/);
			std::string cDataTmapsPath = GetSubDirectoryPath(gameFolder.c_str(), "CDATA");//2353EE - 26F3D5
			std::string dataTmapsPath = GetSubDirectoryPath(cdFolder.c_str(), "DATA");//23531A - 26F3D5
			if (LoadFilesFromCDAndGameData(dataTmapsPath.c_str(), cDataTmapsPath.c_str(), "TMAPS1-0"))//235404 - 234F80
				v0l = 0;
			outtext("\n");
		}
	}
	if (v0l)
	{
		std::string cDataTmapsPath2 = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS2-0.DAT");
		std::string dataTmapsPath2 = GetSubDirectoryFile(cdFolder.c_str(), "DATA", "TMAPS2-0.DAT");
		if (v0h || IsEitherFileMissing_53EF0(cDataTmapsPath2.c_str(), dataTmapsPath2.c_str()))
		{
			//fix it - whne file not exist
			outtext("Creating Setup Directories 3 ..");
			textPosition_F4720 = gettextposition(/*v5, v6, v0*/);
			std::string cDataTmapsPath = GetSubDirectoryPath(gameFolder.c_str(), "CDATA");
			std::string dataTmapsPath = GetSubDirectoryPath(cdFolder.c_str(), "DATA");
			if (LoadFilesFromCDAndGameData(dataTmapsPath.c_str(), cDataTmapsPath.c_str(), "TMAPS2-0"))
				v0l = 0;
			outtext("\n");
		}
	}
	if (v0l && !((gameState_D41A4.setting_byte1_22) & 8))//test it
	{
		outtext("Setting Up Levels ..");
		textPosition_F4720 = gettextposition(/*v7, v8, v0*/);
		std::string clevelsPath = LevelsDirectory();
		std::string levelPath = GetSubDirectoryPath(cdFolder.c_str(), "LEVELS");
		if (LoadFilesFromCDAndGameData(levelPath.c_str(), clevelsPath.c_str(), "LEVELS"))
			v0l = 0;
		outtext("\n");
	}
	if (v0l && v0h)
	{
		versionPath = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "VERSION.DAT");
		readbuffer[0] = 60;
		DataFileIO::OpenWriteClose_98C48(versionPath.c_str(), readbuffer, 4);
	}

	return v0l;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// BD953: using guessed type int /*__fastcall*/ gettextposition(x_DWORD, x_DWORD, x_DWORD);
// BDBED: using guessed type x_DWORD outtext(x_DWORD);
// D41A4: using guessed type int x_DWORD_D41A4;
// F4720: using guessed type int textPosition_F4720;

//----- (00054D30) --------------------------------------------------------
bool SaveSMAPSLEVmovie_54D30(__int16 a1)//235d30 //in game load
{
	FILE* savedfile;
	type_str_0x21AA temp0x21AA;
	type_str_0x21AE temp0x21AE;
	type_str_0x21B2 temp0x21B2;
	type_str_0x21B6 temp0x21B6;
	int temp0x219A;
	int temp0x219E;
	int temp0x21A2;
	int temp0x21A6;
	GraphicsStruct_t graphicsSettings;
	DisplayStruct_t displaySettings;
	type_str_0x2192 tempGraphics;
	type_str_0x2196 tempDispl;

	bool result = true;
	graphicsSettings = world_D41A0.m_GameSettings.m_Graphics;
	displaySettings = world_D41A0.m_GameSettings.m_Display;
	tempGraphics = world_D41A0.m_GameSettings.graphics_0x2192;
	tempDispl = world_D41A0.m_GameSettings.display_0x2196;
	temp0x219A = world_D41A0.dword_0x219A;
	temp0x219E = world_D41A0.dword_0x219E;
	temp0x21A2 = world_D41A0.dword_0x21A2;
	temp0x21A6 = world_D41A0.dword_0x21A6;
	temp0x21AA = world_D41A0.visualSettings_0x21AA;
	temp0x21AE = world_D41A0.miscSettings_0x21AE;
	temp0x21B2 = world_D41A0.screenSettings_0x21B2;
	temp0x21B6 = world_D41A0.detailSettings_0x21B6;
	sprintf(printbuffer, "%s/%s%03d.DAT", "MOVIE", "SMAP", a1);
	savedfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	if (savedfile)
	{
		DataFileIO::Read(savedfile, (uint8_t*)mapTerrainType_10B4E0, 0x10000);
		DataFileIO::Read(savedfile, (uint8_t*)mapHeightmap_11B4E0, 0x10000);
		DataFileIO::Read(savedfile, (uint8_t*)mapShading_12B4E0, 0x10000);
		DataFileIO::Read(savedfile, (uint8_t*)mapAngle_13B4E0, 0x10000);
		DataFileIO::Read(savedfile, (uint8_t*)secondHeightmap_14B4E0, 0x10000);
		DataFileIO::Read(savedfile, (uint8_t*)mapEntityIndex_15B4E0, 0x20000);
		DataFileIO::Read(savedfile, (uint8_t*)building_F2CD0x, 4802);
		DataFileIO::Close(savedfile);
	}
	sprintf(printbuffer, "%s/%s%03d.DAT", "MOVIE", "SLEV", a1);

	uint8_t* D41A0_pointer;
	D41A0_pointer = (uint8_t*)&world_D41A0;
	DataFileIO::ReadFileAndDecompress(printbuffer, &D41A0_pointer);

	ConvertObjectivePointers_55100(2);
	FixPointersAfterLoad_57680();
	RebuildEntityLists_49F90();
	world_D41A0.freeEntityCount2_0x11e6 = -1;
	ReloadLevelTmaps_71990();
	FlvInitSet_473B0();
	world_D41A0.m_GameSettings.m_Graphics = graphicsSettings;
	world_D41A0.m_GameSettings.m_Display = displaySettings;
	world_D41A0.m_GameSettings.graphics_0x2192 = tempGraphics;
	world_D41A0.m_GameSettings.display_0x2196 = tempDispl;
	world_D41A0.dword_0x219A = temp0x219A;
	world_D41A0.dword_0x219E = temp0x219E;
	world_D41A0.dword_0x21A2 = temp0x21A2;
	world_D41A0.dword_0x21A6 = temp0x21A6;
	world_D41A0.visualSettings_0x21AA = temp0x21AA;
	world_D41A0.miscSettings_0x21AE = temp0x21AE;
	world_D41A0.screenSettings_0x21B2 = temp0x21B2;
	world_D41A0.detailSettings_0x21B6 = temp0x21B6;
	return result;
}

//----- (00054F00) --------------------------------------------------------
bool SaveSMAPSLEVmovie2_54F00(__int16 a1)//235f00 //in game save
{
	bool success = true;
	MarkLoadedTmaps_71930();
	ConvertObjectivePointers_55100(1);
	sprintf(printbuffer, "%s/%s%03d.DAT", "MOVIE", "SLEV", a1);
	world_D41A0.typeParamsBase_0x36DF6 = &str_D7BD6[59];

	int size = sizeof(type_shadow_D41A0_BYTESTR_0);
	DataFileIO::OpenWriteClose_98C48(printbuffer, (uint8_t*)&world_D41A0, size);

	sprintf(printbuffer, "%s/%s%03d.DAT", "MOVIE", "SMAP", a1);
	FILE* file = DataFileIO::CreateOrOpenFile(printbuffer, 546);
	if (file != nullptr)
	{
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)mapTerrainType_10B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)mapHeightmap_11B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)mapShading_12B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)mapAngle_13B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)secondHeightmap_14B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)mapEntityIndex_15B4E0, 0x20000);
		DataFileIO::WriteFile_98CAA(file, (uint8_t*)building_F2CD0x, 4802);
		DataFileIO::Close(file);
	}
	ConvertObjectivePointers_55100(2);
	RebuildEntityLists_49F90();
	world_D41A0.freeEntityCount2_0x11e6 = -1;
	return success;
}

//----- (00056D60) --------------------------------------------------------
void LoadLevel_56D60(unsigned int a1, char a2)//237d60
{
	char v2; // bl
	//uint8_t* v3; // ST08_4
	//uint8_t* v4; // eax
	//type_entity_0x6E8E* v5x; // ST08_4

	v2 = soundActive_E3799;
	soundActive_E3799 = false;
	ClearSettings_567C0();
	LevelDecompress_533B0(gameState_D41A4.levelnumber_43w, &world_D41A0.terrain_2FECE);
	if (a2)
	{
		ReadAndDecompressSkyAndBlocks_54660(world_D41A0.terrain_2FECE.MapType, TileTextureSize_D41B5);
		ReadAndDecompressTables_54800(world_D41A0.terrain_2FECE.MapType);
	}
	//v3 = x_D41A0_BYTEARRAY_0 + 196302;
	world_D41A0.NumberOfPlayers_0xe = world_D41A0.terrain_2FECE.numberOfPlayers_0x2FED7;
	GenerateLevelMap_43830(&world_D41A0.terrain_2FECE);
	ResetEntityLists_49F30();
	GenerateLevelFeatures_49270(&world_D41A0.terrain_2FECE);
	memset(&predictedAxis_EB398ar, 0, 6);
	RebuildEntityLists_49F90();
	//v4 = x_D41A0_BYTEARRAY_0;
	world_D41A0.freeEntityCount2_0x11e6 = -1;
	if (a2)
		SetTmapsFile_71A70(world_D41A0.terrain_2FECE.MapType);
	InitStages_58940();
	InitStageVars_11EE0();
	ClearDynamicLights_84790();
	ResetLevelState_4A1E0(0, 1);
	soundActive_E3799 = v2;
	UpdateGameFrame_53160();
	//v5x = Entities_EA3E4[world_D41A0.array_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	UpdateCastles_60F00();
	RecordingLevelSave();
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// D41B5: using guessed type char x_BYTE_D41B5;
// E3799: using guessed type char x_BYTE_E3799_sound_card;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (00057570) --------------------------------------------------------
void CheckLevelEndReached_57570()//238570
{
	if (!(gameState_D41A4.setting_byte2_23 & 1))
	{
		if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
		{
			if (!(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 0x10))
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] | 2;
		}
	}
}

//----- (000575C0) --------------------------------------------------------
void CheckLevelEndFlags_575C0()//2385c0
{
	if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 8)
	{
		if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] & 4)
		{
			if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && (gameState_D41A4.setting_38545 & 0x80))
			{
				if (LoadLevel_555D0(1u, gameState_D41A4.levelnumber_43w))
				{
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] &= 0xF7u;
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] &= 0xFBu;
				}
			}
		}
	}
}

//----- (00057640) --------------------------------------------------------
void AutoSaveLevel_57640()//238640
{
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE) && !(gameState_D41A4.setting_38545 & 0x80))
	{
		if (SaveLevel_55080(1u, gameState_D41A4.levelnumber_43w, (char*)""))
		{
			gameState_D41A4.setting_38545 |= 0x80u;
		}
	}
}

//----- (00058940) --------------------------------------------------------
void InitStages_58940()//239940 //init games objectives
{
	Type_Level_2FECE* terrain = &world_D41A0.terrain_2FECE;
	world_D41A0.stageIndex_0x36E01 = 0;
	memset(world_D41A0.playerObjectives_0x3659C, 0, sizeof(type_str_3654C) * 8);
	memset(world_D41A0.stages_0x3654C, 0, sizeof(type_str_3654C) * 8);
	world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 = 0;
	if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
	{
		world_D41A0.stageIndex_0x36E01 = 1;
		world_D41A0.stages_0x3654C[0].stages_3654C_byte0 = 8;
		for (int result = 0; result < 8; result++)
		{
			world_D41A0.playerObjectives_0x3659C[result].substr_3659C.stage_0x3659F[0] = 1;
		}
	}
	else
	{
		for (int result = 0; result < 8; result++)
		{
			if (terrain->stages_0x36442[result].index_0 != -1)
			{
				switch (world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].stages_3654C_byte0)
				{
				case 1:
				case 2:
				case 4:
				case 6:
				case 7:
				case 9:
					if (!terrain->stages_0x36442[result].stage_1)
						terrain->stages_0x36442[result].index_0 = -1;
					break;
				default:
					break;
				}
			}
			if (terrain->stages_0x36442[result].index_0 != -1)
			{
				world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].stages_3654C_byte0 = terrain->stages_0x36442[result].index_0;
				for (int v7 = 0; v7 < 8; v7++)
				{
					world_D41A0.playerObjectives_0x3659C[v7].substr_3659C.stage_0x3659F[world_D41A0.stageIndex_0x36E01] = 1;
				}
				switch (world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].stages_3654C_byte0)
				{
				case 0:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.dword = terrain->stages_0x36442[result].stage_1;//25973
					break;
				case 1:
				case 2:
				case 6:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.ptr16u = &terrain->entity_0x30311[terrain->stages_0x36442[result].stage_1].type_0x30311;
					break;
				case 4:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.ptr16u = &terrain->entity_0x30311[terrain->stages_0x36442[result].stage_1].type_0x30311;
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].position_3654E.x = terrain->stages_0x36442[result]._axis_2d.x << 8;//25975
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].position_3654E.y = terrain->stages_0x36442[result]._axis_2d.y << 8;//25977
					break;
				case 5:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].position_3654E.x = terrain->stages_0x36442[result]._axis_2d.x << 8;
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].position_3654E.y = terrain->stages_0x36442[result]._axis_2d.y << 8;
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.dword = terrain->stages_0x36442[result].stage_1;//25973
					break;
				case 7:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.dword = terrain->entity_0x30311[terrain->stages_0x36442[result].stage_1].subtype_0x30311;
					break;
				case 8:
					break;
				case 9:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.dword = terrain->entity_0x30311[terrain->stages_0x36442[result].stage_1].par1_14;
					break;
				default:
					world_D41A0.stages_0x3654C[world_D41A0.stageIndex_0x36E01].target_36552.dword = terrain->stages_0x36442[result].stage_1 - 1;
					break;
				}
				world_D41A0.stageIndex_0x36E01++;
			}
		}
	}
}

//----- (00058DA0) --------------------------------------------------------
void LinkObjectiveToEntity_58DA0(type_entity_0x30311* a1x, type_entity_0x6E8E* a2x)//239da0
{
	for (int i = 0; i < world_D41A0.stageIndex_0x36E01; i++)
	{
		switch (world_D41A0.stages_0x3654C[i].stages_3654C_byte0)
		{
		case 1:
		case 2:
		case 4:
			if (a1x == world_D41A0.stages_0x3654C[i].target_36552.ptr0x30311)
			{
				world_D41A0.stages_0x3654C[i].target_36552.ptr0x6E8E = a2x;
				world_D41A0.stages_0x3654C[i].flags_3654D |= 1;
			}
			break;
		case 3:
			if (a2x->class_0x3F_63 == 3)
			{
				if (!a2x->model_0x40_64 || a2x->model_0x40_64 == 1)
				{
					if (a2x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.stages_0x3654C[i].target_36552.dword)
					{
						world_D41A0.stages_0x3654C[i].target_36552.ptr0x6E8E = a2x;
						world_D41A0.stages_0x3654C[i].flags_3654D |= 1;
					}
				}
			}
			break;
		case 6:
			if (a1x == world_D41A0.stages_0x3654C[i].target_36552.ptr0x30311)
			{
				world_D41A0.stages_0x3654C[i].target_36552.dword = a2x - world_D41A0.entities_0x6E8E;
				world_D41A0.stages_0x3654C[i].flags_3654D |= 1;
			}
			break;
		default:
			continue;
		}
	}
	return;
}

//----- (00058F00) --------------------------------------------------------
void GameObjectives_58F00()//239f00
{
	int v0x;
	signed __int16 v3; // bx
	bool achievedGoal; // cx
	type_entity_0x6E8E* v8x; // edx
	__int16 j; // ax
	__int16 i; // ax
	__int64 v11; // rax
	__int64 v12; // rax
	__int16 v13; // dx
	signed __int16 v14; // ax
	signed __int16 k; // bx
	type_entity_0x6E8E* v17x; // [esp+4h] [ebp-44h]
	int v20; // [esp+28h] [ebp-20h]
	signed __int16 v22; // [esp+34h] [ebp-14h]
	signed __int16 v23; // [esp+38h] [ebp-10h]
	int v24; // [esp+3Ch] [ebp-Ch]
	char v25; // [esp+40h] [ebp-8h]
	unsigned __int8 v26; // [esp+44h] [ebp-4h]

	if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		v22 = world_D41A0.NumberOfPlayers_0xe;
	else
		v22 = 1;
	v24 = 0;
	v0x = 0;
	if (v22 > 0)
	{
		do
		{
			if (world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveDone_2)//objective pause
			{
				world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveDone_2--;// = v2 - 1;
			}
			else if (!world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.IsLevelEnd_0)
			{
				v3 = 0;
				achievedGoal = false;
				v23 = 0;
				v17x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
				while (v3 < world_D41A0.stageIndex_0x36E01)
				{
					if (world_D41A0.stages_0x3654C[v3].flags_3654D & 2)
					{
						world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
						achievedGoal = true;
						world_D41A0.stages_0x3654C[v3].flags_3654D &= 0xFDu;
					}
					else if (world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] == 1)
					{
						switch (world_D41A0.stages_0x3654C[v3].stages_3654C_byte0)
						{
						case 0://adress 239FE8 // minimal count of mana?
							if (v17x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
							{//adress 239Ffc
								if (gameState_D41A4.manaStats_242ar.totalMana_4 > 0)
								{//23a02d
									if ((100 * (v17x->playerData_0xA4_164x->extraMana_0x13C_316 + Entities_EA3E4[v17x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mana_0x90_144) / gameState_D41A4.manaStats_242ar.totalMana_4)
										>= world_D41A0.stages_0x3654C[v3].target_36552.dword)
									{
										world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
										achievedGoal = true;
										world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 = 0;
									}
								}
							}
							if (!world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 && v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1 && !achievedGoal)//D41A0_BYTESTR_0.struct_0x3659C[v0x].str_3659D_byte1
								world_D41A0.terrain_2FECE.manaGoalPercent_0x2FED5 = world_D41A0.stages_0x3654C[world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1].target_36552.dword;
							break;
						case 1:
							if (world_D41A0.stages_0x3654C[v3].flags_3654D & 1
								&& world_D41A0.stages_0x3654C[v3].target_36552.ptr0x6E8E->life_0x8 <= -1)
							{
								achievedGoal = true;
								world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
							}
							break;
						case 2:
							if (world_D41A0.stages_0x3654C[v3].flags_3654D & 1
								&& world_D41A0.stages_0x3654C[v3].target_36552.ptr0x6E8E->life_0x8 <= -1
								&& !(world_D41A0.stages_0x3654C[v3].target_36552.ptr0x6E8E->fontTypeIndex_0x3D_61))
							{
								achievedGoal = true;
								world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
							}
							break;
						case 3:
							if (!world_D41A0.players_0x2BDE[world_D41A0.stages_0x3654C[v3].target_36552.ptr0x6E8E->playerData_0xA4_164x->playerColorIndex_0x38_56].playerActive_0x006_2BE4_11236)
							{
								achievedGoal = true;
								world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
							}
							break;
						case 4:
							if (world_D41A0.stages_0x3654C[v3].flags_3654D & 1)
							{
								v20 = world_D41A0.stages_0x3654C[v3].target_36552.dword;
								if (v17x == Entities_EA3E4[*(unsigned __int16*)(v20 + 40)])
								{
									v11 = world_D41A0.stages_0x3654C[v3].position_3654E.x - (signed int)*(signed __int16*)(v20 + 76);
									if ((signed int)((HIDWORD(v11) ^ v11) - HIDWORD(v11)) <= 768
										&& abs(world_D41A0.stages_0x3654C[v3].position_3654E.y - *(signed __int16*)(v20 + 78)) <= 768)
									{
										achievedGoal = true;
										world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
									}
								}
							}
							break;
						case 5:
							if (v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1)
							{
								v12 = world_D41A0.stages_0x3654C[v3].position_3654E.x - (int16_t)v17x->position_0x4C_76.x;
								if ((signed int)((HIDWORD(v12) ^ v12) - HIDWORD(v12)) <= 768
									&& abs(world_D41A0.stages_0x3654C[v3].position_3654E.y - (int16_t)v17x->position_0x4C_76.y) <= 768)
								{
									achievedGoal = true;
									world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;//archieve point
								}
							}
							break;
						case 6:
							if (v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1 && world_D41A0.stages_0x3654C[v3].flags_3654D & 1)
							{
								v13 = 0;
								while (world_D41A0.players_0x2BDE[v24].playerData_0x3E6_2BE4_12228.objectiveTargets_0x1AC_428.completedTargets_2BDE_12658[v13] != world_D41A0.stages_0x3654C[v3].target_36552.dword)
								{
									if (++v13 >= 8)
										goto LABEL_72;
								}
								achievedGoal = true;
								world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
							}
							break;
						case 7:
							if (v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1 && !gameState_D41A4.creatureLists_38403x[world_D41A0.stages_0x3654C[v3].target_36552.dword])
							{
								achievedGoal = true;
								world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;//killed creatures
							}
							break;
						case 8:
							if (v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1)
							{
								achievedGoal = true;
								for (i = 0; i < world_D41A0.NumberOfPlayers_0xe; i++)
								{
									if (i != (x_WORD)v24 && world_D41A0.players_0x2BDE[i].playerActive_0x006_2BE4_11236)
									{
										achievedGoal = false;
										break;
									}
								}
								if (achievedGoal)
									world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
							}
							break;
						case 9:
							if (v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1 && !(gameState_D41A4.FrameTimingIndex_26 & 0xF))
							{
								v8x = gameState_D41A4.buildingList_38527;
								v25 = 0;
								while (v8x > Entities_EA3E4[0] && !v25)
								{
									v26 = 0;
									for (j = world_D41A0.stages_0x3654C[v3].target_36552.dword;
										j && !v25 && v26 < 8u;
										j = buildingParams_D93C0[j].nextIndex_3)
									{
										if (j == v8x->levelPar3_0x46_70)
											v25 = 1;
										v26++;
									}
									v8x = v8x->next_0;
								}
								if (!v25)
								{
									achievedGoal = true;
									world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[v3] = 2;
								}
							}
							break;
						default:
							break;
						}
					}
				LABEL_72:
					if (achievedGoal && v3 == world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1)
						v23 = 1;
					v3++;
				}
				if (achievedGoal)
				{
					UpdateHelpItems_88B20();
					v14 = 1;
					for (k = 0; k < world_D41A0.stageIndex_0x36E01; k++)
					{
						if (world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.stage_0x3659F[k] == 1)
						{
							v14 = 0;
							world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.ObjectiveText_1 = k;
							break;
						}
					}
					world_D41A0.playerObjectives_0x3659C[v0x].substr_3659C.IsLevelEnd_0 = v14;
					if (v23 || v14)
					{
						if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE && (x_WORD)v24 != world_D41A0.LevelIndex_0xc)
						{
							world_D41A0.players_0x2BDE[v24].CurrentNotificationDuration_0x04d_2C2B_11307 = 60;
							world_D41A0.players_0x2BDE[v24].endState_0x04f_2C2D_11309 = 4;
							if (v14)
								sprintf(world_D41A0.players_0x2BDE[v24].CurrentNotificationText_0x01c_2BFA_11258, "%s", (char*)langStrings_E9C4C[431]);//Has Completed All Objectives.
							else
								sprintf(world_D41A0.players_0x2BDE[v24].CurrentNotificationText_0x01c_2BFA_11258, "%s", (char*)langStrings_E9C4C[430]);//Has Completed Objective.
						}
						if ((x_WORD)v24 == world_D41A0.LevelIndex_0xc)
							world_D41A0.tempObjective_0x36E02 = 1;
					}
				}
			}
			v0x++;
			v24++;
		} while ((signed __int16)v24 < v22);
	}
}

//----- (00059760) --------------------------------------------------------
void CheckObjectiveKill_59760(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//23a760
{
	signed int v3; // esi
	signed int v4; // ecx
	int ix;
	int j; // edx

	if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		v3 = world_D41A0.NumberOfPlayers_0xe;
	else
		v3 = 1;
	v4 = 0;
	for (ix = 0; v4 < v3; ix++)
	{
		if (!world_D41A0.playerObjectives_0x3659C[ix].substr_3659C.IsLevelEnd_0)
		{
			for (j = 0; ; j++)
			{
				if (j >= world_D41A0.stageIndex_0x36E01)
					break;
				if (world_D41A0.playerObjectives_0x3659C[ix].substr_3659C.stage_0x3659F[j] == 1)
				{
					if (world_D41A0.stages_0x3654C[j].stages_3654C_byte0 == 2 && world_D41A0.stages_0x3654C[j].target_36552.ptr0x6E8E == a1x)
					{
						if (world_D41A0.stages_0x3654C[j].flags_3654D & 1)
							world_D41A0.stages_0x3654C[j].target_36552.ptr0x6E8E = a2x;
					}
				}
			}
		}
		v4++;
	}
}

//----- (00059820) --------------------------------------------------------
void PresentObjective_59820()//23a820
{
	unsigned __int8 v3; // ch
	char v4; // cl
	uint8_t v8; // bx
	unsigned __int8 v9; // al
	char v10; // cl
	int v11; // ebx

	if (world_D41A0.objectiveBoxCounter_0x36E04)
	{
		if (world_D41A0.objectiveBoxCounter_0x36E04-- == 1)
			UpdateObjectiveHelp_88BA0();
	}
	if (gameState_D41A4.setting_38545 & 0x40 && GetCdTrackStatus_86180(cdDrive_1803EC) == 256)
		StopCdPlayBackAndFadeUp_59AF0();
	if (gameState_D41A4.paletteMod_51 >= 3u)
	{
		v3 = world_D41A0.tempObjective_0x36E02;
		if (v3)
		{
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			{
			LABEL_38:
				world_D41A0.tempObjective_0x36E02 = 0;
				return;
			}
			if (gameState_D41A4.OptionsSettingFlag_24 & SPEECH_ENABLED)
			{
				if (v3 < 7u)
				{
					if (v3 == 1)
					{
						world_D41A0.tempObjective_0x36E02++;
						return;
					}
					goto LABEL_32;
				}
				if (v3 > 7u)
				{
					if (v3 > 8u)
					{
						if (v3 != -56)
						{
						LABEL_32:
							world_D41A0.tempObjective_0x36E02++;
							return;
						}
						goto LABEL_38;
					}
					v4 = world_D41A0.gameFlags_0x36E0B;
					world_D41A0.tempObjective_0x36E02 = v3 + 1;
					if (v4 & 1)
					{
						gameState_D41A4.paletteSubMod_180 = 8;
						PlayCDTrackSegmentForSecretLevel_86F20(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].secretCounter_0x3E4_2BE4_12226);
						FadeDownSoundVolume_59A50();
						if (autoShowObjectivesForForeignLanguages && gameState_D41A4.SelectedLangIndex != 2)
							world_D41A0.objectiveBoxCounter_0x36E04 = 200;
						return;
					}
					if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1)
						PrepareEventSound_6E450(world_D41A0.LevelIndex_0xc, -1, 61);
					if (gameState_D41A4.levelnumber_43w >= 30 && gameState_D41A4.levelnumber_43w <= 34)
					{
						if (!world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
						{
							v9 = 4;
							v8 = 0;
							goto LABEL_30;
						}
						v8 = 10;
					}
					else
					{
						v8 = gameState_D41A4.levelnumber_43w;
						if (!world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.IsLevelEnd_0)
						{
							v9 = world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1 + 1;
						LABEL_30:
							gameState_D41A4.paletteSubMod_180 = 8;
							PlayCDTrackSegmentNumber_86EB0(v8, v9, 1);
							FadeDownSoundVolume_59A50();
							if (autoShowObjectivesForForeignLanguages && gameState_D41A4.SelectedLangIndex != 2)
								world_D41A0.objectiveBoxCounter_0x36E04 = 200;
							return;
						}
					}
					v9 = 9;
					goto LABEL_30;
				}
				world_D41A0.tempObjective_0x36E02 = 8;
				gameState_D41A4.paletteSubMod_180 = 8;
				goto LABEL_36;
			}
			world_D41A0.tempObjective_0x36E02 = 0;
			v10 = world_D41A0.gameFlags_0x36E0B;
			world_D41A0.objectiveBoxCounter_0x36E04 = 200; // set counter for displaying objective message box to 200
			if (v10 & 1)
			{
			LABEL_36:
				PrepareEventSound_6E450(world_D41A0.LevelIndex_0xc, -1, 41);
				return;
			}
			v11 = world_D41A0.LevelIndex_0xc;
			if (world_D41A0.playerObjectives_0x3659C[world_D41A0.LevelIndex_0xc].substr_3659C.ObjectiveText_1)
				PrepareEventSound_6E450(v11, -1, 61);
		}
	}
}

//----- (0005C530) --------------------------------------------------------
void CollectLevelStats_5C530()//23d530
{
	int v0x; // edx
	int v1; // ecx
	type_entity_0x6E8E* v2x; // ebx
	signed int i; // eax
	type_str_164* v4x; // esi
	signed int j; // eax
	int v6; // eax
	//int v7; // ecx
	int v8; // edx
	int v9; // edi
	int v10; // eax
	//int v11; // ecx
	int v12; // edx
	int v13; // edi
	int v14; // eax
	//int v15; // ecx
	type_entity_0x6E8E* v16x; // eax
	//int v16y; // eax
	int v17; // esi
	int v18; // edx
	/*int v19; // esi
	int v20; // eax
	int v21; // eax
	int v22; // eax
	int v23; // eax
	int v24; // eax
	int v25; // eax
	int v26; // eax
	int v27; // eax*/

	v0x = 0;
	v1 = 0;
	v2x = Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240];
	for (i = 0; i < 26; i++)
	{
		v4x = v2x->playerData_0xA4_164x;
		if (v4x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] || v4x->str_611.pickedUpSpells_0x403_1027x.SpellIndex[i])
			v4x->str_611.availableSpells_0x3E9_1001x.SpellIndex[i] = 1;
	}
	if (world_D41A0.gameFlags_0x36E0B & 1)
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].secretCounter_0x3E4_2BE4_12226++;
	for (j = 0; j < 26; j++)
	{
		if (world_D41A0.creatureStats_0x364D2.spawnCounts_0x364D6[j])
		{
			v1++;
			if (v2x->playerData_0xA4_164x->str_611.pickedUpSpells_0x403_1027x.SpellIndex[j])
				v0x++;
		}
	}
	if (v1)
	{
		v6 = 100 * v0x / v1;
		//v0 = v2x->dword_0xA4_164;
		v2x->playerData_0xA4_164x->spellsCollectedPercent_377 = v6;
	}
	else
	{
		v2x->playerData_0xA4_164x->spellsCollectedPercent_377 = 100;
	}
	if (world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2)
	{
		//v7 = v2x->dword_0xA4_164;
		v8 = 100 * v2x->playerData_0xA4_164x->creaturesKilledPercent_373;
		v9 = world_D41A0.creatureStats_0x364D2.livingCreatures_0x364D2;
		v10 = v8 / v9;
		//v0 = v8 % v9;
		v2x->playerData_0xA4_164x->creaturesKilledPercent_373 = v10;
	}
	else
	{
		v2x->playerData_0xA4_164x->creaturesKilledPercent_373 = 100;
	}
	//v11 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->shotsFired_0x165_357)
	{
		v12 = 100 * v2x->playerData_0xA4_164x->shotsHit_0x169_361;
		v13 = v2x->playerData_0xA4_164x->shotsFired_0x165_357;
		v14 = v12 / v13;
		//v0 = v12 % v13;
		v2x->playerData_0xA4_164x->hitAccuracyPercent_381 = v14;
	}
	else
	{
		v2x->playerData_0xA4_164x->hitAccuracyPercent_381 = 100;
	}
	//v15 = v2x->dword_0xA4_164;
	v16x = Entities_EA3E4[v2x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58];
	if (v16x)
	{
		v17 = gameState_D41A4.manaStats_242ar.totalMana_4 - 1;
		if (v17 <= 0)
		{
			v2x->playerData_0xA4_164x->manaCollectedPercent_385 = 100;
			goto LABEL_29;
		}
		v18 = 100 * (v2x->playerData_0xA4_164x->extraMana_0x13C_316 + v16x->mana_0x90_144);
		v2x->playerData_0xA4_164x->manaCollectedPercent_385 = v18 / v17;
		//v0 = v18 % v17;
	}
	else
		v2x->playerData_0xA4_164x->manaCollectedPercent_385 = 0;
	//v2x->playerData_0xA4_164x->manaCollectedPercent_385 = v16y;//? or (uint8_t*)(v18 / v17)
LABEL_29:
	//Get Level Time
	v2x->playerData_0xA4_164x->time_393 = (j___clock() - v2x->playerData_0xA4_164x->time_393) / 0x64u;
	//v20 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->spellsCollectedPercent_377 < 0)
		v2x->playerData_0xA4_164x->spellsCollectedPercent_377 = 0;
	//v21 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->spellsCollectedPercent_377 > 100)
		v2x->playerData_0xA4_164x->spellsCollectedPercent_377 = 100;
	//v22 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->creaturesKilledPercent_373 < 0)
		v2x->playerData_0xA4_164x->creaturesKilledPercent_373 = 0;
	//v23 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->creaturesKilledPercent_373 > 100)
		v2x->playerData_0xA4_164x->creaturesKilledPercent_373 = 100;
	//v24 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->hitAccuracyPercent_381 < 0)
		v2x->playerData_0xA4_164x->hitAccuracyPercent_381 = 0;
	//v25 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->hitAccuracyPercent_381 > 100)
		v2x->playerData_0xA4_164x->hitAccuracyPercent_381 = 100;
	//v26 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->manaCollectedPercent_385 < 0)
		v2x->playerData_0xA4_164x->manaCollectedPercent_385 = 0;
	//v27 = v2x->dword_0xA4_164;
	if (v2x->playerData_0xA4_164x->manaCollectedPercent_385 > 100)
		v2x->playerData_0xA4_164x->manaCollectedPercent_385 = 100;
	ActivateSecretPortal_82AB0(gameState_D41A4.levelnumber_43w);
}
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (00069FF0) --------------------------------------------------------
signed int AllCreaturesKilled_69FF0(type_entity_0x6E8E* a1)//24aff0
{
	return UpdateSpellPickupFloating_692A0(a1);
}

//----- (0006F030) --------------------------------------------------------
type_entity_0x6E8E* SwitchResetLevel_6F030(type_entity_0x6E8E* event)//250030
{
	type_entity_0x6E8E* event2 = InitSwitchChainZaxisAndSound_6F850(event, 1);
	if (event2)
	{
		ResetLevelState_4A1E0(event->id_0x1A_26, 1);
		DisableEntityDrawing04_57F10(event);
	}
	return event2;
}

//----- (0006F070) --------------------------------------------------------
type_entity_0x6E8E* CheckpointArrived_6F070(type_entity_0x6E8E* event)//250070
{
	type_entity_0x6E8E* event2 = InitSwitchChainZaxisAndSound_6F850(event, 0);
	if (event2)
	{
		ResetLevelState_4A1E0(event->id_0x1A_26, 1);
		DisableEntityDrawing04_57F10(event);
	}
	return event2;
}

//----- (0006F0B0) --------------------------------------------------------
void SwitchCheckPlayersNear2_6F0B0(type_entity_0x6E8E* a1x)//2500b0
{
	//type_str_0x6E8E* resultx; // eax

	if (a1x->param_0x10_16)
	{
		//resultx = FindPlayerTouchingSwitch_6F8E0(a1x, 1);
		if (!FindPlayerTouchingSwitch_6F8E0(a1x, 1))
			a1x->param_0x10_16--;
	}
	else
	{
		//resultx = sub_6F850(a1x, 1);
		if (InitSwitchChainZaxisAndSound_6F850(a1x, 1))
		{
			ResetLevelState_4A1E0(a1x->id_0x1A_26, 0);
			a1x->param_0x10_16 = 10;
		}
	}
	//return resultx;
}

type_entity_0x6E8E* (*spellPickupCreators_2A881E[])(axis_3d*) = { AddFireballSpell_511A0, AddPossessSpell_511C0, AddCastleSpell_511E0,AddSpeedUpSpell_51200,AddMorphSpell_51220,
AddHealSpell_51240 ,AddShieldSpell_51260,AddLightningSpell_51280,AddReboundSpell_512A0,AddMeteorSpell_512C0,AddTeleportSpell_512E0,AddInvisibleSpell_51300,AddBeyondSightSpell_51320,AddStealManaSpell_51340,
AddDuelSpell_51360,AddTremorSpell_51380,AddCraterSpell_513A0,AddEarthquakeSpell_513C0,AddVolcanoSpell_513E0,AddSummonArmySpell_51400,AddGravityWellSpell_51420,AddWhirlwindSpell_51440,
AddFoolsManaSpell_51460,AddMagicMineSpell_51480,AddAllianceSpell_514A0,AddCaveInSpell_514C0 };

//----- (0006F100) --------------------------------------------------------
type_entity_0x6E8E* SwitchCheckPlayersNear_6F100(type_entity_0x6E8E* a1x)//250100
{
	type_entity_0x6E8E* resultx; // eax

	if (a1x->param_0x10_16)
	{
		resultx = FindPlayerTouchingSwitch_6F8E0(a1x, 0);
		if (!resultx)
			a1x->param_0x10_16--;
	}
	else
	{
		resultx = InitSwitchChainZaxisAndSound_6F850(a1x, 0);
		if (resultx)
		{
			/*result = */ResetLevelState_4A1E0(a1x->id_0x1A_26, 0);
			a1x->param_0x10_16 = 10;
		}
	}
	return resultx;
}

//----- (0006F150) --------------------------------------------------------
type_entity_0x6E8E* SwitchCheckLevelEnd_6F150(type_entity_0x6E8E* a1x)//250150
{
	type_entity_0x6E8E* resultx; // eax

	resultx = gameState_D41A4.class3List_38519;
	if (resultx > Entities_EA3E4[0])
	{
		while (resultx->model_0x40_64
			|| !world_D41A0.playerObjectives_0x3659C[resultx->playerData_0xA4_164x->playerColorIndex_0x38_56].substr_3659C.IsLevelEnd_0)
		{
			resultx = resultx->next_0;
			if (resultx <= Entities_EA3E4[0])
				return resultx;
		}
		ResetLevelState_4A1E0(a1x->id_0x1A_26, 1);
		DisableEntityDrawing04_57F10(a1x);
	}
	return resultx;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006F1C0) --------------------------------------------------------
void SwitchCheckStage_6F1C0(type_entity_0x6E8E* a1x)//2501c0
{
	int resultx;
	signed __int16 v2; // bx
	signed __int16 v3; // dx

	if (a1x->levelPar3_0x46_70 < world_D41A0.stageIndex_0x36E01)
	{
		if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			v2 = world_D41A0.NumberOfPlayers_0xe;
		else
			v2 = 1;
		v3 = 0;
		resultx = 0;
		while (v3 < v2)
		{
			if (world_D41A0.playerObjectives_0x3659C[resultx].substr_3659C.stage_0x3659F[a1x->levelPar3_0x46_70] == 2)
			{
				world_D41A0.playerObjectives_0x3659C[resultx].substr_3659C.ObjectiveDone_2 = 1;
				ResetLevelState_4A1E0(a1x->id_0x1A_26, 1);
				DisableEntityDrawing04_57F10(a1x);
				return;// 1;
			}
			v3++;
			resultx++;
		}
	}
}

//----- (0006F2B0) --------------------------------------------------------
void ActivateSwitchChain_6F2B0(type_entity_0x6E8E* a1x)//2502b0
{
	type_entity_0x6E8E* result; // eax
	//unsigned __int16 v2; // dx

	result = InitSwitchChainZaxisAndSound_6F850(a1x, 1);
	if (result)
	{
		result->actionIndex_0x45_69 = 12;
		result->levelPar3_0x46_70 = 0;
		DisableEntityDrawing04_57F10(a1x);
		//result = (x_BYTE *)x_D41A0_BYTEARRAY_0;
		//v2 = D41A0_BYTESTR_0.word_0x36DFE;
		if (world_D41A0.switchChainEntity_0x36DFE)
		{
			//result = Entities_EA3E4[v2];
			//result[12] &= 0xFEu;
			Entities_EA3E4[world_D41A0.switchChainEntity_0x36DFE]->flags_0xc_12_15.byte[0] &= 0xFEu;
		}
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0006F300) --------------------------------------------------------
unsigned int CountCreaturesOfModel_6F300(type_entity_0x6E8E* a1x, signed int a2)//250300
{
	unsigned int result; // eax
	int v3; // esi

	result = a2;
	if (a2 == -1)
	{
		for (result = 0; (signed int)result <= 16; ++result)
		{
			if ((result <= 0xB || result >= 0x10 && result <= 0x1C) && gameState_D41A4.creatureLists_38403x[result])
				return result;
		}
		v3 = a1x->param_0x10_16;
		if (!v3)
		{
			a1x->param_0x10_16 = 16;
			return result;
		}
		if (v3 == 1)
		{
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 41);
			ResetLevelState_4A1E0(a1x->id_0x1A_26, 1);
			DisableEntityDrawing04_57F10(a1x);
			return 1;
		}
	}
	else
	{
		if (gameState_D41A4.creatureLists_38403x[a2])
			return result;
		v3 = a1x->param_0x10_16;
		if (!v3)
		{
			a1x->param_0x10_16 = 16;
			return result;
		}
		if (v3 == 1)
		{
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 41);
			ResetLevelState_4A1E0(a1x->id_0x1A_26, 1);
			DisableEntityDrawing04_57F10(a1x);
			return 1;
		}
	}
	result = v3 - 1;
	a1x->param_0x10_16 = v3 - 1;
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (0006F420) --------------------------------------------------------
unsigned int CountCreatures00_6F420(type_entity_0x6E8E* a1x)//250420
{
	return CountCreaturesOfModel_6F300(a1x, 0);
}

//----- (0006F440) --------------------------------------------------------
unsigned int CountCreatures01_6F440(type_entity_0x6E8E* a1)//250440
{
	return CountCreaturesOfModel_6F300(a1, 1u);
}

//----- (0006F460) --------------------------------------------------------
unsigned int CountCreatures02_6F460(type_entity_0x6E8E* a1)//250460
{
	return CountCreaturesOfModel_6F300(a1, 2u);
}

//----- (0006F480) --------------------------------------------------------
unsigned int CountCreatures03_6F480(type_entity_0x6E8E* a1)//250480
{
	return CountCreaturesOfModel_6F300(a1, 3u);
}

//----- (0006F4A0) --------------------------------------------------------
unsigned int CountCreatures04_6F4A0(type_entity_0x6E8E* a1)//2504a0
{
	return CountCreaturesOfModel_6F300(a1, 4u);
}

//----- (0006F4C0) --------------------------------------------------------
unsigned int CountCreatures05_6F4C0(type_entity_0x6E8E* a1)//2504c0
{
	return CountCreaturesOfModel_6F300(a1, 5u);
}

//----- (0006F4E0) --------------------------------------------------------
unsigned int CountCreatures06_6F4E0(type_entity_0x6E8E* a1)//2504e0
{
	return CountCreaturesOfModel_6F300(a1, 6u);
}

//----- (0006F500) --------------------------------------------------------
unsigned int CountCreatures07_6F500(type_entity_0x6E8E* a1)//250500
{
	return CountCreaturesOfModel_6F300(a1, 7u);
}

//----- (0006F520) --------------------------------------------------------
unsigned int CountCreatures08_6F520(type_entity_0x6E8E* a1)//250520
{
	return CountCreaturesOfModel_6F300(a1, 8u);
}

//----- (0006F540) --------------------------------------------------------
unsigned int CountCreatures09_6F540(type_entity_0x6E8E* a1)//250540
{
	return CountCreaturesOfModel_6F300(a1, 9u);
}

//----- (0006F560) --------------------------------------------------------
unsigned int CountCreatures0A_6F560(type_entity_0x6E8E* a1)//250560
{
	return CountCreaturesOfModel_6F300(a1, 0xAu);
}

//----- (0006F580) --------------------------------------------------------
unsigned int CountCreatures0B_6F580(type_entity_0x6E8E* a1)//250580
{
	return CountCreaturesOfModel_6F300(a1, 0xBu);
}

//----- (0006F5A0) --------------------------------------------------------
unsigned int CountCreatures0C_6F5A0(type_entity_0x6E8E* a1)//2505a0
{
	return CountCreaturesOfModel_6F300(a1, 0xCu);
}

//----- (0006F5C0) --------------------------------------------------------
unsigned int CountCreatures0D_6F5C0(type_entity_0x6E8E* a1)//2505c0
{
	return CountCreaturesOfModel_6F300(a1, 0xDu);
}

//----- (0006F5E0) --------------------------------------------------------
unsigned int CountCreatures0E_6F5E0(type_entity_0x6E8E* a1)//2505e0
{
	return CountCreaturesOfModel_6F300(a1, 0xEu);
}

//----- (0006F600) --------------------------------------------------------
unsigned int CountCreatures0F_6F600(type_entity_0x6E8E* a1)//250600
{
	return CountCreaturesOfModel_6F300(a1, 0xFu);
}

//----- (0006F620) --------------------------------------------------------
unsigned int CountCreatures10_6F620(type_entity_0x6E8E* a1)//250620
{
	return CountCreaturesOfModel_6F300(a1, 0x10u);
}

//----- (0006F640) --------------------------------------------------------
unsigned int CountCreatures11_6F640(type_entity_0x6E8E* a1)//250640
{
	return CountCreaturesOfModel_6F300(a1, 0x11u);
}

//----- (0006F660) --------------------------------------------------------
unsigned int CountCreatures12_6F660(type_entity_0x6E8E* a1)//250660
{
	return CountCreaturesOfModel_6F300(a1, 0x12u);
}

//----- (0006F680) --------------------------------------------------------
unsigned int CountCreatures13_6F680(type_entity_0x6E8E* a1)//250680
{
	return CountCreaturesOfModel_6F300(a1, 0x13u);
}

//----- (0006F6A0) --------------------------------------------------------
unsigned int CountCreatures14_6F6A0(type_entity_0x6E8E* a1)//2506a0
{
	return CountCreaturesOfModel_6F300(a1, 0x14u);
}

//----- (0006F6C0) --------------------------------------------------------
unsigned int CountCreatures15_6F6C0(type_entity_0x6E8E* a1)//2506c0
{
	return CountCreaturesOfModel_6F300(a1, 0x15u);
}

//----- (0006F6E0) --------------------------------------------------------
unsigned int CountCreatures16_6F6E0(type_entity_0x6E8E* a1)//2506e0
{
	return CountCreaturesOfModel_6F300(a1, 0x16u);
}

//----- (0006F700) --------------------------------------------------------
unsigned int CountCreatures17_6F700(type_entity_0x6E8E* a1)//250700
{
	return CountCreaturesOfModel_6F300(a1, 0x17u);
}

//----- (0006F720) --------------------------------------------------------
unsigned int CountCreatures18_6F720(type_entity_0x6E8E* a1)//250720
{
	return CountCreaturesOfModel_6F300(a1, 0x18u);
}

//----- (0006F740) --------------------------------------------------------
unsigned int CountCreatures19_6F740(type_entity_0x6E8E* a1)//250740
{
	return CountCreaturesOfModel_6F300(a1, 0x19u);
}

//----- (0006F760) --------------------------------------------------------
unsigned int CountCreatures1A_6F760(type_entity_0x6E8E* a1)//250760
{
	return CountCreaturesOfModel_6F300(a1, 0x1Au);
}

//----- (0006F780) --------------------------------------------------------
unsigned int CountCreatures1B_6F780(type_entity_0x6E8E* a1)//250780
{
	return CountCreaturesOfModel_6F300(a1, 0x1Bu);
}

//----- (0006F7A0) --------------------------------------------------------
unsigned int CountCreatures1C_6F7A0(type_entity_0x6E8E* a1)//2507a0
{
	return CountCreaturesOfModel_6F300(a1, 0x1Cu);
}

//----- (0006F7C0) --------------------------------------------------------
unsigned int CountAllCreatures_6F7C0(type_entity_0x6E8E* a1)//2507c0
{
	return CountCreaturesOfModel_6F300(a1, 0xFFFFFFFF);
}

//----- (0006F7E0) --------------------------------------------------------
void ActivateSwitchChainOnce_6F7E0(type_entity_0x6E8E* entity) //2507e0
{
	if (gameState_D41A4.setting_38545 & 8)
		DisableEntityDrawing04_57F10(entity);
	type_entity_0x6E8E* entity2 = InitSwitchChainZaxisAndSound_6F850(entity, 1);
	if (entity2)
	{
		entity2->actionIndex_0x45_69 = 11;
		entity2->levelPar3_0x46_70 = 0;
		DisableEntityDrawing04_57F10(entity);
		if (world_D41A0.switchOnceEntity_0x36DFC)
		{
			Entities_EA3E4[world_D41A0.switchOnceEntity_0x36DFC]->flags_0xc_12_15.byte[0] &= 0xFEu;
		}
	}
}

//----- (0006F850) --------------------------------------------------------
type_entity_0x6E8E* InitSwitchChainZaxisAndSound_6F850(type_entity_0x6E8E* event, int a2)//250850
{
	type_entity_0x6E8E* ix; // ebx
	if (event->instanceIndex_0x3E_62 & 7)
		return 0;
	for (ix = gameState_D41A4.class3List_38519; ; ix = ix->next_0)
	{
		if (ix <= Entities_EA3E4[0])
		{
			event->position_0x4C_76.z = GetTerrainAlt_10C40(&event->position_0x4C_76);
			return 0;
		}
		if (!ix->model_0x40_64 && CompareAxisWithShift_10750(event, ix) == a2)
			break;
	}
	if (event->model_0x40_64 > 3u)
		PrepareEventSound_6E450(ix - world_D41A0.entities_0x6E8E, -1, 41);
	return ix;
}

//----- (0006F8E0) --------------------------------------------------------
type_entity_0x6E8E* FindPlayerTouchingSwitch_6F8E0(type_entity_0x6E8E* a1x, int a2)////2508e0
{
	unsigned __int16 v2; // bx
	//int v3; // esi
	int v3x;
	type_entity_0x6E8E* v4x; // edi

	v2 = 0;
	//v3 = (int)x_D41A0_BYTEARRAY_0 + 11230;
	v3x = 0;
	while (v2 < world_D41A0.NumberOfPlayers_0xe)
	{
		v4x = Entities_EA3E4[world_D41A0.players_0x2BDE[v3x].playerIndex_0x00a_2BE4_11240];
		if (CompareAxisWithShift_10750(a1x, v4x) == a2)
			return v4x;
		//v3 += 2124;
		v3x++;
		v2++;
	}
	return 0;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
