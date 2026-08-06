#include "Level.h"

bool IsAfterLoad = false;

void sub_71890();
void sub_718F0();


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


//----- (00055250) --------------------------------------------------------
bool SaveLevelSLEV_55250(uint8_t savefileindex, char* savefileindex2)//236250 //in game save
{
	bool success;
	int temptime; // edi
	long acttime; // eax

	//fix for saving
	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
		//if (memory_readable(Entities_EA3E4[indexx]->dword_0xA4_164x,4))
	{
		if (Entities_EA3E4[indexx]->dword_0xA4_164x == unk_F42B0x)//0x014F82E8//0x2c75e28-(uint32_t)&D41A0_BYTESTR_0
			Entities_EA3E4[indexx]->dword_0xA4_164x = (type_str_164*)0x2c75e28;
	}
	//fix for saving

	success = false;
	sprintf(printbuffer, "%s/%s/%s%d%s.DAT", gameDataPath.c_str(), "SAVE", "SLEV", savefileindex + 1, savefileindex2);
	D41A0_0.dword_0x36DF6 = &str_D7BD6[59]; //(x_DWORD)&unk_D7BD6[0x7d6];
	temptime = D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.time_393;
	acttime = j___clock();
	D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.time_393 = acttime - D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.time_393;

	//x64 fix
	type_shadow_D41A0_BYTESTR_0 shadow_type_D41A0_BYTESTR_0;
	Convert_to_shadow_D41A0_BYTESTR_0(&D41A0_0, &shadow_type_D41A0_BYTESTR_0);
	//x64 fix

	int size = sizeof(shadow_type_D41A0_BYTESTR_0);
	if (DataFileIO::sub_98C48_open_nwrite_close(printbuffer, (uint8_t*)&shadow_type_D41A0_BYTESTR_0, size) == size) success = true;
	D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.time_393 = temptime;

	//fix for saving
	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
	{
		type_str_164* Zerotype_str_164 = 0;
		if (Entities_EA3E4[indexx]->dword_0xA4_164x == (type_str_164*)&((int8_t*)Zerotype_str_164)[0x2c75e28])//0x014F82E8//0x2c75e28-(uint32_t)&D41A0_BYTESTR_0
			Entities_EA3E4[indexx]->dword_0xA4_164x = unk_F42B0x;
	}
	//fix for saving

	return success;
}

//----- (00055320) --------------------------------------------------------
bool SaveLevelSMAP_55320(uint8_t savefileindex, char* savefileindex2)//236320 //file index added for develop
{
	FILE* savesmapfile; // eax
	size_t writedsize; // [esp+40h] [ebp-8h]
	Logger->debug("InGameSave-begin");
	sprintf(printbuffer, "%s/%s/%s%d%s.DAT", gameDataPath.c_str(), "SAVE", "SMAP", savefileindex + 1, savefileindex2);
	savesmapfile = DataFileIO::CreateOrOpenFile(printbuffer, 546);
	if (savesmapfile)
	{
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapTerrainType_10B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapHeightmap_11B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapShading_12B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)mapAngle_13B4E0, 0x10000);
		DataFileIO::WriteFile_98CAA(savesmapfile, (uint8_t*)x_BYTE_14B4E0_second_heightmap, 0x10000);
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
	sprintf(printbuffer, "%s/%s/%s%d%s.DAT", gameDataPath.c_str(), "SAVE", "SVER", savefileindex + 1, savefileindex2);
	if (DataFileIO::sub_98C48_open_nwrite_close(printbuffer, (uint8_t*)data, 2*sizeof(int32_t)) == 8)
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
	if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		tempGr = D41A0_0.m_GameSettings.m_Graphics;
		tempDisp = D41A0_0.m_GameSettings.m_Display;
		temp0x2192 = D41A0_0.m_GameSettings.str_0x2192;
		temp0x2196 = D41A0_0.m_GameSettings.str_0x2196;
		temp0x219A = D41A0_0.dword_0x219A;
		temp0x219E = D41A0_0.dword_0x219E;
		temp0x21A2 = D41A0_0.dword_0x21A2;
		temp0x21A6 = D41A0_0.dword_0x21A6;
		temp0x21AA = D41A0_0.str_0x21AA;
		temp0x21AE = D41A0_0.str_0x21AE;
		temp0x21B2 = D41A0_0.str_0x21B2;
		temp0x21B6 = D41A0_0.str_0x21B6;
		readSuccess = DataFileIO::sub_55750_TestExistingSaveFile(fileindex, levelindex, loadRegressionTest);
		//adress  23662a
		if (readSuccess)
		{
			readSuccess = LoadLevelSMAP_558E0(fileindex, loadRegressionTest);
			if (readSuccess)
			{
				qmemcpy(
					&x_D41A0_BYTEARRAY_4_struct.byteindex_256ar,
					&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc],
					0x84Cu);
				readSuccess = LoadLevelSLEV_55A10(fileindex, loadRegressionTest);
				if (readSuccess)
				{
					sub_55100(2);
					sub_57680_FixPointersAfterLoad();
					sub_549A0(&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.str_611, &x_D41A0_BYTEARRAY_4_struct.byteindex_256ar.dword_0x3E6_2BE4_12228.str_611);
					sub_49F90();
					D41A0_0.dword_0x11e6 = -1;
					sub_55AB0(&D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc]);
					sub_71990();
					FlvInitSet_473B0();
				}
			}
		}
		D41A0_0.m_GameSettings.m_Graphics = tempGr;
		D41A0_0.m_GameSettings.m_Display = tempDisp;
		D41A0_0.m_GameSettings.str_0x2192 = temp0x2192;
		D41A0_0.m_GameSettings.str_0x2196 = temp0x2196;
		D41A0_0.dword_0x219A = temp0x219A;
		D41A0_0.dword_0x219E = temp0x219E;
		D41A0_0.dword_0x21A2 = temp0x21A2;
		D41A0_0.dword_0x21A6 = temp0x21A6;
		D41A0_0.str_0x21AA = temp0x21AA;
		D41A0_0.str_0x21AE = temp0x21AE;
		D41A0_0.str_0x21B2 = temp0x21B2;
		D41A0_0.str_0x21B6 = temp0x21B6;
	}
	return readSuccess;
}

