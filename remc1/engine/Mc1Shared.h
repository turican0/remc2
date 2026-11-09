#pragma once

#include <cstdint>

#include "../../remc2/engine/Animation.h"
#include "../../remc2/engine/Basic.h"
#include "../../remc2/engine/DatTabIndexes.h"
#include "../../remc2/engine/EventsFunctions.h"
#include "../../remc2/engine/GameUI.h"
#include "../../remc2/engine/globals.h"
#include "../../remc2/engine/Graphics.h"
#include "../../remc2/engine/PlayerInput.h"
#include "../../remc2/engine/Sound.h"
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
inline constexpr axis_2d* mousePos_9AD90_9AD90 = &mouse_E3760; // x 9AD90, y 9AD92
inline constexpr int16_t* mouseClickX_9AD8C_9AD8C = &mousePosX_E375C;
inline constexpr int16_t* mouseClickY_9AD8E_9AD8E = &mousePosY_E375E;
inline constexpr int16_t* mouseRightPressed_12EFDC_12EFCC = &mouseRightButton_180744;
inline constexpr int16_t* mouseLeftPressed_12EFDE_12EFCE = &mouseLeftButton_180746;
inline constexpr int* mouseMiddleDown_12EFE0_12EFD0 = &mouseEventFlagB_180748;
inline constexpr int16_t* mouseRightDown_12EFE2_12EFD2 = &mouseRightButton2_18074A;
inline constexpr int16_t* mouseLeftDown_12EFE4_12EFD4 = &mouseLeftButton2_18074C;
inline constexpr uint32_t* screenWidth_12EFF0_12EFE0 = &screenWidth_18062C;
inline constexpr uint32_t* screenHeight_12EFF8_12EFE8 = &screenHeight_180624;
inline constexpr int* renderWidth_93AD4_93AD4 = &iScreenWidth_DE560; // set by 79495 / 799A5 (MC2 BCD45)

// functions
inline constexpr auto ClearScreen320_411D7_41517 = &ClearGraphicsBuffer_72883; // rows of 320 (MC2 72883)
inline constexpr auto ClearScreen640_411FD_4153D = &ClearGraphicsBuffer_72883; // rows of 640 (MC2 728A9)
inline constexpr auto SetVideoMode320x200AndPalette_61B90_620A0 = &VgaSetVideoMode320x200AndPalette_90D6E;
inline constexpr auto SetVideoMode640x480AndPalette_61C30_62140 = &VgaSetVideoMode640x480AndPalette_90E07;
inline constexpr auto FadeInOut_61CC0_621D0 = &VgaPalFadeInFadeOut_90B27; // the last argument is the port's frame delay
inline constexpr auto InstallMouse_5C214_5C724 = &InstallMouse_8CEDF;
inline constexpr auto SetCursor_5C05C_5C56C = &SetCursor_8CD27; // a pointers.tab record
inline constexpr auto Blit320_5CDA0_5D2B0 = &VgaBlit320_90478; // the argument 200 rows, port: max fps
inline constexpr auto Blit640_599B0_59EC0 = &VgaBlit640_75200;
inline constexpr auto DrawBitmap320_60CE0_611F0 = &DrawBitmap320_8F8B0;
inline constexpr auto DrawBitmap640_60D18_61228 = &DrawBitmap640_8F8E8;
inline constexpr auto CopyScreen320_62FA8_634B8 = &CopyScreen; // rows of 320 (MC2 9A128)
inline constexpr auto CopyScreen640_62FC4_634D4 = &CopyScreen; // rows of 640 (MC2 9A144)
inline constexpr auto SetTextPointers_44700_44A40 = &CopySentence_5B870;
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
inline constexpr auto SetViewport_65EB0_663C0 = &SetViewport_A0D50; // x, y, width, height of the bitmaps

