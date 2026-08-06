#include "Animation.h"

__int16 redrawTextInVideo_E12FC = 1; // weak
__int16 soundEventIndex_D4004 = 0; // weak//index of act key in anim
//__int16 x_WORD_17DB58; // weak
uint16_t ActualKeyframe_17DB60; // weak
TColor* framebuffer_E12F4x = 0; // weak
FILE* x_DWORD_17DB38_intro_file_handle; // weak
Type_17DB40 unk_17DB40str; // weak

int16_t LastKeyframe_17DB46; // weak
int16_t height_17DB48; // weak
int16_t width_17DB4A; // weak

int fileOffset_E1300 = 0; // weak

__int16 stopPlaybackFlag_17DB5A; // weak

__int16 allowSkipVideo_17DB5C; // weak

int x_DWORD_EA3B4; // bool

__int16 x_WORD_F42AE; // weak

__int16 x_WORD_F42A8; // weak

char DisplaySubtitles_D41C0 = 0; // weak

int x_DWORD_17D730; // weak

uint32_t x_DWORD_17D720[4]; // weak 0

uint16_t x_WORD_17D724; // weak 4
uint16_t x_WORD_17D726; // weak 6

char x_BYTE_17D738[256]; // idb

__int16 x_WORD_E12FE = 0; // weak

long AnimCurrentTick = 0;
std::chrono::system_clock::time_point AnimCurrentTime = std::chrono::system_clock::now();
float AnimTickTime = 8.4f;

//----- (00076160) --------------------------------------------------------
void PlayInfoFmv(__int16 allowSkip, __int16 redrawText, Type_SoundEvent_E17CC* pSoundEvent, char* path)//sub_76160 - 257160
{
	FILE* tempfile;
	redrawTextInVideo_E12FC = redrawText;
	soundEventIndex_D4004 = 0;
	//x_WORD_17DB58 = 0;//not used
	ActualKeyframe_17DB60 = 0;
	framebuffer_E12F4x = (TColor*)pdwScreenBuffer_351628;
	tempfile = DataFileIO::CreateOrOpenFile(path, 512);
	x_DWORD_17DB38_intro_file_handle = tempfile;
	if (tempfile)
	{
		DataFileIO::Read(tempfile, (uint8_t*)&unk_17DB40str, sizeof(Type_17DB40));//ecx=12
		LastKeyframe_17DB46 = unk_17DB40str.frameCount_6;
		height_17DB48 = unk_17DB40str.height_8;
		width_17DB4A = unk_17DB40str.width_alt_10;
		x_WORD_180744_mouse_right_button = 0;
		x_WORD_180746_mouse_left_button = 0;
		fileOffset_E1300 += 12;
		LastPressedKey_1806E4 = 0;
		stopPlaybackFlag_17DB5A = 0;
		FlvInitSet_473B0();//2283b0
		allowSkipVideo_17DB5C = allowSkip;

		AnimCurrentTick = 0;
		AnimCurrentTime = std::chrono::system_clock::now();
		do
		{
			SetFrameStart(std::chrono::system_clock::now());
			if (stopPlaybackFlag_17DB5A)
				break;
			if (ActualKeyframe_17DB60 >= LastKeyframe_17DB46 - 1)//34eb60 a 34eb46
				break;
			PlayIntoSoundEvents_1B280(pSoundEvent);
			ReadFrame_75DB0();//256db0 - read header
			DrawFrame_75E70();//256e70 - draw intro frame
			ActualKeyframe_17DB60++;
		} while (LastPressedKey_1806E4 != 1);//while not key pressed
		DataFileIO::Close(x_DWORD_17DB38_intro_file_handle);
	}
}

//----- (000473B0) --------------------------------------------------------
void FlvInitSet_473B0()//2283b0
{
	x_DWORD_EA3B4 = 0;
	x_WORD_F42AE = LastPressedKey_1806E4;
	x_WORD_F42A8 = 64;
}