//----- (000558E0) --------------------------------------------------------
bool LoadLevelSMAP_558E0(uint8_t savefileindex, bool loadRegressionTest)//2368e0
{
	Logger->debug("InGameLoad-begin\n");

	//fix
	x_D41A0_BYTEARRAY_4_struct.dword_38519 = Entities_EA3E4[1];
	//fix

	// FIXME: cannot set this here.
	//        was before: debugafterload = 1;
	//        cannot be: CommandLineParams.DoDebugafterload() = 1;
	x_D41A0_BYTEARRAY_4_struct.setting_30 = 0x3d;//fix same run after load
	rand2_17B4E0 = 0x21ed;//fix random variable for debugging

	char path[512];
	sprintf(path, "%s/%s", gameDataPath.c_str(), "SAVE");
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
		DataFileIO::Read(loadfile, (uint8_t*)x_BYTE_14B4E0_second_heightmap, 0x10000);
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
	sprintf(path, "%s/%s", gameDataPath.c_str(), "SAVE");
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
		D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.time_393 = j___clock() - D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x3E6_2BE4_12228.time_393;
		success = true;
	}
	type_str_164* tempEntities_EA3E4_0 = Entities_EA3E4[0]->dword_0xA4_164x;//fix for x64
	Convert_from_shadow_D41A0_BYTESTR_0(&shadow_D41A0_BYTESTR_0, &D41A0_0);
	Entities_EA3E4[0]->dword_0xA4_164x =tempEntities_EA3E4_0;//fix for x64
	return success;
}

