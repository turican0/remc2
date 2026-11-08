#include "Mc1Movie.h"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1Menu.h"
#include "Mc1NotPorted.h"
#include "Mc1Shared.h"
#include "Mc1Sound.h"
#include "Mc1System.h"

Type_Mc1FrameHeader frameHeader_AC1A0_AC190 = {};
int32_t frameOffset_AC1B0_AC1A0 = 0;
char frameInfo_AC1B8_AC1A8[256];
void (*frameCallback_AC5C4_AC5B4)() = nullptr;
int16_t musicLoop_AC5DA_AC5CA = 0;
int16_t skipOnInput_90026_90026 = 0;
int32_t framePeriod_9ADC4_9ADC4 = 0;
char moviePath_9ADC8_9ADC8[80];
int16_t savedMouseX_B7314_B7304 = 0;
int16_t savedMouseY_B7316_B7306 = 0;
int16_t savedKey_B7318_B7308 = 0;
int32_t inputChanged_AE410_AE400 = 0;

// 4A1E4 / 4A524; the fourth record never matches (frame 0), its last bytes are the start of the next table
const Type_Mc1MovieEvent intelEvents_4A1E4_4A524[4] = {
	{ 0, 'A', 10, 0 },
	{ 0, 'E', 5, 0 },
	{ 1, 'S', 1, 0 },
	{ 0, 1, 0, 0x054F },
};