//----- (0001B280) --------------------------------------------------------
void PlayIntoSoundEvents_1B280(Type_SoundEvent_E17CC* pSoundEvent)//1fc280
{
	while (1)
	{
		if (ActualKeyframe_17DB60 != pSoundEvent[soundEventIndex_D4004].startFrame)
			break;
		if (pSoundEvent[soundEventIndex_D4004].index == -1)
			break;
		switch (pSoundEvent[soundEventIndex_D4004].key_2)
		{
		case 'A':
		case 'a':
			x_DWORD_E3844 = pSoundEvent[soundEventIndex_D4004].index;
			break;
		case 'B':
		case 'b':
			StopMusic_8E020();
			InitMusicBank_8EAD0(pSoundEvent[soundEventIndex_D4004].index);
			break;
		case 'D':
		case 'd':
			StartMusic_8E160(pSoundEvent[soundEventIndex_D4004].index, 0x64u);
			break;
		case 'E':
		case 'e':
			EndSample_8D8F0();
			LoadSounds_84300(pSoundEvent[soundEventIndex_D4004].index);
			break;
		case 'F':
		case 'f':
			//Stop loop
			if (soundAble_E3798)
				Update_Playing_Sample_Status_8F710(0, pSoundEvent[soundEventIndex_D4004].index, 0, 4u, 1);
			break;
		case 'H':
		case 'h':
			if (soundAble_E3798)
				PlaySample_8F100(0, pSoundEvent[soundEventIndex_D4004].index, 0, 64, 0x64u, -1, IfNotPlayingPlaySample);
			break;
		case 'K':
		case 'W':
			sub_2EC30_clear_img_mem();
			break;
		case 'L':
		case 'l':
			if (x_DWORD_E387C)
				sub_8F0AB(x_FILE_E3840, /*x_DWORD_E387C, */pSoundEvent[soundEventIndex_D4004].index);
			StartMusic_8E160(pSoundEvent[soundEventIndex_D4004].index, 0x7Fu);
			break;
		case 'M':
		case 'Z':
		case 'm':
		case 'z':
			StartMusic_8E160(pSoundEvent[soundEventIndex_D4004].index, 0x7Fu);
			break;
		case 'O':
		case 'o':
			//Materialisation Sound
			if (soundAble_E3798)
				Update_Playing_Sample_Status_8F710(0, pSoundEvent[soundEventIndex_D4004].index, 0x7Fu, 2u, 0);
			break;
		case 'P':
		case 'p':
			//People Sound
			if (soundAble_E3798)
				Update_Playing_Sample_Status_8F710(0, pSoundEvent[soundEventIndex_D4004].index, 0x50u, 2u, 0);
			break;
		case 'Q':
			sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[pSoundEvent[soundEventIndex_D4004].index]);
			break;
		case 'R':
		case 'r':
			if (soundAble_E3798)
				PlaySample_8F100(0, pSoundEvent[soundEventIndex_D4004].index, 127, 64, 0x64u, -1, IfNotPlayingPlaySample);
			break;
		case 'S':
		case 's':
			if (soundAble_E3798)
			{
				if (pSoundEvent[soundEventIndex_D4004].index)
					PlaySample_8F100(0, pSoundEvent[soundEventIndex_D4004].index, 127, 64, 0x64u, 0, IfNotPlayingPlaySample);
				else
					EndSample_8D8F0();
			}
			break;
		case 'T':
		case 't':
			if (soundAble_E3798)
			{
				if (pSoundEvent[soundEventIndex_D4004].index)
					AilEndSamplePlayingByIndex_8F420(0, pSoundEvent[soundEventIndex_D4004].index);
				else
					EndSample_8D8F0();
			}
			break;
		case 'U':
			sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[pSoundEvent[soundEventIndex_D4004].index]);
			break;
		case 'V':
			if (DisplaySubtitles_D41C0)
				StartSubtitles_2EB60();
			break;
		case 'X':
		case 'x':
			StopMusic_8E020();
			break;
		case 'Y':
			sub_2EC60();
			break;
		default:
			break;
		}
		soundEventIndex_D4004++;
	}
}