//----- (00055080) --------------------------------------------------------
bool SaveLevel_55080(uint8_t savefileindex, int32_t LevelNumber, char* savefileindex2)//236080 //filenameindex added for debugging
{
	bool success = false;
	if (!(x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		sub_71930();
		sub_55100(1);
		success = SaveLevelSLEV_55250(savefileindex, savefileindex2);
		if (success)
		{
			success = SaveLevelSMAP_55320(savefileindex, savefileindex2);
			if (success)
				success = SaveLevelSVER_55450(savefileindex, LevelNumber, savefileindex2);
		}
		sub_55100(2);
		sub_49F90();
		D41A0_0.dword_0x11e6 = -1;
	}
	return success;
}

//----- (00049270) --------------------------------------------------------
void sub_49270_generate_level_features(Type_Level_2FECE* terrain)//22a270
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
		fontStruct = xy_DWORD_17DEC0_spritestr;
		fontDatBase = x_DWORD_17DE38str.x_DWORD_17DE54;
		datTabOffset0 = x_DWORD_17DE38str.x_DWORD_17DEC0;
		datTabOffset1 = x_DWORD_17DE38str.x_DWORD_17DEC4;
		patchedBufferPtr = &g_patchedFontDataBuffer;
		break;
	case 1:
		fontStruct = xy_DWORD_17DEC8_spritestr;
		fontDatBase = x_DWORD_17DE38str.x_DWORD_17DE58;
		datTabOffset0 = x_DWORD_17DE38str.x_DWORD_17DEC8;
		datTabOffset1 = x_DWORD_17DE38str.x_DWORD_17DECC;
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
		sub_7AA70_load_and_decompres_dat_file(dataPath, *xadatapald0dat2.colorPalette_var28, 0x17C118, 0x300);
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
		if (x_WORD_180660_VGA_type_resolution & 1)
			sub_98709_create_index_dattab_power(datTabOffset0, datTabOffset1, fontDatBase, fontStruct);
		else
			sub_9874D_create_index_dattab(datTabOffset0, datTabOffset1, fontDatBase, fontStruct);
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
		if (x_WORD_180660_VGA_type_resolution & 1)
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
	switch (D41A0_0.terrain_2FECE.MapType)
	{
		case MapType_t::Day:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/PALD-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, xadatapald0dat2.colorPalette_var28);
		}
		break;
		case MapType_t::Night:
		{
			if (D41A0_0.terrain_2FECE.byte_0x2FED2 & 2)
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

	MapType_t   mt = D41A0_0.terrain_2FECE.MapType;
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
	sub_9874D_create_index_dattab(
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
	if (D41A0_0.terrain_2FECE.MapType == MapType_t::Day)
	{
		sprintf(xadatamsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRD0-0.DAT");
		sprintf(xadatamsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRD0-0.TAB");
		sprintf(xadatahsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRD0-0.DAT");
		sprintf(xadatahsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRD0-0.TAB");
	}
	else if (D41A0_0.terrain_2FECE.MapType == MapType_t::Night)
	{
		sprintf(xadatamsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRN0-0.DAT");
		sprintf(xadatamsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/MSPRN0-0.TAB");
		sprintf(xadatahsprd00dat.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRN0-0.DAT");
		sprintf(xadatahsprd00tab.path, "%s/%s", cdDataPath.c_str(), "DATA/HSPRN0-0.TAB");
	}
	else if (D41A0_0.terrain_2FECE.MapType == MapType_t::Cave)
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

	if (x_WORD_180660_VGA_type_resolution == 1)
	{
		DataFileIO::LoadFileArray_84250(psxadatamsprd00dat);
		filearray_2aa18c[filearrayindex_MSPRD00DATTAB] = { &MSPRD00TAB_BEGIN_BUFFER,&MSPRD00TAB_END_BUFFER,&MSPRD00DAT_BEGIN_BUFFER,&posistruct5 };
		if (pre_x_DWORD_E9C3C)
			FreeMem_83E80(pre_x_DWORD_E9C3C);
		pre_x_DWORD_E9C3C = (uint8_t*)Malloc_83CD0(3000000);
		x_DWORD_E9C3C = &pre_x_DWORD_E9C3C[200000];
		help_VGA_type_resolution = 1;
	}
	else
	{
		DataFileIO::LoadFileArray_84250(psxadatahsprd00dat);//here is loading

		if(fixedMenuGraphics && !(x_WORD_180660_VGA_type_resolution & 1))
			LoadFixedMenuGraphics();

		filearray_2aa18c[filearrayindex_MSPRD00DATTAB] = { &HSPRD00TAB_BEGIN_BUFFER,&HSPRD00TAB_END_BUFFER,&HSPRD00DAT_BEGIN_BUFFER,&posistruct5 };
		if (pre_x_DWORD_E9C3C)
			FreeMem_83E80(pre_x_DWORD_E9C3C);
		pre_x_DWORD_E9C3C = (uint8_t*)Malloc_83CD0(3000000);
		x_DWORD_E9C3C = &pre_x_DWORD_E9C3C[200000];
		CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_HFONT3DATTAB]);//2aa1d4
		help_VGA_type_resolution = 8;

		if (extendedFonts)
			LoadFixedFonts(2, (char*)"HFONT3");
	}
	CreateIndexes_6EB90(&filearray_2aa18c[filearrayindex_MSPRD00DATTAB]);//2aa1bc
	LoadTextureData(x_WORD_180660_VGA_type_resolution, D41A0_0.terrain_2FECE.MapType, pdwScreenBuffer_351628);//ok
}

int debugcounter_236100 = 0;
//----- (00055100) --------------------------------------------------------
void sub_55100(char a1)//236100
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
	v3 = v2 * ((int8_t*)D41A0_0.struct_0x6E8E- ZeroPointer);
	v4 = 0;
	v5 = v2 * ((int8_t*)D41A0_0.terrain_2FECE.entity_0x30311 - ZeroPointer);
	//fix

	while (v4 < D41A0_0.stageIndex_0x36E01)
	{
		if (D41A0_0.stages_0x3654C[v4].str_36552_un.dword)
		{
			v7 = D41A0_0.stages_0x3654C[v4].stages_3654C_byte0;
			if ((v7 < 5) && (v7 > 0))
			{
				if (!(D41A0_0.stages_0x3654C[v4].str_3654D_byte1 & 1))
				{
					if ((v7 == 1) || (v7 == 2) || (v7 == 4))
					{
						type_entity_0x30311* temp_0x30311 = D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x30311;
						if (v2 == -1)
						{
							int diff = D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x30311 - D41A0_0.terrain_2FECE.entity_0x30311;
							int sizediff = diff * sizeof(type_entity_0x30311);
							if (((char*)D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x30311 - (char*)D41A0_0.terrain_2FECE.entity_0x30311) % sizeof(type_entity_0x30311) > 0)
								allert_error();
							D41A0_0.stages_0x3654C[v4].str_36552_un.dword = sizediff;
						}
						else
						{
							int count = D41A0_0.stages_0x3654C[v4].str_36552_un.dword / sizeof(type_entity_0x30311);
							int countadd = D41A0_0.stages_0x3654C[v4].str_36552_un.dword % sizeof(type_entity_0x30311);
							if (countadd > 0)allert_error();
							D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x30311 = &D41A0_0.terrain_2FECE.entity_0x30311[count];
						}
#ifdef x32_BIT_ENVIRONMENT
						if (D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x30311 != (type_entity_0x30311*)((uint8_t*)temp_0x30311 + (xBITINT)v5))
							allert_error();  // only for x86
#endif
					}
				}
				else
				{
					type_entity_0x6E8E* temp_0x6E8E = D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x6E8E;
					if (v2 == -1)
					{
						int diff = D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x6E8E - D41A0_0.struct_0x6E8E;
						int sizediff = diff * sizeof(type_shadow_str_0x6E8E);
						if (((char*)D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x6E8E - (char*)D41A0_0.struct_0x6E8E) % sizeof(type_entity_0x6E8E) > 0)
							allert_error();
						D41A0_0.stages_0x3654C[v4].str_36552_un.dword = sizediff;
					}
					else
					{
						int count = D41A0_0.stages_0x3654C[v4].str_36552_un.dword / sizeof(type_shadow_str_0x6E8E);
						int countadd = D41A0_0.stages_0x3654C[v4].str_36552_un.dword % sizeof(type_shadow_str_0x6E8E);
						if (countadd > 0)allert_error();
						D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x6E8E = &D41A0_0.struct_0x6E8E[count];
					}
#ifdef x32_BIT_ENVIRONMENT
					if (D41A0_0.stages_0x3654C[v4].str_36552_un.ptr0x6E8E != (type_entity_0x6E8E*)((uint8_t*)temp_0x6E8E + (xBITINT)v3))
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
		if (i > D41A0_0.countStageVars_0x36E00)
			break;
		v9 = D41A0_0.StageVars2_0x365F4[i].index_0x3647A_0;
		if (v9 >= 3u && (v9 <= 5u || v9 >= 8u && v9 <= 9u))
		{
			if (!(D41A0_0.StageVars2_0x365F4[i].stage_0x3647A_1 & 2))
			{
				type_entity_0x6E8E* temp_0x6E8E = D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.pointer_0x6E8E;
				if (temp_0x6E8E >= Entities_EA3E4[0] && temp_0x6E8E < Entities_EA3E4[1000])
				{
					if (v2 == -1)
					{
						int diff = D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.pointer_0x6E8E - D41A0_0.struct_0x6E8E;
						int sizediff = diff * sizeof(type_shadow_str_0x6E8E);
						if (((char*)D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.pointer_0x6E8E - (char*)D41A0_0.struct_0x6E8E) % sizeof(type_entity_0x6E8E) > 0)
							allert_error();
						D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.dword = sizediff;
					}
					else
					{
						int count = D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.dword / sizeof(type_shadow_str_0x6E8E);
						int countadd = D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.dword % sizeof(type_shadow_str_0x6E8E);
						if (countadd > 0)allert_error();
						D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.pointer_0x6E8E = &D41A0_0.struct_0x6E8E[count];
					}
#ifdef x32_BIT_ENVIRONMENT
					if (D41A0_0.StageVars2_0x365F4[i].str_0x3647C_4.pointer_0x6E8E != (type_entity_0x6E8E*)((uint8_t*)temp_0x6E8E + (xBITINT)v3))
						allert_error();  // only for x86
#endif
				}
			}
		}
	}

	//23620a
	for (j = 1; j < 50; j++)
	{
		if (D41A0_0.str_0x3664C[j].byte_0)
		{
			type_entity_0x6E8E* temp_0x6E8E = D41A0_0.str_0x3664C[j].event_A.pointer_0x6E8E;
			if (v2 == -1)
			{
				int diff = D41A0_0.str_0x3664C[j].event_A.pointer_0x6E8E - D41A0_0.struct_0x6E8E;
				int sizediff = diff * sizeof(type_shadow_str_0x6E8E);
				if (((char*)D41A0_0.str_0x3664C[j].event_A.pointer_0x6E8E - (char*)D41A0_0.struct_0x6E8E) % sizeof(type_entity_0x6E8E) > 0)
					allert_error();
				D41A0_0.str_0x3664C[j].event_A.dword = sizediff;//0x36656
			}
			else
			{
				int count = D41A0_0.str_0x3664C[j].event_A.dword / sizeof(type_shadow_str_0x6E8E);
				int countadd = D41A0_0.str_0x3664C[j].event_A.dword % sizeof(type_shadow_str_0x6E8E);
				if (countadd > 0)allert_error();
				D41A0_0.str_0x3664C[j].event_A.pointer_0x6E8E = &D41A0_0.struct_0x6E8E[count];//0x36656
			}
#ifdef x32_BIT_ENVIRONMENT
			if (D41A0_0.str_0x3664C[j].event_A.pointer_0x6E8E != (type_entity_0x6E8E*)((uint8_t*)temp_0x6E8E + (xBITINT)v3))
				allert_error();  // only for x86
#endif
		}
	}
}

//----- (00057680) --------------------------------------------------------
void sub_57680_FixPointersAfterLoad()//238680
{
	D41A0_0.str_0x3664C[0].event_A.pointer_0x6E8E = nullptr;

	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
	{
		#if defined(x32_BIT_ENVIRONMENT)
			if ((uint32_t)Entities_EA3E4[indexx]->dword_0xA4_164x == 0x2c75e28)
				Entities_EA3E4[indexx]->dword_0xA4_164x = unk_F42B0x;
		#else
			if ((uint64_t)Entities_EA3E4[indexx]->dword_0xA4_164x == 0x2c75e28)//0x014F82E8//0x2c75e28-(uint32_t)&D41A0_BYTESTR_0
				Entities_EA3E4[indexx]->dword_0xA4_164x = unk_F42B0x;
		#endif
	}

	for (int v0 = 0; v0 < D41A0_0.NumberOfPlayers_0xe; v0++)
	{
		Entities_EA3E4[D41A0_0.array_0x2BDE[v0].playerIndex_0x00a_2BE4_11240]->dword_0xA4_164x = &D41A0_0.array_0x2BDE[v0].dword_0x3E6_2BE4_12228;
	}
	for (int indexx = 1; Entities_EA3E4[indexx] < Entities_EA3E4[1000]; indexx++)
		if (Entities_EA3E4[indexx]->class_0x3F_63)
			Entities_EA3E4[indexx]->dword_0xA0_160x = &str_D7BD6[(Entities_EA3E4[indexx]->dword_0xA0_160x - D41A0_0.dword_0x36DF6)+ 59];
}

//----- (000549A0) --------------------------------------------------------
void sub_549A0(type_str_611* a1x, type_str_611* a2x)//2359a0
{
	a1x->array_0x3E9_1001x = a2x->array_0x3E9_1001x;
	a1x->SpellExperience_0x263_611x = a2x->SpellExperience_0x263_611x;
	a1x->SpellLevels_0x41D_1053z = a2x->SpellLevels_0x41D_1053z;
	a1x->array_0x3B5_949x = a2x->array_0x3B5_949x;
	a1x->array_0x437_1079x = a2x->array_0x437_1079x;
}

//----- (00049F90) --------------------------------------------------------
void sub_49F90()//22af90
{
	int iy;
	signed int v2; // ebx
	type_entity_0x6E8E* indexx; // eax
	for (iy = 1; iy < 0x3e8; iy++)
	{
		if (Entities_EA3E4[iy]->class_0x3F_63 && Entities_EA3E4[iy]->struct_byte_0xc_12_15.byte[1] & 4)
			sub_57F20(Entities_EA3E4[iy]);
	}
	D41A0_0.dword_0x35 = -1;
	v2 = 999;
	D41A0_0.dword_0x11e6 = -1;
	do
	{
		indexx = Entities_EA3E4[v2];
		if (indexx->class_0x3F_63)
		{
			if (indexx->struct_byte_0xc_12_15.byte[2] & 2)
			{
				D41A0_0.dword_0x11e6++;
				D41A0_0.dword_0x11EA[D41A0_0.dword_0x11e6] = Entities_EA3E4[v2];
			}
		}
		else
		{
			D41A0_0.dword_0x35++;
			D41A0_0.pointers_0x246[D41A0_0.dword_0x35] = Entities_EA3E4[v2];
		}
		v2--;
	} while (v2 > 0);
}

//----- (00055AB0) --------------------------------------------------------
void sub_55AB0(type_str_0x2BDE* playStr)//236ab0
{
	for (int i = 0; i < 26; i++)
	{
		if (playStr->dword_0x3E6_2BE4_12228.str_611.array_0x3E9_1001x.SpellIndex[spellIndex_D94FF[i]] || playStr->dword_0x3E6_2BE4_12228.str_611.array_0x403_1027x.SpellIndex[spellIndex_D94FF[i]])
		{
			if (!playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]])
			{
				type_entity_0x6E8E* tempEvent = IfSubtypeCallCreatingManaSphere_4A190(&Entities_EA3E4[playStr->playerIndex_0x00a_2BE4_11240]->position_0x4C_76, 15, spellIndex_D94FF[i]);
				if (tempEvent)
				{
					playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]] = tempEvent - D41A0_0.struct_0x6E8E;
					tempEvent->parentId_0x28_40 = Entities_EA3E4[playStr->playerIndex_0x00a_2BE4_11240] - D41A0_0.struct_0x6E8E;
					tempEvent->struct_byte_0xc_12_15.byte[0] |= 1u;
					SetSpell_6D5E0(tempEvent, playStr->dword_0x3E6_2BE4_12228.str_611.array_0x437_1079x.SpellIndex[spellIndex_D94FF[i]]);
				}
			}
		}
		else
		{
			if (playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]])
			{
				playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]] = 0;
				sub_57F20(Entities_EA3E4[playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex_D94FF[i]]]);
			}
		}
	}
	if (playStr->dword_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 != -1 && !playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[playStr->dword_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105])
		playStr->dword_0x3E6_2BE4_12228.str_611.SpellIndexLeft_0x451_1105 = -1;
	if (playStr->dword_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 != -1 && !playStr->dword_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled[playStr->dword_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107])
		playStr->dword_0x3E6_2BE4_12228.str_611.SpellIndexRight_0x453_1107 = -1;
}

