#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Magic Carpet Plus selector (SELECT.EXE), addresses are IDA addresses of SELECT.EXE;
// the functions and variables shared with remc2 are in Mc1Shared.h (screen, video mode, viewport, lines, keyboard)

enum SelectVideoMode : uint16_t {
	SELECT_MODE_MCGA = 1,  // 320x200
	SELECT_MODE_HIRES = 8, // 640x480 VESA 0x101
};

enum SelectedGame : int16_t {
	SELECTED_NONE = -1,
	SELECTED_MAGIC_CARPET = 1,
	SELECTED_HIDDEN_WORLDS = 2,
};

typedef struct {
	int16_t x_0;
	int16_t y_2;
	int16_t width_4;
	int16_t height_6;
} SelectFrame;

typedef struct {
	int16_t selectedGame_202A0;
	uint8_t frameVisible_202A2;
	uint8_t cursor_23130;      // 0 left, 1 right
	uint32_t blinkTime_23128;
	uint32_t time_23120;
	uint8_t palette_22E20[768];
	std::vector<uint8_t> memory_23144; // screen buffer (screenBuffer_23144) followed by the background
	uint8_t* background_2312C;
	std::string cdPath;     // CD root (MC1 folder)
	std::string installPath; // C:\CARPET.CD
	const char* progressText; // text position of the copy progress
} SelectState;

extern SelectState selectState;
extern const SelectFrame selectFramesHires_202A4[2];
extern const SelectFrame selectFramesMcga_202B4[4];
extern const uint8_t scanCodeChars_202EC[128];

int16_t RunSelect_10010(const std::string& cdPath);
bool InitSelect_10080();
void FreeSelect_10230();
void SelectLoop_10260();
int16_t PathAccess_10310(const char* path);
uint8_t CreateInstallDirectory_10320(const char* subDirectory);
bool FileMissing_10520(const char* path);
bool PathExists_10540(const char* path);
uint8_t CopyDataFiles_10560(const char* sourceDirectory, const char* targetDirectory, const char* name);
void SetupInstallFiles_107E0();
void UpdateSelection_10930();
void DrawRectMcga_10B00(int32_t x, int32_t y, int32_t width, int32_t height, uint16_t color);
void DrawRectHires_10B80(int32_t x, int32_t y, int32_t width, int32_t height, uint16_t color);
void DrawSelectionFrame_10C00(const SelectFrame* frame);
int32_t LoadPackedFile_11060(const char* path, uint8_t* buffer);
uint8_t ReadKey_116C8();
void DrawRectOutlineMcga_11AD0(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
void DrawRectOutlineHires_11BA0(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
uint8_t FindNearestColor_11C70(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue);