//----- (00075DB0) --------------------------------------------------------
void ReadFrame_75DB0()//256db0
{
	x_DWORD_17D730 = fileOffset_E1300;
	DataFileIO::Read(x_DWORD_17DB38_intro_file_handle, (uint8_t*)x_DWORD_17D720, 16);
	x_WORD_17D724 = x_DWORD_17D720[1] & 0xffff;
	x_WORD_17D726 = (x_DWORD_17D720[1] & 0xffff0000) >> 16;
	while (x_WORD_17D724 != 0xf1fa/*-3590*/)
		Logger->error("ERROR UNKNOWN FRAME TYPE");
	DataFileIO::Read(x_DWORD_17DB38_intro_file_handle, x_DWORD_E9C38_smalltit, x_DWORD_17D720[0] - 16);
	fileOffset_E1300 += x_DWORD_17D720[0];
}

int(/*__fastcall*/ *x_DWORD_17DB3C)(); // weak

#pragma pack(push, 1)
typedef struct {
	uint32_t size;
	int16_t  type;   // type chunk:
	//   4  = COLOUR256 (256-color palette)
	//   7  = SS2       (delta/RLE frame)
	//   11 = COLOUR    (64-color palette)
	//   12 = LC        (line compressed)
	//   13 = BLACK     (clear frame to black)
	//   15 = BRUN      (byte run-length compressed)
	//   16 = COPY      (raw uncompressed frame)
	//   18 = PSTAMP    (postage stamp / thumbnail)
} FliChunkHeader;
#pragma pack(pop)
//----- (00075E70) --------------------------------------------------------
void DrawFrame_75E70()//256e70
{
	unsigned int frameChunkIndex;
	FliChunkHeader chunkHeader;
	uint8_t* chunkStreamPos;
	bool paletteChanged = false;
	x_DWORD_17DB50 = x_DWORD_E9C38_smalltit;
	x_BYTE_17D738[0] = 0;
	if (x_WORD_17D724 == 0xf100)
	{
		CopyAndShiftFrom17DB50_75D70(0, x_DWORD_17D720[0] - 16);
		ReadFrame_75DB0();
		DrawFrame_75E70();
	}
	else if (x_WORD_17D724 == 0xF1FA)
	{
		frameChunkIndex = 0;
		while (1)
		{
			if (frameChunkIndex >= x_WORD_17D726)
				break;
			chunkStreamPos = x_DWORD_17DB50;
			CopyAndShiftFrom17DB50_75D70((uint8_t*)&chunkHeader, sizeof(chunkHeader));
			switch (chunkHeader.type)
			{
			case 4:
				ReadIntroPalettes_76260();
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"COLOUR256 ");
				paletteChanged = true;
				break;
			case 7:
				sub_76300();//257300 - uz by mel byt vykreslen text
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"SS2 ");
				break;
			case 11:
				ReadIntroPalettes_76260();
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"COLOUR ");
				paletteChanged = true;
				break;
			case 12:
				sub_76430();
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"LC ");
				break;
			case 13:
				memset((void*)framebuffer_E12F4x, 0, height_17DB48 * width_17DB4A);
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"BLACK ");
				break;
			case 15:
				sub_76540();//257540
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"BRUN ");
				break;
			case 16:
				CopyAndShiftFrom17DB50_75D70((uint8_t*)framebuffer_E12F4x, width_17DB4A * height_17DB48);
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"COPY ");
				chunkHeader.size = width_17DB4A * height_17DB48;//added
				break;
			case 18:
				CopyAndShiftFrom17DB50_75D70(0, chunkHeader.size - 6);//added
				strcpy(&x_BYTE_17D738[strlen(x_BYTE_17D738)], (char*)"PSTAMP ");
				break;
			default:
				break;
			}
			frameChunkIndex++;
			x_DWORD_17DB50 = chunkHeader.size + chunkStreamPos;
		}
	}
	if (x_DWORD_17DB3C)
		x_DWORD_17DB3C();
	sub_75CB0();//256cb0
	if (paletteChanged)
	{
		fix_sub_9A0FC_wait_to_screen_beam(0);//27b0fc
		if (redrawTextInVideo_E12FC)
		{
			sub_41A90_VGA_Palette_install(unk_17D838x);
			sub_2EC90(getPaletteIndex_5BE80(unk_17D838x, 0x3Fu, 0x3Fu, 0x3Fu));//20fc90 -zde se prekresli texty
		}
	}

	if (DisplaySubtitles_D41C1)
	{
		pdwScreenBuffer_351628 += 0x26C0;
		sub_90478_VGA_Blit320(UINT8_MAX);
		pdwScreenBuffer_351628 -= 0x26C0;
	}
	else
	{
		sub_90478_VGA_Blit320(UINT8_MAX);
	}
}