//----- (00071990) --------------------------------------------------------
void sub_71990()//252990
{
	char i; // bl
	unsigned __int16 j; // si
	unsigned int v4; // ST04_4
	char v6_return; // [esp+4h] [ebp-4h]

	v6_return = 0;
	if (!x_DWORD_E9C28_str)
		sub_712F0();
	for(int i=0;i<504;i++)
		if (!D41A0_0.array_0x39[i])
			ResetTmap_70D20(i);
	for (i = 2; i && !v6_return; i--)
	{
		for (j = 0; j < 504 && !v6_return; j++)
		{
			if (i == D41A0_0.array_0x39[j] && !str_DWORD_F66F0x[j])
			{
				v4 = sub_70EF0(j);
				if (sub_71E60(x_DWORD_E9C28_str) <= v4)
				{
					if (sub_71E60(x_DWORD_E9C28_str) < 0x400)
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
void sub_71930()//252930
{
	for (int i=0; i < 508; i++) D41A0_0.array_0x39[i] = 0;
	for (int i=0; i < 504; i++) {
		if (str_DWORD_F66F0x[i])
		{
			D41A0_0.array_0x39[i] = 1;
			if (m_LevelSpriteList_F5340[i])
				D41A0_0.array_0x39[i]++;
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

	sub_47130();
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
			//x_DWORD_D4188 = (uint8_t*)filearray1;
			x_DWORD_D4188 = (bitmap_pos_struct2_t*)Malloc_83CD0(file2lenght + file1Lenght);
			if (x_DWORD_D4188)
			{
				qmemcpy(x_DWORD_D4188, (void*)textureBuffer, file2lenght + file1Lenght);
				x_DWORD_D418C = (bitmap_pos_struct2_t*)((int8_t*)x_DWORD_D4188+file1Lenght);
				//x_DWORD_D418C = &x_DWORD_D4188[file1Lenght / sizeof(bitmap_pos_struct2_t)];
				x_DWORD_D4190 = (bitmap_pos_struct2_t*)((int8_t*)x_DWORD_D4188+file2lenght+file1Lenght);
				//x_DWORD_D4190 = &x_DWORD_D4188[(file2lenght + file1Lenght) / sizeof(bitmap_pos_struct2_t)];
				if (vgaTypeResolution & 1)
					sub_98709_create_index_dattab_power(x_DWORD_D418C, x_DWORD_D4190, (uint8_t*)x_DWORD_D4188, x_DWORD_D4188t_spritestr);
				else
					sub_9874D_create_index_dattab(x_DWORD_D418C, x_DWORD_D4190, (uint8_t*)x_DWORD_D4188, x_DWORD_D4188t_spritestr);
			}
		}
	}
}

//----- (0006D5E0) --------------------------------------------------------
void SetSpell_6D5E0(type_entity_0x6E8E* entity, int spellId)//24e5e0
{
	int locSpellId = spellId;
	if (locSpellId > SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].byte_0 - 1)
		locSpellId = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].byte_0 - 1;
	if (entity->word_0x2E_46)
	{
		entity->word_0x2C_44 = locSpellId + 1;
	}
	else
	{
		entity->byte_0x46_70 = locSpellId;
		entity->subSpellIndex_0x2A_42 = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].subSpellIndex_2;
		entity->word_0x30_48 = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].word_0x18;
		entity->byte_0x3B_59 = (SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].fontType_0x1B & 1) == 0;
		entity->byte_0x3C_60 = 0;
		entity->fontTypeIndex_0x3D_61 = 0;
		//fix
		entity->manaRegen_0x88_136 = SPELLS_BEGIN_BUFFER_str[entity->model_0x40_64].subspell[locSpellId].maxManaLimit_A;
		//fix
		int mana = GetSpellManaCost_6D710(Entities_EA3E4[entity->parentId_0x28_40], entity->model_0x40_64, locSpellId);
		entity->maxMana_0x8C_140 = mana;
		if (entity->word_0x30_48)
			mana /= entity->word_0x30_48;
		entity->mana_0x90_144 = mana;
		if (x_D41A0_BYTEARRAY_4_struct.OptionsSettingFlag_24 & 0x20)
		{
			entity->manaRegen_0x88_136 = 0;
			entity->mana_0x90_144 = 1;
		}
	}
}