// FLI player (MC1 107C0-10BD0, the same addresses in HW; MC2 76160-76540)
inline constexpr FILE** movieFile_AC5C0_AC5B0 = &fmvFile_17DB38;
inline constexpr Type_17DB40* movieHeader_AC5C8_AC5B8 = &fmvHeader_17DB40;
inline constexpr int16_t* movieFrames_AC5CE_AC5BE = &LastKeyframe_17DB46; // header +6, remc2 keeps a copy
inline constexpr int16_t* movieWidth_AC5D0_AC5C0 = &height_17DB48; // header +8, the row length (remc2 calls it height)
inline constexpr int16_t* movieHeight_AC5D2_AC5C2 = &width_17DB4A; // header +0Ah
inline constexpr uint16_t* movieFrame_AC5E0_AC5D0 = &ActualKeyframe_17DB60;
inline constexpr int16_t* movieStop_AC5DC_AC5CC = &stopPlaybackFlag_17DB5A;
inline constexpr int16_t* movieAllowSkip_AC5DE_AC5CE = &allowSkipVideo_17DB5C;
inline constexpr int16_t* movieRedrawText_90024_90024 = &redrawTextInVideo_E12FC;
inline constexpr int16_t* movieEventIndex_90030_90030 = &soundEventIndex_D4004;
inline constexpr int* movieFileOffset_90028_90028 = &fileOffset_E1300;
inline constexpr uint8_t** chunkStream_AC5BC_AC5AC = &chunkStream_17DB50;
inline constexpr TColor** movieScreen_9001C_9001C = &framebuffer_E12F4x;
inline constexpr TColor* moviePalette_AC2B8_AC2A8 = unk_17D838x;
inline constexpr char* subtitles_90090_90090 = &DisplaySubtitles_D41C0;
inline constexpr char* subtitlesShown_90091_90091 = &DisplaySubtitles_D41C1;
inline constexpr uint8_t* lastKey_12EF70_12EF60 = &LastPressedKey_1806E4;
inline constexpr int* mouseMiddlePressed_12EFDA_12EFCA = &mouseEventFlagA_180740;
inline constexpr auto CopyChunk_103C0_103C0 = &CopyAndShiftFrom17DB50_75D70;
inline constexpr auto ReadPalette_108C0_108C0 = &ReadIntroPalettes_76260; // the argument 100h / 40h is not used
inline constexpr auto DecodeSs2_10960_10960 = &DecodeFlcDeltaChunk_76300;
inline constexpr auto DecodeLc_10AB0_10AB0 = &DecodeFliLineCompressChunk_76430;
inline constexpr auto DecodeBrun_10BD0_10BD0 = &DecodeFliByteRunChunk_76540;
inline constexpr char* animDecodeMode_967E4_967E4 = &animDecodeMode_E2A20; // 1 in the main menu
inline constexpr auto SetAnimationPosition_10010_10010 = &SetAnimationPosition_765FC;
inline constexpr auto DecodeAnimationFrame_1002D_1002D = &DecodeAnimationFrame_76619;
inline constexpr FILE** soundEventFile_9ADC0_9ADC0 = &soundEventFile_E3840; // opened by 5C468 (MC2 8D12F)
inline constexpr int* fileReadBuffer_9AF08_9AF08 = &fileReadBuffer_E387C; // set neither in MC1 nor in MC2
inline constexpr auto ReadIntoFileBuffer_5D230_5D740 = &ReadIntoFileBuffer_8F0AB;
inline constexpr auto WaitBeam_5CC54_5D164 = &fix_sub_9A0FC_wait_to_screen_beam; // port: the frame delay argument

// SELECT.EXE, the selector (one executable, one address)
inline constexpr uint8_t** screenBuffer_23144 = &pdwScreenBuffer_351628;
inline constexpr int16_t* videoMode_2317E = &videoMode_180660; // 1 MCGA 320x200, 8 hires
inline constexpr uint8_t* scanCode_23210 = &LastPressedKey_1806E4;
inline constexpr auto SetVideoModeMcga_11460 = &VgaSetVideoMode320x200AndPalette_90D6E;
inline constexpr auto SetVideoModeHires_11500 = &VgaSetVideoMode640x480AndPalette_90E07;
inline constexpr auto CopyScreenMcga_11704 = &CopyScreen; // rows of 320 (MC2 9A128)
inline constexpr auto CopyScreenHires_11720 = &CopyScreen; // rows of 640 (MC2 9A144)
inline constexpr auto PresentMcga_11744 = &VgaBlit320_90478;
inline constexpr auto PresentHires_11780 = &VgaBlit640_75200;
inline constexpr auto SetClipRect_13F90 = &SetViewport_A0D50;
inline constexpr auto ReadKeyboard_1406C = &ReadKeyboard_9D31C;
inline constexpr auto DrawLineMcga_15748 = &DrawLine320_9025C;
inline constexpr auto DrawLineHires_15860 = &DrawLine640_90374;
inline constexpr auto WriteFile_10CF0 = &DataFileIO::WriteFile_98CAA;