//----- (0002EC60) --------------------------------------------------------
void sub_2EC60()//20fc60
{
	if (SubtitlesOn_D41CE)
	{
		sub_2EC30_clear_img_mem();
		DisplaySubtitles_D41C1 = 0;
		SubtitlesOn_D41CE = 0;
		x_DWORD_D41D0 = 0;
	}
}

//----- (00076300) --------------------------------------------------------
void sub_76300()//257300
{
	int32_t v0; // edi
	//int16_t result; // ax
	uint8_t* v2; // esi
	uint8_t* v3; // ebx
	uint16_t v4; // cx
	uint16_t v5; // [esp+0h] [ebp-1Ch]
	//int v6; // [esp+4h] [ebp-18h]
	int32_t i; // [esp+8h] [ebp-14h]
	int16_t v8; // [esp+Ch] [ebp-10h]
	int16_t v9; // [esp+10h] [ebp-Ch]
	int8_t v10; // [esp+14h] [ebp-8h]
	uint8_t v11; // [esp+18h] [ebp-4h]

	  /*
	  int v0; // edi
	__int16 result; // ax
	uint8_t* v2; // esi
	uint8_t* v3; // ebx
	unsigned __int16 v4; // cx
	int16_t v5; // [esp+0h] [ebp-1Ch]
	//int v6; // [esp+4h] [ebp-18h]
	int i; // [esp+8h] [ebp-14h]
	int16_t v8; // [esp+Ch] [ebp-10h]
	int16_t v9; // [esp+10h] [ebp-Ch]
	int8_t v10; // [esp+14h] [ebp-8h]
	int8_t v11; // [esp+18h] [ebp-4h]
	*/

	v0 = 0;
	CopyAndShiftFrom17DB50_75D70((uint8_t*)&v5, 2u);
	v2 = (uint8_t*)framebuffer_E12F4x;//2b22f4
	if (v5 > 0u)
	{
		do
		{
			v3 = v2;
			CopyAndShiftFrom17DB50_75D70((uint8_t*)&v9, 2u);
			if ((v9 & 0x8000) == 0)
			{
				//v6 = v9;
				for (i = 0; i < v9; i++)
				{
					CopyAndShiftFrom17DB50_75D70((uint8_t*)&v11, 1u);
					v3 += v11;
					CopyAndShiftFrom17DB50_75D70((uint8_t*)&v10, 1u);
					if (v10 >= 0)
					{
						if (v10 > 0)
						{
							CopyAndShiftFrom17DB50_75D70((uint8_t*)v3, 2 * v10);
							v3 += 2 * v10;
						}
					}
					else
					{
						CopyAndShiftFrom17DB50_75D70((uint8_t*)&v8, 2u);
						v4 = 0;
						while (abs(v10) > v4)
						{
							v3 += 2;
							v4++;
							*((int16_t*)v3 - 1) = v8;
						}
					}
				}
			}
			else if (v9 & 0x4000)
			{
				v0--;
				v2 += height_17DB48 * (abs(v9) - 1);
			}
			else
			{
				*(x_BYTE*)(v2 + height_17DB48 - 1) = v9;
			}
			//result = x_WORD_17DB48;
			v0++;
			v2 += height_17DB48;
		} while (v0 < v5);
	}
	//return result;
}
// E12F4: using guessed type int x_DWORD_E12F4;
// 17DB48: using guessed type __int16 x_WORD_17DB48;

