#include "Select.h"

#include <cstdio>
#include <cstring>

#include "../../remc2/engine/read_config.h"
#include "../../remc2/portability/port_filesystem.h"
#include "../../remc2/portability/port_sdl_vga_mouse.h"
#include "../../remc2/portability/port_time.h"
#include "../../remc2/utilities/DataFileIO.h"

SelectState selectState;

const SelectFrame selectFramesHires_202A4[2] = {
	{ 0x30, 0x99, 0xD1, 0x10D },
	{ 0x172, 0x98, 0xD1, 0x10F },
};

// the last two frames are not used by the selector
const SelectFrame selectFramesMcga_202B4[4] = {
	{ 0x30, 0x7E, 0xD4, 0xE4 },
	{ 0x172, 0x7E, 0xD4, 0xE4 },
	{ 0x11C, 0x118, 0x1E, 0x10 },
	{ 0x13B, 0x118, 0x1E, 0x10 },
};

// the original table has 116 bytes, the rest reads following data (zero at that time)
const uint8_t scanCodeChars_202EC[128] = {
	0, 0x27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0x0D, 0, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0x27, '#', 0, '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' ', 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'/', 0, 0, '(', ')', '/', '*', 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, '.', 0x0D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int16_t RunSelect_10010(const std::string& cdPath)
{
	selectState = SelectState();
	selectState.selectedGame_202A0 = SELECTED_NONE;
	selectState.cdPath = cdPath;
	selectState.installPath = cdPath + "/CARPET.CD";

	if (!InitSelect_10080())
		return SELECTED_NONE;
	SelectLoop_10260();
	FreeSelect_10230();

	std::string cpDat = selectState.installPath + "/CP.DAT";
	if (selectState.selectedGame_202A0 == SELECTED_NONE)
	{
		remove(cpDat.c_str());
	}
	else
	{
		FILE* file = DataFileIO::CreateOrOpenFile(cpDat.c_str(), 0x222);
		if (file)
		{
			DataFileIO::WriteFile_98CAA(file, (uint8_t*)&selectState.selectedGame_202A0, 2);
			DataFileIO::Close(file);
		}
		else
			printf("ERROR OPENING FILE C:\\CARPET.CD\\CP.DAT\n");
	}
	return selectState.selectedGame_202A0;
}

bool InitSelect_10080()
{
	printf("\nCopyright (c) 1995\nBullfrog Productions Ltd.\n");
	printf("All Rights Reserved.\n");
	printf("Magic Carpet Plus.\n");
	selectState.videoMode_2317E = SELECT_MODE_MCGA;

	// the original takes MCGA (0x1F500 bytes) only when 0x96100 bytes or the VESA info block cannot be allocated
	selectState.memory_23144.assign(0x96100, 0);
	selectState.screen_23144 = selectState.memory_23144.data();
	selectState.videoMode_2317E = SELECT_MODE_HIRES;
	selectState.background_2312C = selectState.screen_23144 + 0x4B000;
	printf("\nSetting up Hires Screen\n");
	LoadPackedFile_11060((selectState.cdPath + "/data/selp1-0.dat").c_str(), selectState.palette_22E20);
	LoadPackedFile_11060((selectState.cdPath + "/data/sel1-0.dat").c_str(), selectState.background_2312C);

	CreateInstallDirectory_10320("save");
	CreateInstallDirectory_10320("data");
	CreateInstallDirectory_10320("levels");
	SetupInstallFiles_107E0();
	if (selectState.videoMode_2317E & SELECT_MODE_MCGA)
		SetVideoModeMcga_11460(selectState.palette_22E20);
	else
		SetVideoModeHires_11500(selectState.palette_22E20);
	return true;
}

void FreeSelect_10230()
{
	RestoreVideoMode_10D90();
	std::vector<uint8_t>().swap(selectState.memory_23144);
	selectState.screen_23144 = nullptr;
	selectState.background_2312C = nullptr;
}

void SelectLoop_10260()
{
	selectState.cursor_23130 = 0;
	selectState.blinkTime_23128 = mygethundredths();
	while (selectState.selectedGame_202A0 == SELECTED_NONE)
	{
		if (selectState.scanCode_23210 == 1) // Esc
		{
			selectState.selectedGame_202A0 = SELECTED_NONE;
			return;
		}
		if (selectState.videoMode_2317E & SELECT_MODE_MCGA)
			CopyScreenMcga_11704(selectState.background_2312C, selectState.screen_23144, 200);
		else
			CopyScreenHires_11720(selectState.background_2312C, selectState.screen_23144, 480);
		UpdateSelection_10930();
		if (selectState.videoMode_2317E & SELECT_MODE_MCGA)
			PresentMcga_11744();
		else
			PresentHires_11780(480);
		ReadKey_116C8();
	}
}

int16_t PathAccess_10310(const char* path)
{
	// DOS access returns 0 when the path exists, myaccess returns 1
	return myaccess(path, 0) >= 0 ? 0 : -1;
}

uint8_t CreateInstallDirectory_10320(const char* subDirectory)
{
	// the original first checks the drive with _dos_getdiskfree (returns 1 when it fails)
	const std::string& basePath = selectState.installPath;
	if (PathAccess_10310(basePath.c_str()) <= -1 && mymkdir(basePath.c_str()))
		return 2;
	std::string path = basePath + "/" + subDirectory;
	if (PathAccess_10310(path.c_str()) < 0 && mymkdir(path.c_str()))
		return 2;
	return 0;
}

bool FileMissing_10520(const char* path)
{
	//!!!!! the original opens the file to test it and never closes it
	return !file_exists(path);
}

bool PathExists_10540(const char* path)
{
	return PathAccess_10310(path) == 0;
}

uint8_t CopyDataFiles_10560(const char* sourceDirectory, const char* targetDirectory, const char* name)
{
	char sourcePath[144];
	char targetPath[144];
	char progress[100];

	if (!PathExists_10540(sourceDirectory))
		return 2;
	if (!PathExists_10540(targetDirectory))
		return 1;

	snprintf(sourcePath, sizeof(sourcePath), "%s/%s.tab", sourceDirectory, name);
	snprintf(targetPath, sizeof(targetPath), "%s/%s.tab", targetDirectory, name);
	FILE* source = DataFileIO::CreateOrOpenFile(sourcePath, 0x200);
	if (!source)
		return 3;
	FILE* target = DataFileIO::CreateOrOpenFile(targetPath, 0x222);
	if (!target)
	{
		//!!!!! the original leaves the source file open here
		DataFileIO::Close(source);
		return 3;
	}
	uint32_t length = (uint32_t)DataFileIO::FileLengthBytes(source);
	size_t read = DataFileIO::Read(source, selectState.screen_23144, length);
	DataFileIO::WriteFile_98CAA(target, selectState.screen_23144, (uint32_t)read);
	DataFileIO::Close(source);
	DataFileIO::Close(target);

	snprintf(sourcePath, sizeof(sourcePath), "%s/%s.dat", sourceDirectory, name);
	snprintf(targetPath, sizeof(targetPath), "%s/%s.dat", targetDirectory, name);
	source = DataFileIO::CreateOrOpenFile(sourcePath, 0x200);
	if (source)
	{
		target = DataFileIO::CreateOrOpenFile(targetPath, 0x222);
		if (target)
		{
			int32_t copied = 0;
			int32_t total = (int32_t)DataFileIO::FileLengthBytes(source);
			int32_t remaining = total;
			while (remaining)
			{
				int32_t chunk = remaining <= 64000 ? remaining : 64000;
				int32_t done = (int32_t)DataFileIO::Read(source, selectState.screen_23144, chunk);
				if (done <= 0)
					break; // a read error loops forever in the original
				remaining -= done;
				DataFileIO::WriteFile_98CAA(target, selectState.screen_23144, done);
				copied += done;
				int16_t percent = (int16_t)(100 * copied / total);
				if (percent > 100)
					percent = 100;
				snprintf(progress, sizeof(progress), "%d%c", percent, '%');
				printf("\r%s%s", selectState.progressText, progress);
			}
			DataFileIO::Close(target);
		}
		//!!!!! the original leaves the source file open when the target cannot be created
		DataFileIO::Close(source);
	}
	memset(selectState.screen_23144, 0, 64000);
	return 0;
}

void SetupInstallFiles_107E0()
{
	std::string tmaps = selectState.installPath + "/data/tmaps.dat";
	if (FileMissing_10520(tmaps.c_str()))
	{
		std::string target = selectState.installPath + "/data";
		selectState.progressText = "Creating Setup Directories..";
		printf("%s", selectState.progressText);
		CopyDataFiles_10560((selectState.cdPath + "/data").c_str(), target.c_str(), "tmaps");
	}
	printf("\n");
	std::string levels = selectState.installPath + "/levels/levels.dat";
	if (FileMissing_10520(levels.c_str()))
	{
		std::string target = selectState.installPath + "/levels";
		selectState.progressText = "Setup levels..";
		printf("%s", selectState.progressText);
		CopyDataFiles_10560((selectState.cdPath + "/levels").c_str(), target.c_str(), "levels");
	}
}

void UpdateSelection_10930()
{
	if (selectState.scanCode_23210 == 0x4D) // right
		selectState.cursor_23130 = 1;
	else if (selectState.scanCode_23210 == 0x4B) // left
		selectState.cursor_23130 = 0;
	if (selectState.scanCode_23210 == 0x1C) // Enter
		selectState.selectedGame_202A0 = selectState.cursor_23130 ? SELECTED_HIDDEN_WORLDS : SELECTED_MAGIC_CARPET;

	selectState.time_23120 = mygethundredths();
	if ((selectState.time_23120 - selectState.blinkTime_23128) / 25 >= 1)
	{
		selectState.frameVisible_202A2 = !selectState.frameVisible_202A2;
		selectState.blinkTime_23128 = selectState.time_23120;
	}
	if (selectState.frameVisible_202A2)
	{
		if (selectState.videoMode_2317E == SELECT_MODE_MCGA)
			DrawSelectionFrame_10C00(&selectFramesMcga_202B4[selectState.cursor_23130]);
		else
			DrawSelectionFrame_10C00(&selectFramesHires_202A4[selectState.cursor_23130]);
	}
}

void DrawRectMcga_10B00(int32_t x, int32_t y, int32_t width, int32_t height, uint16_t color)
{
	if (width < 2 || height < 2 || x < 0)
		return;
	int32_t right = x + width;
	if (right >= 640 || (right < 0 && x >= 640))
		return;
	if (y < 0)
		return;
	int32_t bottom = y + height;
	if (bottom >= 400 || (bottom < 0 && y >= 400))
		return;
	DrawRectOutlineMcga_11AD0((int16_t)x, (int16_t)y, (int16_t)width, (int16_t)height, color);
}

void DrawRectHires_10B80(int32_t x, int32_t y, int32_t width, int32_t height, uint16_t color)
{
	if (width < 2 || height < 2 || x < 0)
		return;
	int32_t right = x + width;
	if (right >= 640 || (right < 0 && x >= 640))
		return;
	if (y < 0)
		return;
	int32_t bottom = y + height;
	if (bottom >= 480 || (bottom < 0 && y >= 480))
		return;
	DrawRectOutlineHires_11BA0((int16_t)x, (int16_t)y, (int16_t)width, (int16_t)height, color);
}

void DrawSelectionFrame_10C00(const SelectFrame* frame)
{
	uint8_t white = FindNearestColor_11C70(selectState.palette_22E20, 63, 63, 63);
	if (selectState.videoMode_2317E & SELECT_MODE_MCGA)
		DrawRectMcga_10B00(frame->x_0, frame->y_2, frame->width_4, frame->height_6, white);
	else
		DrawRectHires_10B80(frame->x_0, frame->y_2, frame->width_4, frame->height_6, white);
}

void RestoreVideoMode_10D90()
{
	// the original sets the video mode saved at start (int 10h), the port closes the window
	VGA_close();
}

int32_t LoadPackedFile_11060(const char* path, uint8_t* buffer)
{
	FILE* file = DataFileIO::CreateOrOpenFile(path, 0x200);
	if (!file)
		return -1;
	DataFileIO::Close(file);
	int result = DataFileIO::ReadFileAndDecompress(path, &buffer);
	if (result == -2)
	{
		printf("ERROR decompressing %s\n", path);
		return -2;
	}
	return result;
}

void SetVideoModeMcga_11460(uint8_t* palette)
{
	selectState.screenWidth_23140 = 320;
	selectState.screenHeight_23148 = 200;
	VGA_Init(windowResWidth, windowResHeight, 320, 200, maintainAspectRatio, displayIndex);
	VGA_Set_Palette(palette);
	// the mouse ranges set by the original (int 33h) are not used, the selector has no mouse handler
	SetClipRect_13F90(0, 0, 320, 200);
}

void SetVideoModeHires_11500(uint8_t* palette)
{
	selectState.screenWidth_23140 = 640;
	selectState.screenHeight_23148 = 480;
	VGA_Init(windowResWidth, windowResHeight, 640, 480, maintainAspectRatio, displayIndex);
	VGA_Set_Palette(palette);
	SetClipRect_13F90(0, 0, 640, 480);
}

uint8_t ReadKey_116C8()
{
	ReadKeyboard_1406C();
	if (selectState.scanCode_23210 >= 0x80)
		return 0;
	return scanCodeChars_202EC[selectState.scanCode_23210];
}

void CopyScreenMcga_11704(const uint8_t* source, uint8_t* target, uint16_t lines)
{
	memcpy(target, source, lines * 320);
}

void CopyScreenHires_11720(const uint8_t* source, uint8_t* target, uint16_t lines)
{
	memcpy(target, source, lines * 640);
}

void PresentMcga_11744()
{
	// the original hides the mouse cursor around the copy when a mouse handler is installed (never in the selector)
	VGA_Blit(selectState.screen_23144);
}

void PresentHires_11780(uint16_t /*lines*/)
{
	// the original copies the lines through the VESA banks
	VGA_Blit(selectState.screen_23144);
}

void DrawRectOutlineMcga_11AD0(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
	width--;
	height--;
	DrawLineMcga_15748(x, y, (int16_t)(x + width), y, color, 0);
	DrawLineMcga_15748((int16_t)(x + width), y, (int16_t)(x + width), (int16_t)(y + height + 1), color, 0);
	DrawLineMcga_15748(x, (int16_t)(y + height), (int16_t)(x + width + 1), (int16_t)(y + height), color, 0);
	DrawLineMcga_15748(x, y, x, (int16_t)(y + height), color, 0);
}

void DrawRectOutlineHires_11BA0(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
	width--;
	height--;
	DrawLineHires_15860(x, y, (int16_t)(x + width), y, color, 0);
	DrawLineHires_15860((int16_t)(x + width), y, (int16_t)(x + width), (int16_t)(y + height + 1), color, 0);
	DrawLineHires_15860(x, (int16_t)(y + height), (int16_t)(x + width + 1), (int16_t)(y + height), color, 0);
	DrawLineHires_15860(x, y, x, (int16_t)(y + height), color, 0);
}

uint8_t FindNearestColor_11C70(const uint8_t* palette, uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t count = (selectState.videoMode_2317E & 6) ? 16 : 256;
	int32_t bestDistance = 9999;
	//!!!!! the original returns an uninitialized value when no color is closer than 9999
	uint8_t bestIndex = 0;
	const uint8_t* color = palette;
	for (int16_t index = 0; index < count; index++, color += 3)
	{
		int32_t dg = green - color[1];
		int32_t dr = red - color[0];
		int32_t db = blue - color[2];
		int32_t distance = dg * dg + dr * dr + db * db;
		if ((int16_t)distance < (int16_t)bestDistance)
		{
			bestDistance = distance;
			bestIndex = (uint8_t)index;
		}
	}
	return bestIndex;
}

void SetClipRect_13F90(int16_t x, int16_t y, int16_t width, int16_t height)
{
	selectState.clipMinX_23158 = x;
	selectState.clipMinY_23168 = y;
	selectState.clipWidth_23150 = width;
	selectState.clipHeight_23178 = height;
	selectState.clipMaxX_23174 = x + width;
	selectState.clipMaxY_23160 = y + height;
}

void ReadKeyboard_1406C()
{
	int16_t shift = VGA_get_shift_status();
	selectState.shiftFlags_23211 = 0;
	if (shift & 3)
		selectState.shiftFlags_23211 = 1;
	if (shift & 4)
		selectState.shiftFlags_23211 |= 2;
	if (shift & 8)
		selectState.shiftFlags_23211 |= 4;
	selectState.scanCode_23210 = 0;
	if (VGA_check_standart_input_status())
		selectState.scanCode_23210 = (uint8_t)(VGA_read_char_from_buffer() >> 8);
}

void DrawLineMcga_15748(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int16_t /*flags*/)
{
	// coordinates are in 640x400, the buffer is 320x200
	int16_t minX = (int16_t)(selectState.clipMinX_23158 << 1);
	int16_t maxX = (int16_t)(selectState.clipMaxX_23174 << 1);
	int16_t minY = (int16_t)(selectState.clipMinY_23168 << 1);
	int16_t maxY = (int16_t)(selectState.clipMaxY_23160 << 1);
	if (x1 < minX)
		x1 = minX;
	if (x2 < minX || x1 >= maxX)
		return;
	if (x2 >= maxX)
		x2 = maxX;
	if (y1 < minY)
		y1 = minY;
	if (y2 < minY || y1 >= maxY)
		return;
	if (y2 >= maxY)
		y2 = maxY;
	uint8_t* target = selectState.screen_23144 + ((uint16_t)x1 >> 1) + ((uint16_t)y1 >> 1) * 320;
	// flag 4 (translucent through the table at 20858) is never used by the selector
	uint16_t lengthX = (uint16_t)(x2 - x1);
	if (lengthX)
	{
		memset(target, (uint8_t)color, lengthX >> 1);
		return;
	}
	//!!!!! a zero length makes the original loop 2^32 times, never happens for the selector frames
	for (uint16_t count = (uint16_t)(y2 - y1) >> 1; count; count--, target += 320)
		*target = (uint8_t)color;
}

void DrawLineHires_15860(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, int16_t /*flags*/)
{
	int16_t minX = (int16_t)selectState.clipMinX_23158;
	int16_t maxX = (int16_t)selectState.clipMaxX_23174;
	int16_t minY = (int16_t)selectState.clipMinY_23168;
	int16_t maxY = (int16_t)selectState.clipMaxY_23160;
	if (x1 < minX)
		x1 = minX;
	if (x2 < minX || x1 >= maxX)
		return;
	if (x2 >= maxX)
		x2 = maxX;
	if (y1 < minY)
		y1 = minY;
	if (y2 < minY || y1 >= maxY)
		return;
	if (y2 >= maxY)
		y2 = maxY;
	uint8_t* target = selectState.screen_23144 + (uint16_t)x1 + (uint16_t)y1 * 640;
	uint16_t lengthX = (uint16_t)(x2 - x1);
	if (lengthX)
	{
		memset(target, (uint8_t)color, lengthX);
		return;
	}
	//!!!!! a zero length makes the original loop 2^32 times, never happens for the selector frames
	for (uint16_t count = (uint16_t)(y2 - y1); count; count--, target += 640)
		*target = (uint8_t)color;
}