//----- (000712F0) --------------------------------------------------------
void sub_712F0()//2522f0
{
	x_DWORD_E9C28_str = sub_71B40(x_D41A0_BYTEARRAY_4_struct.dword_0xE6_heapsize_230, 504, (type_x_DWORD_E9C28_str*)x_D41A0_BYTEARRAY_4_struct.pointer_0xE2_heapbuffer_226);
	if (x_DWORD_E9C28_str)
		animations_E9C08x = sub_72120(0x1F8u);
	sub_70A60_open_tmaps();
	sub_71A70_setTmaps(D41A0_0.terrain_2FECE.MapType);
	memset(str_DWORD_F66F0x, 0, 504 * sizeof(type_particle_str**));
	memset(str_F5F10, 0, 504*sizeof(type_particle_str**));
	memset(x_DWORD_F5730, 0, 2016);
	sub_71890();
	sub_718F0();
}

//----- (00071890) --------------------------------------------------------
void sub_71890()//252890
{
	//char result; // al
	int v0y = 0;
	memset(m_LevelSpriteList_F5340, 0, 504);
    while (particlesParameters_D951C[v0y].speed_6 || particlesParameters_D951C[v0y].rotSpeed_8)
    {
        if (particlesParameters_D951C[v0y].byte_11 == -1)
        {
            m_LevelSpriteList_F5340[particlesParameters_D951C[v0y].word_0] = particlesParameters_D951C[v0y].byte_11;
        }
        v0y++;
    }
}

