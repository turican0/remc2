#pragma once

#include <cstdint>

#include "../../remc2/engine/Basic.h"
#include "../../remc2/engine/DatTabIndexes.h"
#include "../../remc2/engine/EventsFunctions.h"
#include "../../remc2/engine/globals.h"
#include "../../remc2/engine/Graphics.h"
#include "../../remc2/engine/PlayerInput.h"
#include "../../remc2/portability/port_sdl_vga_mouse.h"
#include "../../remc2/utilities/DataFileIO.h"
#include "../../remc2/utilities/DataFileRNC.h"
#include "../../remc2/utilities/Maths.h"

// MC1 variables and functions that are the same code in MC2: the MC1 name with the MC1 and HW addresses is a
// constexpr pointer to the remc2 one (a direct access / call in the optimized build, no copy of the code)

// variables
inline constexpr uint8_t** workScreen_12EFF4_12EFE4 = &pdwScreenBuffer_351628; // *WScreen
inline constexpr int16_t* videoMode_12F02E_12F01E = &videoMode_180660; // 1 MCGA 320x200, 8 hires
inline constexpr int* mouseInstalled_9AD98_9AD98 = &mouseInstalled_E3768;
inline constexpr uint8_t** cursorData_12EFC8_12EFB8 = &cursorData_180730;
inline constexpr uint8_t** cursorBackground_12EF98_12EF88 = &cursorBackground_180700;
inline constexpr uint8_t** cursorBackground2_12EF88_12EF78 = &cursorBackground2_1806F0;
inline constexpr int8_t (*pressedKeys_12EEF0_12EEE0)[128] = &pressedKeys_180664;
inline constexpr uint32_t (*uvTable_902DC_902DC)[32][8] = &UVTable_D4350; // the same initial data
inline constexpr int* screenWidth_93AD4_93AD4 = &iScreenWidth_DE560; // set by 79495 / 799A5 (MC2 BCD45)

// functions
inline constexpr auto ClearScreen320_411D7_41517 = &ClearGraphicsBuffer_72883; // rows of 320 (MC2 72883)
inline constexpr auto ClearScreen640_411FD_4153D = &ClearGraphicsBuffer_72883; // rows of 640 (MC2 728A9)
inline constexpr auto SetVideoMode320x200AndPalette_61B90_620A0 = &VgaSetVideoMode320x200AndPalette_90D6E;
inline constexpr auto SetVideoMode640x480AndPalette_61C30_62140 = &VgaSetVideoMode640x480AndPalette_90E07;
inline constexpr auto FadeInOut_61CC0_621D0 = &VgaPalFadeInFadeOut_90B27; // the last argument is the port's frame delay
inline constexpr auto InstallMouse_5C214_5C724 = &InstallMouse_8CEDF;
inline constexpr auto SetCursor_5C05C_5C56C = &SetCursor_8CD27; // a pointers.tab record
inline constexpr auto ClearPressedKeys_36D83_37143 = &ClearPressedKeys_753D0; // and hooks int 9 (SDL events in the port)
inline constexpr auto PaletteInstall_319A0_319E0 = &VgaPaletteInstall_41A90;
inline constexpr auto ClearScreenAndBlit_5B560_5BA70 = &ClearScreenAndBlit_75C50;
inline constexpr auto FillUVTable_29BA0_29BE0 = &FillD4350Table_3B4D0;
inline constexpr auto IntegerSqrt_410CE_4140E = &Maths::IntegerSqrt_7277A;
inline constexpr auto OpenWriteClose_62E60_63370 = &DataFileIO::OpenWriteClose_98C48;
inline constexpr auto WriteFile_62ED0_633E0 = &DataFileIO::WriteFile_98CAA;
inline constexpr auto DecompressRnc_62B60_63070 = &DataFileRNC::Decompress;
inline constexpr auto CreateIndexDatTabDoubled_65D70_66280 = &CreateIndexDatTabDoubled_98709;
inline constexpr auto CreateIndexDatTab_65DC0_662D0 = &CreateIndexDatTab_9874D;