static void WaitForEvents()
{
	// port: the original waits for the timer interrupt, the port lets SDL deliver the input and the timer
	PollSdlEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void WaitMovieFrame_10300_10300()
{
	if (skipOnInput_90026_90026 && InputChanged_35710_35AD0())
	{
		*movieStop_AC5DC_AC5CC = 1;
		return;
	}
	if (*movieAllowSkip_AC5DE_AC5CE && (*lastKey_12EF70_12EF60 || *mouseLeftPressed_12EFDE_12EFCE || *mouseRightPressed_12EFDC_12EFCC))
	{
		*movieStop_AC5DC_AC5CC = 1;
		return;
	}
	while (timerTicks_AC5D4_AC5C4 < (uint32_t)framePeriod_9ADC4_9ADC4)
	{
		if (skipOnInput_90026_90026 && InputChanged_35710_35AD0())
		{
			*movieStop_AC5DC_AC5CC = 1;
			return;
		}
		if (*movieAllowSkip_AC5DE_AC5CE
			&& (*lastKey_12EF70_12EF60 || *mouseLeftPressed_12EFDE_12EFCE || *mouseRightPressed_12EFDC_12EFCC))
		{
			*movieStop_AC5DC_AC5CC = 1;
			return;
		}
		WaitForEvents();
	}
	timerTicks_AC5D4_AC5C4 = 0;
}

void ReadMovieFrame_103F0_103F0()
{
	frameOffset_AC1B0_AC1A0 = *movieFileOffset_90028_90028;
	DataFileIO::Read(*movieFile_AC5C0_AC5B0, (uint8_t*)&frameHeader_AC1A0_AC190, 16);
	while (frameHeader_AC1A0_AC190.type_4 != 0xF1FA)
		printf("ERROR UNKNOWN FRAME TYPE\n"); //!!!!! an endless loop in the original
	while (frameHeader_AC1A0_AC190.size_0 >= 0xFA00)
		printf("PAGE SIZE IS > BSCREEN\n"); //!!!!! an endless loop in the original
	DataFileIO::Read(*movieFile_AC5C0_AC5B0, backScreen_AE3FC_AE3EC, frameHeader_AC1A0_AC190.size_0 - 16);
	*movieFileOffset_90028_90028 += frameHeader_AC1A0_AC190.size_0;
}

void DrawMovieFrame_104D0_104D0()
{
	bool paletteChanged = false;
	frameInfo_AC1B8_AC1A8[0] = 0;
	*chunkStream_AC5BC_AC5AC = backScreen_AE3FC_AE3EC;
	if (frameHeader_AC1A0_AC190.type_4 == 0xF100)
	{
		CopyChunk_103C0_103C0(nullptr, frameHeader_AC1A0_AC190.size_0 - 16);
		ReadMovieFrame_103F0_103F0();
		DrawMovieFrame_104D0_104D0();
	}
	else if (frameHeader_AC1A0_AC190.type_4 == 0xF1FA)
	{
		for (uint32_t index = 0; index < frameHeader_AC1A0_AC190.chunks_6; index++)
		{
			uint8_t* chunkStart = *chunkStream_AC5BC_AC5AC;
#pragma pack(push, 1)
			struct {
				uint32_t size;
				uint16_t type;
			} chunk;
#pragma pack(pop)
			CopyChunk_103C0_103C0((uint8_t*)&chunk, 6);
			int32_t pixels = (uint16_t)*movieWidth_AC5D0_AC5C0 * (uint16_t)*movieHeight_AC5D2_AC5C2;
			switch (chunk.type)
			{
			case 4:
				ReadPalette_108C0_108C0();
				strcat(frameInfo_AC1B8_AC1A8, "COLOUR256 ");
				paletteChanged = true;
				break;
			case 7:
				DecodeSs2_10960_10960();
				strcat(frameInfo_AC1B8_AC1A8, "SS2 ");
				break;
			case 11:
				ReadPalette_108C0_108C0();
				strcat(frameInfo_AC1B8_AC1A8, "COLOUR ");
				paletteChanged = true;
				break;
			case 12:
				DecodeLc_10AB0_10AB0();
				strcat(frameInfo_AC1B8_AC1A8, "LC ");
				break;
			case 13:
				memset(*movieScreen_9001C_9001C, 0, pixels);
				strcat(frameInfo_AC1B8_AC1A8, "BLACK ");
				break;
			case 15:
				DecodeBrun_10BD0_10BD0();
				strcat(frameInfo_AC1B8_AC1A8, "BRUN ");
				break;
			case 16:
				CopyChunk_103C0_103C0((uint8_t*)*movieScreen_9001C_9001C, pixels);
				strcat(frameInfo_AC1B8_AC1A8, "COPY ");
				chunk.size = pixels;
				break;
			case 18:
				CopyChunk_103C0_103C0(nullptr, chunk.size - 6);
				strcat(frameInfo_AC1B8_AC1A8, "PSTAMP ");
				break;
			}
			*chunkStream_AC5BC_AC5AC = chunkStart + chunk.size;
		}
	}
	if (frameCallback_AC5C4_AC5B4)
		frameCallback_AC5C4_AC5B4();
	WaitMovieFrame_10300_10300();
	if (paletteChanged)
	{
		WaitBeam_5CC54_5D164(0);
		if (*movieRedrawText_90024_90024)
		{
			PaletteInstall_319A0_319E0(moviePalette_AC2B8_AC2A8);
			sub_24BF0_24BF0(FindNearestColor_5CC70_5D180((const uint8_t*)moviePalette_AC2B8_AC2A8, 0x3F, 0x3F, 0x3F));
		}
	}
	// port: the frame timing is done above, the blit does not limit the frame rate
	if (*subtitlesShown_90091_90091)
	{
		*workScreen_12EFF4_12EFE4 += 0x1A40;
		Blit320_5CDA0_5D2B0(UINT8_MAX);
		*workScreen_12EFF4_12EFE4 -= 0x1A40;
	}
	else
		Blit320_5CDA0_5D2B0(UINT8_MAX);
}

void PlayMovie_107C0_107C0(int16_t allowSkip, int16_t redrawText, const Type_Mc1MovieEvent* events)
{
	*movieRedrawText_90024_90024 = redrawText;
	*movieEventIndex_90030_90030 = 0;
	musicLoop_AC5DA_AC5CA = 0;
	*movieFrame_AC5E0_AC5D0 = 0;
	*movieScreen_9001C_9001C = (TColor*)*workScreen_12EFF4_12EFE4;
	*movieFile_AC5C0_AC5B0 = DataFileIO::CreateOrOpenFile(Mc1DataFilePath(moviePath_9ADC8_9ADC8).c_str(), 0x200);
	if (!*movieFile_AC5C0_AC5B0)
		return; // port: the original reads through the failed handle
	DataFileIO::Read(*movieFile_AC5C0_AC5B0, (uint8_t*)movieHeader_AC5C8_AC5B8, 12);
	// port: remc2 keeps the header words in separate variables
	*movieFrames_AC5CE_AC5BE = movieHeader_AC5C8_AC5B8->frameCount_6;
	*movieWidth_AC5D0_AC5C0 = movieHeader_AC5C8_AC5B8->height_8;
	*movieHeight_AC5D2_AC5C2 = movieHeader_AC5C8_AC5B8->width_alt_10;
	*mouseRightPressed_12EFDC_12EFCC = 0;
	*mouseLeftPressed_12EFDE_12EFCE = 0;
	*movieFileOffset_90028_90028 += 12;
	*lastKey_12EF70_12EF60 = 0;
	*movieStop_AC5DC_AC5CC = *lastKey_12EF70_12EF60;
	InitInputChange_356E0_35AA0();
	*movieAllowSkip_AC5DE_AC5CE = allowSkip;
	if (!*movieStop_AC5DC_AC5CC)
	{
		while ((int16_t)*movieFrame_AC5E0_AC5D0 < (int32_t)(uint16_t)*movieFrames_AC5CE_AC5BE - 1)
		{
			MovieEvents_111B0_111B0(events);
			ReadMovieFrame_103F0_103F0();
			DrawMovieFrame_104D0_104D0();
			(*movieFrame_AC5E0_AC5D0)++;
			if (*movieStop_AC5DC_AC5CC)
				break;
		}
	}
	DataFileIO::Close(*movieFile_AC5C0_AC5B0);
	*movieFile_AC5C0_AC5B0 = nullptr;
}

void MovieEvents_111B0_111B0(const Type_Mc1MovieEvent* events)
{
	int16_t index = *movieEventIndex_90030_90030;
	for (;; index++)
	{
		const Type_Mc1MovieEvent& event = events[index];
		if (event.frame_0 != *movieFrame_AC5E0_AC5D0)
			break;
		*movieEventIndex_90030_90030 = index;
		int16_t value = event.value_3;
		switch (event.command_2)
		{
		case 'A':
		case 'a':
			framePeriod_9ADC4_9ADC4 = value;
			break;
		case 'B':
		case 'b':
			sub_20E60_20E60();
			sub_5CEF0_5D400((uint16_t)value);
			break;
		case 'E':
		case 'e':
			sub_5D010_5D520();
			sub_5D070_5D580((uint8_t)value);
			break;
		case 'L':
		case 'l':
			if (dword_9AF08_9AF08)
				sub_5D230_5D740(dword_9ADC0_9ADC0, dword_9AF08_9AF08, value);
			sub_5D290_5D7A0(value);
			musicLoop_AC5DA_AC5CA = 0;
			break;
		case 'M':
		case 'm':
			sub_5D290_5D7A0(value);
			musicLoop_AC5DA_AC5CA = 0;
			break;
		case 'Z':
		case 'z':
			sub_5D290_5D7A0(value);
			musicLoop_AC5DA_AC5CA = value + 1;
			break;
		case 'O':
			if (*subtitles_90090_90090)
				sub_24AB0_24AB0(value);
			break;
		case 'K':
			sub_24B80_24B80();
			break;
		case 'P':
			sub_24BB0_24BB0(value);
			break;
		case 'Q':
			sub_24B50_24B50((const char*)textPointers_AE238_AE228[value]);
			break;
		case 'R':
		case 'r':
			if (byte_939E4_939E4)
				sub_5D460_5D970(0, value, -1);
			break;
		case 'S':
		case 's':
			if (byte_939E4_939E4)
			{
				if (value)
					sub_5D610_5DB20(0, value);
				else
					sub_5D010_5D520();
			}
			break;
		case 'T':
		case 't':
			if (byte_939E4_939E4)
			{
				if (value)
					sub_5D750_5DC60(0, value);
				else
					sub_5D010_5D520();
			}
			break;
		case 'X':
		case 'x':
			sub_20E60_20E60();
			musicLoop_AC5DA_AC5CA = 0;
			break;
		}
	}
	*movieEventIndex_90030_90030 = index;
	if (musicLoop_AC5DA_AC5CA && sub_5D7F0_5DD00(dword_CBFD8_CBFC8))
	{
		// the looped music ended, start it again
		sub_20E60_20E60();
		sub_5D290_5D7A0(musicLoop_AC5DA_AC5CA - 1);
	}
}

void InitInputChange_356E0_35AA0()
{
	savedMouseX_B7314_B7304 = mousePos_9AD90_9AD90->x;
	savedMouseY_B7316_B7306 = mousePos_9AD90_9AD90->y;
	savedKey_B7318_B7308 = *lastKey_12EF70_12EF60;
	inputChanged_AE410_AE400 = 0;
}

int32_t InputChanged_35710_35AD0()
{
	if (!inputChanged_AE410_AE400)
	{
		if (mousePos_9AD90_9AD90->x != savedMouseX_B7314_B7304 || mousePos_9AD90_9AD90->y != savedMouseY_B7316_B7306
			|| *mouseLeftDown_12EFE4_12EFD4 || *mouseRightDown_12EFE2_12EFD2 || *lastKey_12EF70_12EF60 != savedKey_B7318_B7308)
		{
			savedMouseX_B7314_B7304 = mousePos_9AD90_9AD90->x;
			savedMouseY_B7316_B7306 = mousePos_9AD90_9AD90->y;
			savedKey_B7318_B7308 = *lastKey_12EF70_12EF60;
			inputChanged_AE410_AE400 = 1;
		}
	}
	return inputChanged_AE410_AE400;
}

int32_t CheckQuitKey_4B430_4B770()
{
	// Shift+Q
	int8_t* keys = *pressedKeys_12EEF0_12EEE0;
	if (!keys[0x2A] && !keys[0x36])
		return 0;
	if (*lastKey_12EF70_12EF60 != 0x10)
		return 0;
	menuDone_96880_96880 = 1;
	mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8].exitGame_0x04_4 = 1;
	return 1;
}

