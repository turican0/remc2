#include "Select.h"

#include <cstdio>
#include <cstring>

#include "Mc1Shared.h"
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
			WriteFile_10CF0(file, (uint8_t*)&selectState.selectedGame_202A0, 2);
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
	*videoMode_2317E = SELECT_MODE_MCGA;

	// the original takes MCGA (0x1F500 bytes) only when 0x96100 bytes or the VESA info block cannot be allocated
	selectState.memory_23144.assign(0x96100, 0);
	*screenBuffer_23144 = selectState.memory_23144.data();
	*videoMode_2317E = SELECT_MODE_HIRES;
	selectState.background_2312C = *screenBuffer_23144 + 0x4B000;
	printf("\nSetting up Hires Screen\n");
	LoadPackedFile_11060((selectState.cdPath + "/data/selp1-0.dat").c_str(), selectState.palette_22E20);
	LoadPackedFile_11060((selectState.cdPath + "/data/sel1-0.dat").c_str(), selectState.background_2312C);

	CreateInstallDirectory_10320("save");
	CreateInstallDirectory_10320("data");
	CreateInstallDirectory_10320("levels");
	SetupInstallFiles_107E0();
	// port: opens the window, it stays open for the game
	VGA_Init(windowResWidth, windowResHeight, gameResWidth, gameResHeight, maintainAspectRatio, displayIndex);
	if (*videoMode_2317E & SELECT_MODE_MCGA)
		SetVideoModeMcga_11460((TColor*)selectState.palette_22E20);
	else
		SetVideoModeHires_11500((TColor*)selectState.palette_22E20);
	return true;
}

void FreeSelect_10230()
{
	// 10D90 restores the text mode of the start (MC2 8C21F, removed in remc2); Mc1Main closes the window
	std::vector<uint8_t>().swap(selectState.memory_23144);
	*screenBuffer_23144 = nullptr;
	selectState.background_2312C = nullptr;
}

void SelectLoop_10260()
{
	selectState.cursor_23130 = 0;
	selectState.blinkTime_23128 = mygethundredths();
	while (selectState.selectedGame_202A0 == SELECTED_NONE)
	{
		if (*scanCode_23210 == 1) // Esc
		{
			selectState.selectedGame_202A0 = SELECTED_NONE;
			return;
		}
		if (*videoMode_2317E & SELECT_MODE_MCGA)
			CopyScreenMcga_11704(selectState.background_2312C, *screenBuffer_23144, 320, 200);
		else
			CopyScreenHires_11720(selectState.background_2312C, *screenBuffer_23144, 640, 480);
		UpdateSelection_10930();
		if (*videoMode_2317E & SELECT_MODE_MCGA)
			PresentMcga_11744(0);
		else
			PresentHires_11780(480, 0);
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
	size_t read = DataFileIO::Read(source, *screenBuffer_23144, length);
	WriteFile_10CF0(target, *screenBuffer_23144, (uint32_t)read);
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
				int32_t done = (int32_t)DataFileIO::Read(source, *screenBuffer_23144, chunk);
				if (done <= 0)
					break; // a read error loops forever in the original
				remaining -= done;
				WriteFile_10CF0(target, *screenBuffer_23144, done);
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
	memset(*screenBuffer_23144, 0, 64000);
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
	if (*scanCode_23210 == 0x4D) // right
		selectState.cursor_23130 = 1;
	else if (*scanCode_23210 == 0x4B) // left
		selectState.cursor_23130 = 0;
	if (*scanCode_23210 == 0x1C) // Enter
		selectState.selectedGame_202A0 = selectState.cursor_23130 ? SELECTED_HIDDEN_WORLDS : SELECTED_MAGIC_CARPET;

	selectState.time_23120 = mygethundredths();
	if ((selectState.time_23120 - selectState.blinkTime_23128) / 25 >= 1)
	{
		selectState.frameVisible_202A2 = !selectState.frameVisible_202A2;
		selectState.blinkTime_23128 = selectState.time_23120;
	}
	if (selectState.frameVisible_202A2)
	{
		if (*videoMode_2317E == SELECT_MODE_MCGA)
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
	if (*videoMode_2317E & SELECT_MODE_MCGA)
		DrawRectMcga_10B00(frame->x_0, frame->y_2, frame->width_4, frame->height_6, white);
	else
		DrawRectHires_10B80(frame->x_0, frame->y_2, frame->width_4, frame->height_6, white);
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

uint8_t ReadKey_116C8()
{
	ReadKeyboard_1406C();
	if (*scanCode_23210 >= 0x80)
		return 0;
	return scanCodeChars_202EC[*scanCode_23210];
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
	uint16_t count = (*videoMode_2317E & 6) ? 16 : 256;
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