//----- (00076430) --------------------------------------------------------
int sub_76430()
{
	uint8_t* v0; // edi
	int result; // eax
	char* v2; // ebx
	unsigned __int16 v3; // si
	int v4; // eax
	int v5; // [esp+0h] [ebp-18h]
	unsigned __int16 v6; // [esp+4h] [ebp-14h]
	unsigned __int8 v7; // [esp+8h] [ebp-10h]
	char v8; // [esp+Ch] [ebp-Ch]
	unsigned __int8 v9; // [esp+10h] [ebp-8h]
	unsigned __int8 v10; // [esp+14h] [ebp-4h]

	CopyAndShiftFrom17DB50_75D70((uint8_t*)&v6, 2u);
	v0 = height_17DB48 * v6 + (uint8_t*)framebuffer_E12F4x;
	CopyAndShiftFrom17DB50_75D70((uint8_t*)&v6, 2u);
	v5 = 0;
	result = 0;
	if (v6 > 0u)
	{
		do
		{
			v2 = (char*)v0;
			v3 = 0;
			CopyAndShiftFrom17DB50_75D70((uint8_t*)&v9, 1u);
			while (v3 < (signed int)v9)
			{
				CopyAndShiftFrom17DB50_75D70((uint8_t*)&v7, 1u);
				v2 += v7;
				CopyAndShiftFrom17DB50_75D70((uint8_t*)&v8, 1u);
				if (v8 >= 0)
				{
					if (v8 <= 0)
						goto LABEL_8;
					CopyAndShiftFrom17DB50_75D70((uint8_t*)v2, v8);
					v4 = v8;
				}
				else
				{
					CopyAndShiftFrom17DB50_75D70((uint8_t*)&v10, 1u);
					memset(v2, v10, abs(v8));
					v4 = abs(v8);
				}
				v2 += v4;
			LABEL_8:
				++v3;
			}
			++v5;
			v0 += (unsigned __int16)height_17DB48;
			result = v5;
		} while ((unsigned __int16)v5 < v6);
	}
	return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// E12F4: using guessed type int x_DWORD_E12F4;
// 17DB48: using guessed type __int16 x_WORD_17DB48;

//----- (00076540) --------------------------------------------------------
int sub_76540()//257540
{
	uint8_t* v0; // esi
	char* v1; // ebx
	unsigned __int16 v2; // di
	int result; // eax
	int v4; // [esp+0h] [ebp-Ch]
	unsigned __int8 v5; // [esp+4h] [ebp-8h]
	char v6; // [esp+8h] [ebp-4h]

	v0 = (uint8_t*)framebuffer_E12F4x;
	v4 = 0;
	while (1)
	{
		result = v4;
		if (v4 >= width_17DB4A)
			break;
		v1 = (char*)v0;
		v2 = 0;
		CopyAndShiftFrom17DB50_75D70(0, 1u);
		while (v2 < height_17DB48)
		{
			CopyAndShiftFrom17DB50_75D70((uint8_t*)&v6, 1u);
			if (v6 >= 0)
			{
				if (v6 > 0)
				{
					CopyAndShiftFrom17DB50_75D70((uint8_t*)&v5, 1u);
					memset(v1, v5, v6);
				}
			}
			else
			{
				v6 = abs(v6);
				CopyAndShiftFrom17DB50_75D70((uint8_t*)v1, v6);
			}
			v2 += v6;
			v1 += v6;
		}
		++v4;
		v0 += height_17DB48;
	}
	return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// E12F4: using guessed type int x_DWORD_E12F4;
// 17DB48: using guessed type __int16 x_WORD_17DB48;
// 17DB4A: using guessed type __int16 x_WORD_17DB4A;

std::string get_current_time_str() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%X"); // %X je lokální formát času (HH:MM:SS)
	return ss.str();
}

//----- (00075CB0) --------------------------------------------------------
void sub_75CB0()//256cb0
{
	if (x_WORD_E12FE && sub_473E0())
	{
		stopPlaybackFlag_17DB5A = 1;
	}
	else if (allowSkipVideo_17DB5C && (LastPressedKey_1806E4 || x_WORD_180746_mouse_left_button || x_WORD_180744_mouse_right_button))
	{
		stopPlaybackFlag_17DB5A = 1;
	}
	else
	{
		AnimCurrentTick += x_DWORD_E3844;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point deadline = AnimCurrentTime + std::chrono::milliseconds(static_cast<long long>(AnimCurrentTick * AnimTickTime));
		while (now < deadline)
		//while (GameTimerTurn_17DB54 < x_DWORD_E3844)
		{
			if (x_WORD_E12FE && sub_473E0())
			{
				stopPlaybackFlag_17DB5A = 1;
				return;
			}
			if (allowSkipVideo_17DB5C && (LastPressedKey_1806E4 || x_WORD_180746_mouse_left_button || x_WORD_180744_mouse_right_button))
			{
				stopPlaybackFlag_17DB5A = 1;
				return;
			}
			now = std::chrono::system_clock::now();
		}
		GameTimerTurn_17DB54 = 0;
	}
}

/*
//----- (0009A0FC) --------------------------------------------------------
void sub_9A0FC_wait_to_screen_beam()//27B0fc
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	VGA_Blit(nullptr);
	mydelay(1);
}*/

//----- (000473E0) --------------------------------------------------------
int sub_473E0()//2283e0
{
	char v0; // bl
	//__int16 v1; // ax

	v0 = 0;
	if (x_DWORD_EA3B4)
		return x_DWORD_EA3B4;
	if (x_WORD_F42A8)
		--x_WORD_F42A8;
	if (!x_WORD_18074C_mouse_left2_button && !x_WORD_18074A_mouse_right2_button)
	{
		if (pressedKeys_180664[42] || pressedKeys_180664[54])
		{
			if (LastPressedKey_1806E4 < 0x36u)
			{
				if (LastPressedKey_1806E4 != 0x2a)
					goto LABEL_24;
			}
			else if (LastPressedKey_1806E4 > 0x36u
				&& (LastPressedKey_1806E4 < 0x3Bu || LastPressedKey_1806E4 > 0x3Fu && LastPressedKey_1806E4 != 66))
			{
				goto LABEL_24;
			}
		}
		else if (LastPressedKey_1806E4 < 0x3Bu)
		{
			if (LastPressedKey_1806E4 != 19)
				goto LABEL_24;
		}
		else if (LastPressedKey_1806E4 > 0x3Bu
			&& (LastPressedKey_1806E4 < 0x3Fu || LastPressedKey_1806E4 > 0x41u))
		{
		LABEL_24:
			//HIBYTE(v1) = HIBYTE(x_WORD_F42A8);
			if (x_WORD_F42A8)
			{
				x_WORD_F42AE = LastPressedKey_1806E4;
			}
			else
			{
				//LOBYTE(v1) = LastPressedKey_1806E4;
				if (LastPressedKey_1806E4 != x_WORD_F42AE)
					v0 = 1;
			}
			goto LABEL_28;
		}
		FlvInitSet_473B0();
		goto LABEL_24;
	}
	v0 = 1;
LABEL_28:
	if (v0)
		x_DWORD_EA3B4 = 1;
	return x_DWORD_EA3B4;
}
// EA3B4: using guessed type int x_DWORD_EA3B4;
// F42A8: using guessed type __int16 x_WORD_F42A8;
// F42AE: using guessed type __int16 x_WORD_F42AE;
// 18068E: using guessed type char x_BYTE_18068E;
// 18069A: using guessed type char x_BYTE_18069A;
// 1806E4: using guessed type char x_BYTE_1806E4;
// 18074A: using guessed type __int16 x_WORD_18074A_mouse_right2_button;
// 18074C: using guessed type __int16 x_WORD_18074C_mouse_left2_button;

//----- (00076260) --------------------------------------------------------
void ReadIntroPalettes_76260()
{
	uint16_t countPalette;
	int32_t count;
	uint8 offset;
	TColor* palette = unk_17D838x;
	int actPalette = 0;
	CopyAndShiftFrom17DB50_75D70((uint8_t*)&countPalette, 2u);
	if (countPalette > 0u)
	{
		do
		{
			CopyAndShiftFrom17DB50_75D70((uint8_t*)&offset, 1u);
			palette += offset;
			count = 0;
			CopyAndShiftFrom17DB50_75D70((uint8_t*)&count, 1u);
			if (!count)
				count = 256;
			for (int i = 0; i < count; i++)
			{
				CopyAndShiftFrom17DB50_75D70((uint8_t*)palette, 3u);
				palette++;
			}
			actPalette++;
		} while (actPalette < countPalette);
	}
}