void WaitSeconds_4B480_4B7C0(void (*callback)(), int32_t seconds)
{
	bool done = false;
	uint32_t end = timerTicks_AC5D4_AC5C4 + seconds * 120;
	while (!done)
	{
		if (CheckQuitKey_4B430_4B770())
			done = true;
		if (*mouseLeftPressed_12EFDE_12EFCE || *mouseRightPressed_12EFDC_12EFCC || *mouseMiddlePressed_12EFDA_12EFCA
			|| *lastKey_12EF70_12EF60)
		{
			*lastKey_12EF70_12EF60 = 0;
			*mouseMiddlePressed_12EFDA_12EFCA = 0;
			*mouseRightPressed_12EFDC_12EFCC = 0;
			*mouseLeftPressed_12EFDE_12EFCE = 0;
			done = true;
		}
		if (end < timerTicks_AC5D4_AC5C4)
			done = true;
		if (!done && callback)
			callback();
		if (!done)
			WaitForEvents();
	}
}

static void ClearAndBlitScreen()
{
	if (*videoMode_12F02E_12F01E & 1)
		ClearScreen320_411D7_41517(*workScreen_12EFF4_12EFE4, 320, 200, 0);
	else
		ClearScreen640_411FD_4153D(*workScreen_12EFF4_12EFE4, 640, 480, 0);
	BlitScreen();
}

void PlayIntelIntro_4EFC0_4F300()
{
	DetectCpu_19470_19470();
	if (mc1Settings_AE408_AE3F8->fastCpu_0x08_8)
	{
		if (!inputDevices_90B23_90B23)
			SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
		FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		ClearAndBlitScreen();
		sprintf(moviePath_9ADC8_9ADC8, "intro/intel.dat"); // intro\intel.dat
		*mouseMiddlePressed_12EFDA_12EFCA = 0;
		*mouseRightPressed_12EFDC_12EFCC = 0;
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		*lastKey_12EF70_12EF60 = 0;
		skipOnInput_90026_90026 = 0;
		PlayMovie_107C0_107C0(1, 1, intelEvents_4A1E4_4A524);
		WaitSeconds_4B480_4B7C0(nullptr, 1);
		FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		sub_5D010_5D520();
		sub_20E60_20E60();
		ClearAndBlitScreen();
	}
	mainMenuItem_12CBCE_12CBBE = 9;
}