//----- (000718F0) --------------------------------------------------------
void sub_718F0()//2528f0
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
void sub_71A70_setTmaps(MapType_t a1)//252a70
{
	FILE* v1; // edx
	//unsigned __int8 result; // al

	v1 = x_DWORD_DB73C_tmapsfile;
	//result = a1;
	switch (a1)
	{
	case MapType_t::Day:
	{
		v1 = x_DWORD_DB740_tmaps00file;
		break;
	}
	case MapType_t::Night:
	{
		v1 = x_DWORD_DB744_tmaps10file;
		break;
	}
	case MapType_t::Cave:
	{
		v1 = x_DWORD_DB748_tmaps20file;
		break;
	}
	}
	x_DWORD_DB73C_tmapsfile = v1;
	//return result;
}
// DB73C: using guessed type int x_DWORD_DB73C_tmapsfile;
// DB740: using guessed type int x_DWORD_DB740_tmaps00file;
// DB744: using guessed type int x_DWORD_DB744_tmaps10file;
// DB748: using guessed type int x_DWORD_DB748_tmaps20file;

//----- (00072120) --------------------------------------------------------
type_E9C08* sub_72120(unsigned __int16 a1)//253120
{
	unsigned __int16 v1; // di
	type_E9C08* v2x; // esi
	type_E9C08* v3x; // ebx
	type_animations1* v4; // eax
	//int v5; // eax

	v1 = a1;
	sub_83CC0(9);
	v2x = (type_E9C08*)Malloc_83CD0(sizeof(type_E9C08));
	v3x = v2x;
	if (v2x && (v4 = (type_animations1*)Malloc_83CD0(sizeof(type_animations1) * a1), (v2x->dword_2 = v4) != 0))
	{
		v2x->word_0 = a1;
		while (--v1 != 0xffff)
		{
			//v5 = 7 * v1;
			//*(x_DWORD *)(v2x->dword_2 + 4 * v5 + 4) = 0;
			//*(x_DWORD *)(v2x->dword_2 + 4 * v5) = 0;
			v2x->dword_2[v1].Particles_4 = 0;
			v2x->dword_2[v1].dword_0 = 0;
		}
	}
	else if (v2x)
	{
		FreeMem_83E80((uint8_t*)v2x);
		v3x = 0;
	}
	sub_83CC0(10);
	return v3x;
}

