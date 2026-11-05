#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Magic Carpet Plus selector (SELECT.EXE), addresses are IDA addresses of SELECT.EXE

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
	uint16_t videoMode_2317E;
	int16_t selectedGame_202A0;
	uint8_t frameVisible_202A2;
	uint8_t cursor_23130;      // 0 left, 1 right
	uint8_t scanCode_23210;
	uint8_t shiftFlags_23211;
	uint32_t blinkTime_23128;
	uint32_t time_23120;
	int32_t screenWidth_23140;
	int32_t screenHeight_23148;
	int32_t clipMinX_23158;
	int32_t clipMinY_23168;
	int32_t clipWidth_23150;
	int32_t clipHeight_23178;
	int32_t clipMaxX_23174;
	int32_t clipMaxY_23160;
	uint8_t palette_22E20[768];
	std::vector<uint8_t> memory_23144; // screen buffer followed by the background
	uint8_t* screen_23144;
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
void RestoreVideoMode_10D90();
int32_t LoadPackedFile_11060(const char* path, uint8_t* buffer);
void SetVideoModeMcga_11460(uint8_t* palette);
void SetVideoModeHires_11500(uint8_t* palette);
uint8_t ReadKey_116C8();
void CopyScreenMcga_11704(const uint8_t* source, uint8_t* target, uint16_t lines);
void CopyScreenHires_11720(const uint8_t* source, uint8_t* target, uint16_t lines);
void PresentMcga_11744();
void PresentHires_11780(uint16_t lines);
void DrawRectOutlineMcga_11AD0(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
void DrawRectOutlineHires_11BA0(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
uint8_t FindNearestColor_11C70(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue);
void SetClipRect_13F90(int16_t x, int16_t y, int16_t width, int16_t height);
void ReadKeyboard_1406C();
void DrawLineMcga_15748(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int16_t flags);
void DrawLineHires_15860(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int16_t flags);