//----- (00071B40) --------------------------------------------------------
type_x_DWORD_E9C28_str* sub_71B40(int a1, unsigned __int16 a2, type_x_DWORD_E9C28_str* a3y)//252b40
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
		a3y->word_20 = 0;
		a3y->word_24 = 2;
		a3y->dword_0 = v6;
		a3y->dword_4 = v6;
		result = a3y;
		a3y->word_22 = a2;
		a3y->str_8_data = a3y->data;
		a3y->dword_12x = (subtype_x_DWORD_E9C28_str**)&a3y->data[a2];
		a3y->dword_16x = (uint32_t*)((uint8_t*)a3y->data+((sizeof(subtype_x_DWORD_E9C28_str) * a2) + sizeof(subtype_x_DWORD_E9C28_str*) * a2));
		while (--index != 0xffff)
			a3y->str_8_data[index].dword_4 = 0;
	}
	else
	{
		result = (type_x_DWORD_E9C28_str*)Malloc_83CD0(26);
		//result = v10y;
		if (!result
			|| (v11x = (uint8_t*)Malloc_83CD0(a1), (result->dword_16x = (uint32_t*)v11x) == 0)
			|| (v12x = (uint8_t*)Malloc_83CD0(sizeof(subtype_x_DWORD_E9C28_str) * a2), (result->str_8_data = (subtype_x_DWORD_E9C28_str*)v12x) == 0)
			|| (v13x = (uint8_t*)Malloc_83CD0(sizeof(subtype_x_DWORD_E9C28_str*) * a2), (result->dword_12x = (subtype_x_DWORD_E9C28_str**)v13x) == 0))
		{
			if (result)
			{
				if (result->dword_16x)
				{
					if (result->str_8_data)
						FreeMem_83E80((uint8_t*)result->str_8_data);
					FreeMem_83E80((uint8_t*)result->dword_16x);
				}
				FreeMem_83E80((uint8_t*)result);
			}
			exit(1);
		}
		result->word_20 = 0;
		result->word_24 = 1;
		result->word_22 = a2;
		result->dword_0 = a1;
		result->dword_4 = a1;
		while (--index != 0xffff)
			result->str_8_data[index].dword_4 = 0;
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
		type_entity_0x6E8E* entity2 = Entities_EA3E4[event->dword_0xA4_164x->CastleEntityIndex_0x3A_58];
		if (entity2 <= Entities_EA3E4[0])
		{
			if (event->dword_0xA4_164x->byte_0x1BE_446)
				add3000 = 3000;
			if (add3000)
				result += 3000;
			return result;
		}
		switch (entity2->dword_0x10_16)
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
		if (entity2->dword_0x10_16 >= 7)
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
		if (entity2->dword_0x10_16)
		{
			if (add3000)
				result += 3000;
			return result;
		}
		if (event->dword_0xA4_164x->byte_0x1BE_446)
			add3000 = true;
		if (add3000)
			result += 3000;
		return result;
	}
	return result;
}

//----- (00083CC0) --------------------------------------------------------
void sub_83CC0(char a1)//264cc0
{
	x_BYTE_E29E8 = a1; // FIXME: Can this be removed? Only written, never read.
}
// E29E8: using guessed type char x_BYTE_E29E8;
