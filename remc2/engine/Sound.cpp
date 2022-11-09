#include "Sound.h"

//fix str2_E37A4_sound_buffer3 = str_E37A0_sound_buffer2->next_str + v8x[a2].dword_8;
//fix x_DWORD_E380C = str_E3808_music_header->next_str + headerx[drivernumber].dword_8;
//replace str2_E37A4_sound_buffer3 and x_DWORD_E380C with equvivalent index_E37A4_MaxSound and index_E380C_MaxMusic
//tabbuffer rewrite tostruct
//remove index functions


int x_DWORD_E3794_sound_buffer3_lenght = 10; // weak
bool soundAble_E3798 = true; // weak
bool soundActive_E3799 = true; // weak
bool soundLoaded_E379A = true; // weak
bool autoScanForSoundHardware_E379B = true; // weak
char x_BYTE_E379C = 1; // weak

type_E37A0_sound_buffer2* soundIndex_E37A0 = 0;
//uint8_t* x_DWORD_E37A0_sound_buffer2 = 0; // weak
//replace str2_E37A4_sound_buffer3 and x_DWORD_E380C with equvivalent index_E37A4_MaxSound and index_E380C_MaxMusic
int numOfLoadedSounds_E37A4 = 0;
//int8_t* str2_E37A4_sound_buffer3 = 0; // weak
uint8_t* soundBuffer1_E37A8 = nullptr; // weak
uint8_t defaultSoundIndex_E37AC = 0;
int8_t actualSound_E37AD = -1; // weak
char x_BYTE_E37AE = 0; // weak
int defaultVolume_E37B0 = 127; // weak
__int16 soundFreqType2_E37B4 = 1644; // weak
__int16 soundFreqType_E37B6 = 0; // weak
char soundInitialized_E37B8 = 0; // weak
int soundFrequence_E37BC = 0; // weak
int musicVolume_E37F8 = 127; // weak
bool musicAble_E37FC = true; // weak//2b47fc
bool musicActive_E37FD = true; // weak
bool musicLoaded_E37FE = false; // weak
__int16 songCurrentlyPlaying_E3802 = 0; // weak //2b4802
__int16 m_iNumberOfTracks = 0;
type_E3808_music_header* musicHeader_E3808 = nullptr; // weak
//uint8_t* x_DWORD_E3808_music_header = 0; // weak
//int8_t* x_DWORD_E380C = 0; // weak
int countOfMusicTracks_E380C = 0; // weak
//replace str2_E37A4_sound_buffer3 and x_DWORD_E380C with equvivalent index_E37A4_MaxSound and index_E380C_MaxMusic
uint8_t* musicData_E3810 = nullptr; // weak
uint8_t musicChannel_E3814 = 0; // weak
char x_BYTE_E3815 = 0; // weak
char x_BYTE_E3816 = 0; // weak
char x_BYTE_E3817 = 1; // weak
char x_BYTE_E3818 = 0; // weak
char pcSpeakerSoundDev_E3819 = 0; // weak
char x_BYTE_E381A = -1; // weak
__int16 x_WORD_E381C = 0; // weak
__int16 x_WORD_E381E = 0; // weak
__int16 x_WORD_E3820 = 0; // weak
__int16 x_WORD_E3822 = 0; // weak
int x_DWORD_E3824 = 0; // weak
__int16 x_WORD_E3828 = 0; // weak
__int16 x_WORD_E3834 = 0; // weak
int x_DWORD_E383C = 0; // weak
FILE* x_FILE_E3840 = 0; // weak
int x_DWORD_E3844 = 5; // weak
//_UNKNOWN unk_E3848; // weak
__int16 x_WORD_E387A = 0; // weak
int x_DWORD_E387C = 0; // weak
int x_DWORD_E3880 = 0; // weak
int x_DWORD_E3884 = 0; // weak
int x_DWORD_E3888 = 0; // weak
bool TimerRun_E388D = false; // weak
//int x_DWORD_E3890 = 0; // weak

int preference_181DAC[19]; // fix it -  weak

char x_DWORD_181C14[256]; // weak

uint8_t x_BYTE_181C2C; // weak

int8_t soundActiveL_E2A14 = 1; // weak
int8_t soundActiveH_E2A14 = 0; // weak

int soundBufferLen_E2A18 = 0; // weak
//int x_DWORD_E2A1C = 0; // weak

HDIGDRIVER hDigSoundEffectsDriver_180B48; // weak
//uint8_t* x_DWORD_180B4C_end_sound_buffer3; // weak
uint32_t x_DWORD_180B4C_end_sound_buffer3_endindex;
__int16 indexLoadedSound_180B50; // weak
AIL_INI musicAILSettings; // weak
char textBuffer_180BE0[512]; // weak
HSEQUENCE m_hSequence; // weak
HMDIDRIVER hMdiMusicDriver; // weak
//int x_DWORD_180C80; // weak
char musicDriverType_180C84; // weak

HSAMPLE SoundBuffer3_180750[50]; // weak -351750 23 // delka 3fc, tj 2c struktur o delce 17 end x_DWORD_180B4C_end_sound_buffer3
AIL_INI soundEffectsAILSettings; // weak

__int16 midiPort_180B30; // weak

IO_PARMS ioParms_181D90; // weak

__int16 x_WORD_E39A4 = 0; // weak
__int16 x_WORD_E39A6 = 0; // weak
__int16 x_WORD_E39A8 = 0; // weak
__int16 x_WORD_E39AA = 0; // weak
__int16 x_WORD_E39AC = 0; // weak
__int16 x_WORD_E39AE = 0; // weak
__int16 x_WORD_E39B0 = 0; // weak
__int16 x_WORD_E39B2 = 0; // weak
__int16 x_WORD_E39B4 = 0; // weak
__int16 x_WORD_E39B6 = 0; // weak
char x_BYTE_E39B8 = 0; // weak

int ailIndent_181C04 = 0; // weak 352c04
struct tm* x_DWORD_181C08; // weak
int x_DWORD_181C0C; // weak
int x_DWORD_181C10; // weak

uint32_t unk_181B42 = 0; // weak
int16_t x_WORD_181B44 = 0; // weak
int x_DWORD_181BE0; // weak
FILE* ailDebufFile_181BF0; // weak
int x_DWORD_181BF4 = 0; // weak//352bf4
int x_DWORD_181BF8; // weak
int x_DWORD_181BFC; // weak

__int16 x_WORD_E3B4C = 0; // weak
char x_BYTE_E3B50 = 0; // weak
char x_BYTE_E3B51 = 0; // weak

AIL_DRIVER* ailDrivers_181C50[16]; // idb
char textBuffer_181C90[513]; // weak

int x_DWORD_E3E38 = 0; // weak
int x_DWORD_181DA8; // weak

IO_PARMS ioParms_181DF8; // weak
HSAMPLE mainSample_181E10; // weak

int x_DWORD_E3E40 = 0; // weak
bool useSampleData_E3E44 = false; // weak
bool sampleFinished_E3E45 = true; // weak

int x_DWORD_181E26; // weak
__int16 x_WORD_181E2A; // weak
int x_DWORD_181E2C; // weak
__int16 x_WORD_181E30; // weak
int unk_181E80x[3]; // weak
int x_DWORD_181E8C; // weak
int x_DWORD_181E90; // weak
int x_DWORD_181E94; // weak
int x_DWORD_181E98; // weak
int x_DWORD_181E9C; // weak
int x_DWORD_181EA0; // weak
int x_DWORD_181EB0; // weak
int x_DWORD_181EB4; // weak
HSEQUENCE x_DWORD_181EB8; // weak
int x_DWORD_181EBC; // weak
int x_DWORD_181EC0; // weak
int x_DWORD_181EC4; // weak
int x_DWORD_181EC8; // weak
int x_DWORD_181ECC; // weak
int x_DWORD_181ED0; // weak
int x_DWORD_181ED4; // weak
int x_DWORD_181ED8; // weak
int x_DWORD_181EDC; // weak
uint8_t unk_181EE0x[512]; // weak
uint8_t* x_DWORD_182188[6]; // idb
uint8_t* x_DWORD_1821A0[6]; // idb
uint8_t* x_DWORD_1821B8[6]; // idb
uint8_t* x_DWORD_1821D0[6]; // idb
uint8_t* x_DWORD_1821E8[6]; // idb

int x_DWORD_E4E1C = 0; // weak

type_str_32_16 str_E3E54 = {0, 0}; // weak

int x_DWORD_E3FE2 = 0; // weak
int x_DWORD_E3FE6 = 0; // weak
int x_DWORD_E3FEA = 0; // weak
int x_DWORD_E3FEE = 0; // weak
int x_DWORD_E3FF2 = 0; // weak

int x_DWORD_E3E9C[16]; // weak//sound anything
//int x_DWORD_E3ED8 = 0; // weak//x_DWORD_E3E9C[0xf]
int x_DWORD_E3EDC[16]; // weak
int x_DWORD_E3F1C[16]; // weak
int x_DWORD_E3F5C[16]; // weak

int isr_E3FF8 = 0; // weak
int x_DWORD_E3FFC = 0; // weak

int x_DWORD_E3FDC = 0; // weak

int x_DWORD_A1270 = 1397753374; // weak
uint16_t x_WORD_A1277 = 39321; // weak
uint16_t x_WORD_A1286 = 39321; // weak
int x_DWORD_A128C = 2576980377; // weak
uint16_t x_WORD_A12A2 = 39321; // weak
uint16_t x_WORD_A12AC = 39321; // weak
uint16_t x_WORD_A12B3 = 39321; // weak

__int16 x_WORD_E3B4E = 0; // weak

uint32_t unk_E4004; // weak
__int16 x_WORD_E4A04 = 0; // weak
int x_DWORD_E4A08 = 0; // weak
uint8_t x_BYTE_E4A0C[128] =//table of sound
{
  0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,
  0x10,0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
  0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E,
  0x30,0x32,0x34,0x36,0x38,0x3A,0x3C,0x3E,
  0x40,0x42,0x44,0x46,0x48,0x4A,0x4C,0x4E,
  0x50,0x52,0x54,0x56,0x58,0x5A,0x5C,0x5E,
  0x60,0x62,0x64,0x66,0x68,0x6A,0x6C,0x6E,
  0x70,0x72,0x74,0x76,0x78,0x7A,0x7C,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80
}; // idb
int x_DWORD_E4A8C = 0; // weak
std::string aSamplesPath{ "SAMPLE" }; // weak
int x_DWORD_E4B18 = 0; // weak
char aTimb_0[5] = "TIMB"; // weak
char x_BYTE_E4B2A = -1; // weak
char x_BYTE_E4B2B = -1; // weak
int x_DWORD_E4C90 = 0; // weak
int x_DWORD_E4C94 = 0; // weak


int x_DWORD_A9390[128] = {
0x00000008,0x00000011,0x00000012,0x00000013,
0x00000014,0x00000015,0x00000017,0x00000018,
0x0000001A,0x0000001B,0x0000001D,0x0000001E,
0x00000020,0x00000022,0x00000024,0x00000026,
0x00000029,0x0000002B,0x0000002E,0x00000031,
0x00000033,0x00000037,0x0000003A,0x0000003D,
0x00000041,0x00000045,0x00000049,0x0000004D,
0x00000052,0x00000057,0x0000005C,0x00000062,
0x00000067,0x0000006E,0x00000074,0x0000007B,
0x00000082,0x0000008A,0x00000092,0x0000009B,
0x000000A4,0x000000AE,0x000000B9,0x000000C4,
0x000000CF,0x000000DC,0x000000E9,0x000000F7,
0x00000105,0x00000115,0x00000125,0x00000137,
0x00000149,0x0000015D,0x00000172,0x00000188,
0x0000019F,0x000001B8,0x000001D2,0x000001ED,
0x0000020B,0x0000022A,0x0000024B,0x0000026E,
0x00000293,0x000002BA,0x000002E4,0x00000310,
0x0000033E,0x00000370,0x000003A4,0x000003DB,
0x00000416,0x00000454,0x00000496,0x000004DC,
0x00000526,0x00000574,0x000005C8,0x00000620,
0x0000067D,0x000006E0,0x00000748,0x000007B7,
0x0000082D,0x000008A9,0x0000092D,0x000009B8,
0x00000A4C,0x00000AE9,0x00000B90,0x00000C40,
0x00000CFA,0x00000DC0,0x00000E91,0x00000F6F,
0x0000105A,0x00001153,0x0000125B,0x00001371,
0x00001499,0x000015D3,0x00001720,0x00001880,
0x000019F4,0x00001B80,0x00001D23,0x00001EDE,
0x000020B4,0x000022A6,0x000024B6,0x000026E3,
0x00002933,0x00002BA6,0x00002E40,0x00003100,
0x000033E9,0x00003700,0x00003A46,0x00003DBC,
0x00004169,0x0000454C,0x0000496C,0x00004DC6,
0x00005266,0x0000574C,0x00005C80,0x00006200 }; // weak

char x_BYTE_D3450[17] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',0 }; // weak

const char* envs_E4CC4[7] = { "PATH=Z:\\",
"COMSPEC=Z:\\COMMAND.COM",
"PROMPT=$P$G",
"BLASTER=A220 I7 D1 H5 T6",
"AIL_DEBUG=sound.dbg",//debug
"AIL_SYS_DEBUG=soundd.dbg",//debug
NULL };

char IsTable[256] =
{
  0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x01,
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  0x01,0x0A,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,
  0x0C,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x0C,0x0C,0x0C,0x0C,0x0C,
  0x0C,0x0C,0x58,0x58,0x58,0x58,0x58,0x58,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,
  0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x0C,0x0C,0x0C,0x0C,
  0x0C,0x0C,0x98,0x98,0x98,0x98,0x98,0x98,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,
  0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x0C,0x0C,0x0C,0x0C,
  0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}; // weak

int sub_AEF40() { stub_fix_it(); return 0; };
int sub_AEF51() { stub_fix_it(); return 0; };

int(*off_AEB40[2])() = { &sub_AEF40, &sub_AEF51 }; // weak

int32_t AilCallDriver_91F70(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* in, VDI_CALL* out);//AIL_call_driver
void sub_91F50(unsigned int a1);
void sub_9E250(HMDIDRIVER mdidrv) {
	if (!x_DWORD_E3E38)
	{
		x_DWORD_E3E38 = 1;
		x_DWORD_181DA8 = sub_91F20();
		AilCallDriver_91F70(mdidrv->drvr_0, 770, nullptr, nullptr);
		sub_91F50(x_DWORD_181DA8);
		x_DWORD_E3E38 = 0;
	}
}; // weak
int sub_A2DE0() { stub_fix_it(); return 0; }; // weak
int sub_A47A0() { stub_fix_it(); return 0; }; // weak
int sub_A4920() { stub_fix_it(); return 0; }; // weak
int sub_A9C00() { stub_fix_it(); return 0; }; // weak
int sub_A9C50() { stub_fix_it(); return 0; }; // weak
void sub_A6F30(void*  /*a*/) { stub_fix_it(); }; // weak

//----- (0008D290) --------------------------------------------------------
void InitSound_8D290()//26e290
{
	IO_PARMS ioParms = {}; // [esp+4h] [ebp-3Ch]
	const char* mdSound;
	bool soundCardOk;

	char mdSoundPar1[16]; // [esp+1Ch] [ebp-24h] BYREF

	char soundGamePath[MAX_PATH];

	sprintf(soundGamePath, "%s/SOUND", gameDataPath.c_str());

	soundCardOk = false;
	if (!soundAble_E3798)
		return;
	soundFreqType_E37B6 = soundFreqType2_E37B4;
	if (!soundFreqType2_E37B4)
	{
		if (!musicAble_E37FC)
		{
			if (soundInitialized_E37B8)
				AilShutdown_919C0();
		}
		soundAble_E3798 = false;
		soundActive_E3799 = false;
		myprintf("\nCould not load any digital samples\n");
		return;
	}
	if (!soundInitialized_E37B8)
	{
		//removed
		//sub_90F60();//Get act path
		//sub_9D3F0_set_malloc_type(sub_83CD0_malloc2);//Set malloc Function
		//v3 = sub_9D420(( (*)(void*))sub_83E80_freemem4);
		//((void (*)(int (*)(int)))loc_917D0)(v3);
		//test removing sub_917D0(/*v3*/);
		//removed

		//fixed
		preference_181DAC[0] = 0xc8;
		preference_181DAC[1] = 0x01;//DB0
		preference_181DAC[2] = 0x800;//DB4
		preference_181DAC[3] = 0x64;//DB8
		preference_181DAC[4] = 0x10;//DBC
		preference_181DAC[5] = 0x64;//DC0
		preference_181DAC[6] = 0x28f;//DC4
		preference_181DAC[7] = 0x00;//DC8
		preference_181DAC[8] = 0x00;//DCC
		preference_181DAC[9] = 0x01;//DD0
		preference_181DAC[10] = 0x00;//DD4
		preference_181DAC[11] = 0x78;//DD8
		preference_181DAC[12] = 0x08;//DDC
		preference_181DAC[13] = 0x7f;//DE0
		preference_181DAC[14] = 0x01;//DE4
		preference_181DAC[15] = 0x00;//DE8
		preference_181DAC[16] = 0x02;//DEC
		preference_181DAC[17] = 0x01;//DF0
		preference_181DAC[18] = 0x01;//DF4
		//fixed

		soundInitialized_E37B8 = 1;
	}

	if (!DirExists(soundGamePath))
	{
		soundAble_E3798 = false;
		soundActive_E3799 = false;
		if (musicAble_E37FC && musicLoaded_E37FE)
		{
			sub_99C90();
		}
		else
		{
			musicAble_E37FC = false;
			musicActive_E37FD = false;
			AilShutdown_919C0();
		}
		return;
	}
	mdSound = mygetenv("MDSOUND");
	if (mdSound)
	{
		sscanf((char* const)mdSound, "%s %x %d %d %d", mdSoundPar1, &ioParms.IO, &ioParms.IRQ, &ioParms.DMA_8_bit, &ioParms.DMA_16_bit);
		hDigSoundEffectsDriver_180B48 = AilInstallDigDriverFile_93330(mdSoundPar1, &ioParms);

		if (!hDigSoundEffectsDriver_180B48)
		{
			if (!musicAble_E37FC)
				AilShutdown_919C0();
			soundAble_E3798 = false;
			soundActive_E3799 = false;
			return;
		}
		sprintf(soundEffectsAILSettings.driver_name, "%s", mdSoundPar1);
		midiPort_180B30 = ioParms.IO;//fixed
		AilUninstallDigDriver_93480(hDigSoundEffectsDriver_180B48);
		soundCardOk = true;
	}
	else
	{
		char digPath[MAX_PATH];
		sprintf(digPath, "%s/DIG.INI", soundGamePath);
		if (AilReadIniFile_92190(&soundEffectsAILSettings, digPath)) // sound driver
		{
			if (!_stricmp(soundEffectsAILSettings.driver_name, "None"))//351ab0
			{
				if (!musicAble_E37FC)
					AilShutdown_919C0();
				soundAble_E3798 = false;
				soundActive_E3799 = false;
				return;
			}
			if (!AilInstallDigIni_931F0(&hDigSoundEffectsDriver_180B48, digPath))//351b48
			{
				AilUninstallDigDriver_93480(hDigSoundEffectsDriver_180B48);
				soundCardOk = true;
			}
			
			//fix hqsound
			for (int i = 0; i < hDigSoundEffectsDriver_180B48->n_samples_24; i++)
				hDigSoundEffectsDriver_180B48->samples_23[i].start_44mhz = nullptr;
			//fix hqsound
		}
		if (!soundCardOk)
		{
			if (!musicAble_E37FC)
				AilShutdown_919C0();
			soundAble_E3798 = false;
			soundActive_E3799 = false;
			return;
		}
	}

	//sub_90F20 //set directory
	sub_8D800_sound_proc2();
	if (soundFreqType_E37B6)
	{
		switch (soundFreqType_E37B6) {
			case 0:
				soundAble_E3798 = false;
				soundActive_E3799 = false;
				myprintf("\nCould not load any digital samples\n");
				return;
			case 800:
				soundFrequence_E37BC = 11025;//800
				x_BYTE_E37AE = 0;
				break;
			case 811:
				soundFrequence_E37BC = 11025;//811
				x_BYTE_E37AE = 0;
				break;
			case 822:
				soundFrequence_E37BC = 22050;//822
				x_BYTE_E37AE = 0;
				break;
			case 1611:
				soundFrequence_E37BC = 11025;//1611
				x_BYTE_E37AE = 1;
				break;
			case 1622:
				soundFrequence_E37BC = 22050;//1622
				x_BYTE_E37AE = 1;
				break;
			case 1644:
				soundFrequence_E37BC = 44100;//1644
				x_BYTE_E37AE = 1;
				break;
		}
			
		AilSetPreference_91A80(0, 200);
		AilSetPreference_91A80(1, soundFrequence_E37BC);
		AilSetPreference_91A80(3, 100);
		AilSetPreference_91A80(4, x_DWORD_E3794_sound_buffer3_lenght + 1);
		AilSetPreference_91A80(5, 127);
		AilSetPreference_91A80(6, 655);
		if (x_BYTE_E379C)
			AilSetPreference_91A80(7, 1);
		else
			AilSetPreference_91A80(7, 0);
		if (!autoScanForSoundHardware_E379B)
			AilSetPreference_91A80(17, 0);
		if (x_BYTE_E37AE)
		{
			AilSetPreference_91A80(8, 1);
			AilSetPreference_91A80(9, 1);
		}
		else
		{
			AilSetPreference_91A80(8, 0);
			AilSetPreference_91A80(9, 0);
		}

		hDigSoundEffectsDriver_180B48 = AilInstallDigDriverFile_93330(soundEffectsAILSettings.driver_path, &ioParms_181D90);
		if (hDigSoundEffectsDriver_180B48)
		{
			if (soundAble_E3798)
			{
				x_DWORD_180B4C_end_sound_buffer3_endindex = x_DWORD_E3794_sound_buffer3_lenght;
				for (int i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
				{
					SoundBuffer3_180750[i] = AilAllocateSampleHandle_93510(hDigSoundEffectsDriver_180B48);
					SoundBuffer3_180750[i]->len_4_5[1] = 0;
					SoundBuffer3_180750[i]->start_44mhz = nullptr;
				}
				soundLoaded_E379A = true;
				sub_8E470_sound_proc17_volume(defaultVolume_E37B0);
			}
		}
		else
		{
			if (!musicAble_E37FC)
				AilShutdown_919C0();
			soundAble_E3798 = false;
			soundActive_E3799 = false;
			FreeMem_83E80((uint8_t*)soundIndex_E37A0);
			numOfLoadedSounds_E37A4 = 0;
			FreeMem_83E80(soundBuffer1_E37A8);
		}
	}
	else
	{
		if (!musicAble_E37FC)
			AilShutdown_919C0();
		soundAble_E3798 = false;
		soundActive_E3799 = false;
		myprintf("\nCould not load any digital samples\n");
	}
}

//----- (0008D800) --------------------------------------------------------
void sub_8D800_sound_proc2()//26E800
{
	bool runAgain = true;

	while (runAgain)
	{
		runAgain = LoadSound_84300(defaultSoundIndex_E37AC);
		if (runAgain)
		{
			switch (soundFreqType_E37B6)
			{
				case 800:
					soundFreqType_E37B6 = 0;//800
					return;
				case 811:
					soundFreqType_E37B6 = 800;//811
					break;
				case 822:
					soundFreqType_E37B6 = 1611;//822
					break;
				case 1611:
					soundFreqType_E37B6 = 811;//1611
					break;
				case 1622:
					soundFreqType_E37B6 = 822;//1622
					break;
				case 1644:
					soundFreqType_E37B6 = 1622;//1644
					break;
			}
		}
	}
}

//----- (0008D8F0) --------------------------------------------------------
void EndSample_8D8F0()//26e8f0
{
	if (soundAble_E3798 && soundActive_E3799)
	{
		StopTimer_8F850();//270850
		for (int i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
		{
			AilEndSample_93D00(SoundBuffer3_180750[i]);//274d00
			SoundBuffer3_180750[i]->vol_scale_18[0][0] = 0;
			SoundBuffer3_180750[i]->flags_14 = 0;
			SoundBuffer3_180750[i]->vol_scale_18[0][2] = 0;
			SoundBuffer3_180750[i]->vol_scale_18[0][3] = 0;
		}
	}
	sub_9F040();
}

//----- (0008D970) --------------------------------------------------------
void InitMusic_8D970()//26e970
{
	IO_PARMS ioParms = {}; // [esp+90h] [ebp-38h]
	char soundCdPath[MAX_PATH];
	char soundGamePath[MAX_PATH];

	char mdMusicPar1[16];

	sprintf(soundCdPath, "%s/SOUND", cdDataPath.c_str());
	sprintf(soundGamePath, "%s/SOUND", gameDataPath.c_str());

	const char* mdMusic = nullptr;
	bool musicCardOk = false;
	if (!musicAble_E37FC)
		return;
	if (!soundInitialized_E37B8)
	{
		//removed
		//sub_9D3F0_set_malloc_type(sub_83CD0_malloc2); fix it
		//v3 = sub_9D420((int (*)(int))sub_83E80_freemem4);
		//((void (/*__fastcall*/ *)(int (*)(int)))loc_917D0)(v3);
		//removed
		soundInitialized_E37B8 = 1;
	}
	AilSetPreference_91A80(11, 120);
	AilSetPreference_91A80(12, 1);
	AilSetPreference_91A80(13, 127);
	AilSetPreference_91A80(14, 1);
	AilSetPreference_91A80(15, 0);
	AilSetPreference_91A80(16, 12);
	if (!autoScanForSoundHardware_E379B)
		AilSetPreference_91A80(17, 0);

	char samplesPath[MAX_PATH];
	sprintf(samplesPath, "%s/SAMPLE", soundCdPath);
	AilSetGtlFilenamePrefix_96AE0(samplesPath);
	if (!DirExists(soundCdPath))
	{
		musicAble_E37FC = false;
		musicActive_E37FD = false;
		if (soundAble_E3798 && soundLoaded_E379A)
		{
			EndSounds_99C10();
		}
		else
		{
			soundAble_E3798 = false;
			soundActive_E3799 = false;
			AilShutdown_919C0();
		}
		return;
	}
	mdMusic = mygetenv("MDMUSIC");
	if (mdMusic)
	{
		sscanf((char* const)mdMusic, "%s %x %d %d %d", mdMusicPar1, &ioParms.IO, &ioParms.IRQ, &ioParms.DMA_8_bit, &ioParms.DMA_16_bit);
		hMdiMusicDriver = AilInstakkMidiDriverFile_95850(mdMusicPar1, &ioParms);
		if (!hMdiMusicDriver)
		{
			if (!soundAble_E3798)
				AilShutdown_919C0();
			musicAble_E37FC = false;
			musicActive_E37FD = false;
			return;
		}
		sprintf(textBuffer_180BE0, "%s", mdMusicPar1);
		musicCardOk = true;
	}
	else
	{
		char musicMdiPath[MAX_PATH];
		sprintf(musicMdiPath, "%s/MDI.INI", soundGamePath);
		if (AilReadIniFile_92190(&musicAILSettings, musicMdiPath)) //driver
		{
			if (!_stricmp(musicAILSettings.driver_name, "None"))
			{
				if (!soundAble_E3798)
					AilShutdown_919C0();
				musicAble_E37FC = false;
				musicActive_E37FD = false;
				return;
			}
			if (!AilInstallMidiIni_95710(&hMdiMusicDriver, musicMdiPath))
				musicCardOk = true;
		}
		if (!musicCardOk)
		{
			if (!soundAble_E3798)
				AilShutdown_919C0();
			musicAble_E37FC = false;
			musicActive_E37FD = false;
			return;
		}
	}

	m_hSequence = AilAllocateSequenceHandle_95A30(hMdiMusicDriver);//driver
	bool isDriver = false;
	if (!_stricmp(musicAILSettings.driver_name, "ADLIB.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;		
	}
	if (!_stricmp(musicAILSettings.driver_name, "ADLIBG.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "OPL3.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "PAS.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "PASPLUS.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBLASTER.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBPRO1.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBPRO2.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "ESFM.MDI"))
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBAWE32.MDI"))
	{
		musicDriverType_180C84 = 'w';
		InitAWE32_9F740((char*)"Bullfrog");
		if (!x_BYTE_E3815)
			musicDriverType_180C84 = 'g';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "MPU401.MDI"))
	{
		musicDriverType_180C84 = 'g';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SNDSCAPE.MDI"))
	{
		musicDriverType_180C84 = 'g';
		isDriver = true;
	}
	if (!_stricmp(musicAILSettings.driver_name, "MT32MPU.MDI"))
	{
		musicDriverType_180C84 = 'r';
		isDriver = true;
	}
	if (!sub_969A0_AIL_MDI_driver_type(hMdiMusicDriver))
	{
		musicDriverType_180C84 = 'g';
		isDriver = true;
	}
	if (sub_969A0_AIL_MDI_driver_type(hMdiMusicDriver) == 1 || sub_969A0_AIL_MDI_driver_type(hMdiMusicDriver) == 2)
	{
		musicDriverType_180C84 = 'f';
		isDriver = true;
	}
	if (isDriver)
	{
		if (musicAble_E37FC && InitMusicBank_8EAD0(musicChannel_E3814))
		{
			if (x_BYTE_E3815 == 1)
			{
				sub_9FA80();
				x_BYTE_E3815 = 0;
			}
			AilUninstallMidiDriver_959A0(hMdiMusicDriver);
			if (!soundAble_E3798)
				AilShutdown_919C0();
			musicAble_E37FC = false;
			musicActive_E37FD = false;
			myprintf("\nError opening music files\n");
		}
		else
		{
			musicLoaded_E37FE = true;
			sub_8E410_sound_proc16_xmidivolume(musicVolume_E37F8);
		}
		return;
	}
	AilUninstallMidiDriver_959A0(hMdiMusicDriver);
	if (!soundAble_E3798)
		AilShutdown_919C0();
	musicAble_E37FC = false;
	musicActive_E37FD = false;
	myprintf("\nError driver not supported\n");
}

//----- (0008E020) --------------------------------------------------------
void StopMusic_8E020()//26f020
{
	if (musicAble_E37FC && musicActive_E37FD && songCurrentlyPlaying_E3802)
	{
		if (x_BYTE_E3818)
			;// sub_92DC0_AIL_release_timer_handle(x_DWORD_180C80);
		pcSpeakerSoundDev_E3819 = 0;
		x_BYTE_E3818 = 0;
		x_BYTE_E3817 = 1;
		x_BYTE_E3816 = 0;
		x_BYTE_E381A = -1;
		if (AilSequenceStatus_96170(m_hSequence) != 2)
		{
			AilStopSequence_95DE0(m_hSequence);
			AilEndSequence_95F00(m_hSequence);
		}
		songCurrentlyPlaying_E3802 = 0;
	}
}

//----- (0008E160) --------------------------------------------------------
void StartMusic_8E160(int track, int volume)//26f160
{
	if (musicAble_E37FC && musicActive_E37FD && track <= m_iNumberOfTracks && songCurrentlyPlaying_E3802 != track)
	{
		if (x_BYTE_E3818)
			;// sub_92DC0_AIL_release_timer_handle(x_DWORD_180C80);
		pcSpeakerSoundDev_E3819 = 0;
		x_BYTE_E3818 = 0;
		x_BYTE_E3817 = 1;
		x_BYTE_E3816 = 0;
		x_BYTE_E381A = -1;
		if (songCurrentlyPlaying_E3802)
		{
			if (AilSequenceStatus_96170(m_hSequence) != 2)
			{
				AilStopSequence_95DE0(m_hSequence);
				AilEndSequence_95F00(m_hSequence);
			}
			songCurrentlyPlaying_E3802 = 0;
		}
		AilInitSequence_95C00(m_hSequence, musicHeader_E3808->str_8.track_10[track].xmiData_0, 0, track);
		AilRegisterTriggerCallback_97670(m_hSequence, reinterpret_cast<void*>(sub_8E0D0));

		if (volume < 127)
			AilSetSequenceVolume_96030(volume, -1);

		AilStartSequence_95D50(m_hSequence, track);
		songCurrentlyPlaying_E3802 = track;
	}
}

//----- (0008E410) --------------------------------------------------------
void sub_8E410_sound_proc16_xmidivolume(int master_volume)//26f410
{
	if (musicAble_E37FC)
	{
		if (musicLoaded_E37FE)
		{
			if (master_volume != musicVolume_E37F8 && master_volume <= 127 && master_volume >= 0)
			{
				AilSetXMidiMasterVolume_96670(master_volume);
				musicVolume_E37F8 = master_volume;
			}
		}
	}
}

//----- (0008E470) --------------------------------------------------------
void sub_8E470_sound_proc17_volume(int volume)//26f470
{
	if (soundAble_E3798)
	{
		if (soundLoaded_E379A)//2b479a
		{
			if (volume != defaultVolume_E37B0 && volume <= 127 && volume >= 0)
			{
				AilDigitalMasterVolume_94650(volume);
				defaultVolume_E37B0 = volume;
			}
		}
	}
}

//----- (00090FD0) --------------------------------------------------------
void InitSoundAndMusic_90FD0()//271fd0
{
	InitSound_8D290();
	InitMusic_8D970();
	if (!_stricmp(soundEffectsAILSettings.driver_name, "SB16.DIG"))
		InitAWE32orMPU401_91010();
}

//----- (00091010) --------------------------------------------------------
void InitAWE32orMPU401_91010()
{
	if (midiPort_180B30 == 544)
	{
		x_outp(midiPort_180B30 + 4, 48);
		x_WORD_E39A4 = x_inp(midiPort_180B30 + 5) >> 3;
		x_outp(midiPort_180B30 + 4, 49);
		x_WORD_E39A6 = x_inp(midiPort_180B30 + 5) >> 3;
		x_outp(midiPort_180B30 + 4, 50);
		x_WORD_E39AC = x_inp(midiPort_180B30 + 5);
		x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A4 - 32);
		x_outp(midiPort_180B30 + 4, 51);
		x_WORD_E39AE = x_inp(midiPort_180B30 + 5);
		x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A6 - 32);
		x_outp(midiPort_180B30 + 4, 68);
		x_WORD_E39B0 = x_inp(midiPort_180B30 + 5);
		x_outp(midiPort_180B30 + 5, 96);
		x_outp(midiPort_180B30 + 4, 69);
		x_WORD_E39B2 = x_inp(midiPort_180B30 + 5);
		x_outp(midiPort_180B30 + 5, 96);
		x_outp(midiPort_180B30 + 4, 70);
		x_WORD_E39B4 = x_inp(midiPort_180B30 + 5);
		x_outp(midiPort_180B30 + 5, 120);
		x_outp(midiPort_180B30 + 4, 71);
		x_WORD_E39B6 = x_inp(midiPort_180B30 + 5);
		x_outp(midiPort_180B30 + 5, 120);
		if (!_stricmp(textBuffer_180BE0, "SBAWE32.MDI"))
		{
			x_outp(midiPort_180B30 + 4, 52);
			x_WORD_E39A8 = x_inp(midiPort_180B30 + 5);
			x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A4 - 24);
			x_outp(midiPort_180B30 + 4, 53);
			x_WORD_E39AA = x_inp(midiPort_180B30 + 5);
			x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A6 - 24);
		}
		else if (!_stricmp(textBuffer_180BE0, "MPU401.MDI"))
		{
			x_outp(midiPort_180B30 + 4, 52);
			x_WORD_E39A8 = x_inp(midiPort_180B30 + 5);
			x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A4 - 24);
			x_outp(midiPort_180B30 + 4, 53);
			x_WORD_E39AA = x_inp(midiPort_180B30 + 5);
			x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A6 - 24);
		}
		else
		{
			x_outp(midiPort_180B30 + 4, 52);
			x_WORD_E39A8 = x_inp(midiPort_180B30 + 5);
			x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A4 - 8);
			x_outp(midiPort_180B30 + 4, 53);
			x_WORD_E39AA = x_inp(midiPort_180B30 + 5);
			x_outp(midiPort_180B30 + 5, 8 * x_WORD_E39A6 - 8);
		}
		x_BYTE_E39B8 = 1;
	}
}

//----- (00091420) --------------------------------------------------------
void sub_91420()
{
	x_outp(midiPort_180B30 + 4, 50);
	x_outp(549, x_WORD_E39AC);
	x_outp(midiPort_180B30 + 4, 51);
	x_outp(midiPort_180B30 + 5, x_WORD_E39AE);
	x_outp(midiPort_180B30 + 4, 52);
	x_outp(midiPort_180B30 + 5, x_WORD_E39A8);
	x_outp(midiPort_180B30 + 4, 53);
	x_outp(midiPort_180B30 + 5, x_WORD_E39AA);
	x_outp(midiPort_180B30 + 4, 68);
	x_outp(midiPort_180B30 + 5, x_WORD_E39B0);
	x_outp(midiPort_180B30 + 4, 69);
	x_outp(midiPort_180B30 + 5, x_WORD_E39B2);
	x_outp(midiPort_180B30 + 4, 70);
	x_outp(midiPort_180B30 + 5, x_WORD_E39B4);
	x_outp(midiPort_180B30 + 4, 71);
	x_outp(midiPort_180B30 + 5, x_WORD_E39B6);
}

//----- (000919C0) --------------------------------------------------------
void AilShutdown_919C0()
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_shutdown()\n");
	UninstallDrivers_9ED70();
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8))
	{
		if (!GetE3FFE_A16A2())
			fclose(ailDebufFile_181BF0);
	}
	ailIndent_181C04--;
}

//----- (00091A80) --------------------------------------------------------
void AilSetPreference_91A80(uint32_t number, int32_t value)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_preference(%d,%d)\n", number, value);
	AilSetPreference_9E6E0(number, value);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		//dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
}

//----- (00091BD0) --------------------------------------------------------
void PlusE3FF2_91BD0()//fix it - remove
{
	PlusE3FF2_A0EEC();
}

//----- (00091BF0) --------------------------------------------------------
void MinusE3FF2_91BF0()//fix it - remove
{
	MinusE3FF2_A0EF9();
}

//----- (00091C10) --------------------------------------------------------
uint16_t AilGetRealVect_91C10(uint32_t vectnum)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_get_real_vect(0x%X)\n", vectnum);
	uint16_t result = AilApiGetRealVect_A121D(vectnum);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00091D50) --------------------------------------------------------
void AilSetRealVect_91D50(uint32_t vectnum, uint16_t real_ptr)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_real_vect(0x%X,0x%X)\n", vectnum, real_ptr);
	AilApiSetRealVect_A1249(vectnum, real_ptr);
	ailIndent_181C04--;
}

//----- (00091E90) --------------------------------------------------------
void AilRestoreUSE16ISR_91E90(int32_t isr)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_restore_USE16_ISR(%d)\n", isr);
	AilApiRestoreUSE16IISR_A14DB(isr);
	ailIndent_181C04--;
}

//----- (00099C10) --------------------------------------------------------
void EndSounds_99C10()//27ac10
{
	if (soundLoaded_E379A)
	{
		EndSample_8D8F0();
		if (!musicLoaded_E37FE)
			AilShutdown_919C0();
		if (soundIndex_E37A0)
		{
			FreeMem_83E80((uint8_t*)soundIndex_E37A0);
			numOfLoadedSounds_E37A4 = 0;
		}
		if (soundBuffer1_E37A8)
			FreeMem_83E80(soundBuffer1_E37A8);
		soundAble_E3798 = false;
		soundActive_E3799 = false;
		soundLoaded_E379A = false;
	}
}

//----- (00099C90) --------------------------------------------------------
void sub_99C90()//27ac90
{
	if (musicLoaded_E37FE)
	{
		if (songCurrentlyPlaying_E3802)
		{
			AilStopSequence_95DE0((HSEQUENCE)m_hSequence);
			AilEndSequence_95F00(m_hSequence);
			songCurrentlyPlaying_E3802 = 0;
		}
		if (x_BYTE_E3815 == 1)
			sub_9FA80();
		if (!soundLoaded_E379A)
			AilShutdown_919C0();
		if (musicHeader_E3808)
		{
			FreeMem_83E80((uint8_t*)musicHeader_E3808);
			countOfMusicTracks_E380C = 0;
		}
		if (musicData_E3810)
			FreeMem_83E80(musicData_E3810);
		musicAble_E37FC = false;
		musicActive_E37FD = false;
		musicLoaded_E37FE = false;
	}
}

void sub_917D0()
{
	struct tm* timeptr;
	time_t timer;
	
	x_DWORD_181BF4 = 0;
	x_DWORD_181BF8 = 0;
	const char* debugFilename = mygetenv("AIL_DEBUG");
	if (!debugFilename)
	{
		sub_9EC30();
		return;
	}
	if (mygetenv("AIL_SYS_DEBUG"))
		x_DWORD_181BF8 = 1;
	ailDebufFile_181BF0 = myopent((char*)debugFilename, (char*)"w+t"/*(char*)"haw+t"*/);
	if (!ailDebufFile_181BF0)
	{
		sub_9EC30();
		return;
	}
	/*if (isatty(*(_DWORD *)(x_DWORD_181BF0_AIL_debugfile + 16)))
		setbuf(x_DWORD_181BF0_AIL_debugfile, 0);*/ // fix
	time(&timer);
	timeptr = (struct tm*)localtime(&timer);
	strcpy(x_DWORD_181C14, asctime(timeptr));
	x_BYTE_181C2C = 0;
	dbgfprintf(ailDebufFile_181BF0, "-----------------------------------------------------------------");
	dbgfprintf(ailDebufFile_181BF0, "Audio Interface Library application usage script generated by AIL", "3.03");
	dbgfprintf(ailDebufFile_181BF0, "Start time: %s", x_DWORD_181C14);
	dbgfprintf(ailDebufFile_181BF0, "-----------------------------------------------------------------");
	sub_9EC30();
	x_DWORD_181BFC = 0;
	x_DWORD_181C0C = 0;
	x_DWORD_181C08 = nullptr;
	x_DWORD_181C10 = 0;
	//x_DWORD_181C00 = sub_A16AE_register_timer(sub_91660);
	//sub_A1840_AIL_API_set_timer_frequency(x_DWORD_181C00, 100);
	//sub_A1768_start_timer(x_DWORD_181C00);
	x_DWORD_181BF4 = 1;
	ailIndent_181C04 = 1;
	DebugSoundTimer_916F0();
	ailIndent_181C04 = 0;
	dbgfprintf(ailDebufFile_181BF0, "AIL_startup()");
}

const char* mygetenv(const char* envName)//27ae33
{
	unsigned int strLenPlus;
	const char* actEnv;

	int envIndex = 0;
	if (envs_E4CC4 && envName)
	{
		strLenPlus = strlen(envName) + 1;
		while (1)
		{
			actEnv = envs_E4CC4[envIndex];
			if (!envs_E4CC4[envIndex])
				break;
			if (!strnicmp(actEnv, envName, strLenPlus - 1) && actEnv[strLenPlus - 1] == '=')
				return &actEnv[strLenPlus];
			envIndex++;
		}
	}
	return 0;
}


//----- (00093330) --------------------------------------------------------
HDIGDRIVER AilInstallDigDriverFile_93330(char* filename, IO_PARMS* IO)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_install_DIG_driver_file(%s,0x%X)\n", filename, IO);
	HDIGDRIVER result = AilApiInstallDigDriverFile_A3600(filename, IO);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00093480) --------------------------------------------------------
int AilUninstallDigDriver_93480(HDIGDRIVER dig)//AIL_uninstall_DIG_driver //274480
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_uninstall_DIG_driver(0x%X)\n", dig);
	int result = sub_A37A0(dig);
	ailIndent_181C04--;
	return result;
}

//----- (00093510) --------------------------------------------------------
HSAMPLE AilAllocateSampleHandle_93510(HDIGDRIVER dig)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_allocate_sample_handle(0x%X)\n", dig);
	HSAMPLE result = sub_A3820_allocate_sample_handle(dig);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00093830) --------------------------------------------------------
void AilInitSample_93830(HSAMPLE S)//274830
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_init_sample(0x%X)\n", S);
	InitSample_A38E0(S);
	ailIndent_181C04--;
}

//----- (000938C0) --------------------------------------------------------
int32_t AilSetSampleFile_938C0(HSAMPLE S, uint8_t* pWavData, int32_t block)//2748c0
{
	ailIndent_181C04--;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_file(0x%X,0x%X,%d)\n", S, pWavData, block);

	int32_t result = SetSampleFile_A4B20(S, pWavData, block);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00093A10) --------------------------------------------------------
void AilSetSampleAddress_93A10(HSAMPLE S, uint8_t* start, uint32_t len)//274a10
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_address(0x%X,0x%X,%u)\n", S, start, len);
	SetSampleAddress_A3A30(S, start, len);
	ailIndent_181C04--;
}

//----- (00093AB0) --------------------------------------------------------
void AilSetSampleType_93AB0(HSAMPLE S, int32_t channels, uint32_t byteSampleType)//274ab0
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_type(0x%X,%d,%u)\n", S, channels, byteSampleType);
	SetSampleType_A3A70(S, channels, byteSampleType);
	ailIndent_181C04--;
}

//----- (00093B50) --------------------------------------------------------
void AilStartSample_93B50(HSAMPLE S)//274b50
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_start_sample(0x%X)\n", S);
	ApiAilStartSample_A3CB0(S);
	ailIndent_181C04--;
}

//----- (00093D00) --------------------------------------------------------
void AilEndSample_93D00(HSAMPLE S)//274d00
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_end_sample(0x%X)\n", S);
	ApiAilEndSample_A3DA0(S);
	ailIndent_181C04--;
}

//----- (00093D90) --------------------------------------------------------
void AilSetSamplePlaybackRate_93D90(HSAMPLE S, int32_t playback_rate)//274d90
{
	ailIndent_181C04--;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_playback_rate(0x%X,%d)\n", S, playback_rate);
	SetSamplePlaybackRate_A3AF0(S, playback_rate);
	ailIndent_181C04--;
}

//----- (00093E30) --------------------------------------------------------
void AilSetSampleVolume_93E30(HSAMPLE S, int32_t volume)//274e30
{
	SOUND_set_sample_volume(S, volume);
}

//----- (00093ED0) --------------------------------------------------------
void AilSetSampleVolumePan_93ED0(HSAMPLE S, int32_t volume)//274ed0
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_pan(0x%X,%d)\n", S, volume);
	SetSampleVolumePan_A3BB0(S, volume);
	ailIndent_181C04++;
}

//----- (00093F70) --------------------------------------------------------
void AilSetSampleLoopCount_93F70(HSAMPLE S, int loop_count) //274f70
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_loop_count(0x%X,%d)\n", S, loop_count);
	SetSampleLoopCount_A3C90(S, loop_count);
	ailIndent_181C04--;
}

//----- (00094010) --------------------------------------------------------
uint32_t AilSampleStatus_94010(HSAMPLE S)
{
	return SOUND_sample_status(S);
}

void AilDigitalMasterVolume_94650(int master_volume)
{
	SOUND_set_master_volume(master_volume);
}

//----- (00094650) --------------------------------------------------------
void sub_94650_AIL_set_digital_master_volume_orig(HDIGDRIVER driver, int master_volume)//AIL_set_digital_master_volume
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_digital_master_volume(0x%X,%d)\n", driver, master_volume);
	SetDigitalMasterVolume_A3F70(driver, master_volume);
	ailIndent_181C04--;
}

//----- (00095140) --------------------------------------------------------
EOS_callback_type AilRegisterEosCallback_95140(HSAMPLE S, EOS_callback_type EOS)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_register_EOS_callback(0x%X,0x%X)\n", S, EOS);
	EOS_callback_type result = RegisterEosCallback_A3EB0(S, EOS);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (000953E0) --------------------------------------------------------
void AilSetSampleUserData_953E0(HSAMPLE S, int index, int value)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_sample_user_data(0x%X,%u,%d)\n", S, index, value);
	SetSampleUserData_A3F00(S, index, value);
	ailIndent_181C04--;
}

//----- (00095480) --------------------------------------------------------
int AilSampleUserData_95480(HSAMPLE S, int index)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_sample_user_data(0x%X,%u)\n", S, index);
	int result = SampleUserData_A3F30(S, index);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00095710) --------------------------------------------------------
int32_t AilInstallMidiIni_95710(HMDIDRIVER* mdi, char* fileName)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_install_MDI_INI(0x%X)\n", mdi);
	int result = AilApiInstallMDIIni_A78F0(mdi, fileName);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00095850) --------------------------------------------------------
HMDIDRIVER AilInstakkMidiDriverFile_95850(char* filename, IO_PARMS* IO)//276850
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_install_MDI_driver_file(%s,0x%X)\n", filename, IO);
	HMDIDRIVER result = AilApiInstallMDIIni_A77D0(filename, IO);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (000959A0) --------------------------------------------------------
int AilUninstallMidiDriver_959A0(HMDIDRIVER mdi)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_uninstall_MDI_driver(0x%X)\n", mdi);
	int result = sub_A7970_AIL_API_uninstall_MDI_driver(mdi);
	ailIndent_181C04--;
	return result;
}

//----- (00095A30) --------------------------------------------------------
HSEQUENCE AilAllocateSequenceHandle_95A30(HMDIDRIVER mdi)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_allocate_sequence_handle(0x%X)\n", mdi);
	HSEQUENCE result = AilApiAllocateSequenceHandle_A7B30(mdi);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00095C00) --------------------------------------------------------
int32_t AilInitSequence_95C00(HSEQUENCE hSequence, uint8_t* start, int32_t sequence_num, uint32_t track)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_init_sequence(0x%X,0x%X,%d)\n", hSequence, start, sequence_num);
	int result = AilApiInitSequence_A7C20(hSequence, start, sequence_num, track);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00095D50) --------------------------------------------------------
void AilStartSequence_95D50(HSEQUENCE hSequence, uint32_t track)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_start_sequence(0x%X)\n", hSequence);
	AilApiStartSequence_A8010(hSequence, track);
	ailIndent_181C04--;
}

//----- (00095DE0) --------------------------------------------------------
void AilStopSequence_95DE0(HSEQUENCE hSequence)//AIL_stop_sequence
{
	SOUND_stop_sequence(hSequence->sequence_num);
}

//----- (00095DE0) --------------------------------------------------------
void sub_95DE0_AIL_stop_sequence_orig(HSEQUENCE hSequence)//AIL_stop_sequence
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_stop_sequence(0x%X)\n", hSequence);
	sub_A8050_AIL_API_stop_sequence(hSequence);
	ailIndent_181C04--;
}

//----- (00095E70) --------------------------------------------------------
void sub_95E70_AIL_resume_sequence(HSEQUENCE hSequence)//AIL_resume_sequence
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_resume_sequence(0x%X)\n", hSequence);
	sub_A8180_AIL_API_resume_sequence(hSequence);
	ailIndent_181C04--;
}

//----- (00095F00) --------------------------------------------------------
void AilEndSequence_95F00(HSEQUENCE hSequence/*HSEQUENCE S*/)//AIL_end_sequence
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_end_sequence(0x%X)\n", hSequence);
	AilApiEndSequence_A8250(hSequence);
	ailIndent_181C04--;
}

//----- (00096030) --------------------------------------------------------
void AilSetSequenceVolume_96030(int32_t volume, int32_t  milliseconds)
{
	SOUND_set_sequence_volume(volume, milliseconds);
}

//----- (00096170) --------------------------------------------------------
int AilSequenceStatus_96170(HSEQUENCE hSequence/*HSEQUENCE S*/)//AIL_sequence_status
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_sequence_status(0x%X)\n", hSequence);
	int result = AilApiSequenceStatus_A8410(hSequence);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %u\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00096670) --------------------------------------------------------
void AilSetXMidiMasterVolume_96670(int32_t master_volume)
{
	SOUND_set_sequence_volume(master_volume, 0);
}

//----- (000969A0) --------------------------------------------------------
int32_t sub_969A0_AIL_MDI_driver_type(HMDIDRIVER mdi)//AIL_MDI_driver_type
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_MDI_driver_type(0x%X)\n", mdi);
	int32_t result = sub_A7990_AIL_API_MDI_driver_type(mdi);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	return result;
}

//----- (00096AE0) --------------------------------------------------------
void AilSetGtlFilenamePrefix_96AE0(char* samplesPath)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_set_GTL_filename_prefix(%s)\n", samplesPath);
	AilApiSetGTLFilenamePrefix_A7AA0(samplesPath);
	ailIndent_181C04--;
}

//----- (00097330) --------------------------------------------------------
void AilSequencePosition_97330(HSEQUENCE hSequence, x_DWORD* beat, x_DWORD* measure)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_sequence_position(0x%X,0x%X,0x%X)\n", hSequence, beat, measure);
	sub_A8830(hSequence, beat, measure);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8))
	{
		if (!GetE3FFE_A16A2())
		{
			for (int i = 0; i < 14; i++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int j = 1; j < ailIndent_181C04; j++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "Result = %d:%d\n", *measure + 1, *beat + 1);
		}
	}
	ailIndent_181C04--;
}

//----- (00097480) --------------------------------------------------------
void AilBranchIndex_97480(HSEQUENCE hSequence, int marker)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_branch_index(0x%X,%u)\n", hSequence, marker);
	sub_A8900(hSequence, marker);
	ailIndent_181C04--;
}

//----- (00097670) --------------------------------------------------------
void AilRegisterTriggerCallback_97670(HSEQUENCE hSequence, xCALLBACK callback)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_register_trigger_callback(0x%X,0x%X)\n", hSequence, callback);
	AilApiTriggerCallback_A8A20(hSequence, callback);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		//dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
}

//----- (00097F90) --------------------------------------------------------
int AilLockChannel_97F90(MDI_DRIVER* mdi)
{
	ailIndent_181C04--;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_lock_channel(0x%X)\n", mdi);
	int result = AilApiLockChannel_97F90_A8BF0(mdi);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (000980D0) --------------------------------------------------------
void AilReleaseChannel_980D0(HMDIDRIVER mdi, int channel)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_release_channel(0x%X,%d)\n", mdi, channel);
	sub_A8EA0(mdi, channel);
	ailIndent_181C04--;
}

//----- (00098170) --------------------------------------------------------
void AilMapSequenceChannel_98170(HSEQUENCE S, int seq_channel, int new_channel)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_map_sequence_channel(0x%X,%d,%d)\n", S, seq_channel, new_channel);
	sub_A9080(S, seq_channel, new_channel);
	ailIndent_181C04--;
}

//----- (00098360) --------------------------------------------------------
void AilSendChannelVoiceMessage_98360(HMDIDRIVER mdi, HSEQUENCE hSequence, int32_t status, int32_t data_1, int32_t data_2)//AIL_send_channel_voice_message // 279360
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_send_channel_voice_message(0x%X,0x%X,0x%X,0x%X,0x%X)\n", mdi, hSequence, status, data_1, data_2);
	AilAPISendChannelVoiceMessage_A9140(mdi, hSequence, status, data_1, data_2);
	ailIndent_181C04--;
}

//----- (000986E0) --------------------------------------------------------
void sub_986E0()//2796e0
{
	sub_99C90();
	EndSounds_99C10();
	if (x_BYTE_E39B8)
		sub_91420();
}

//----- (00098790) --------------------------------------------------------
void SetMusicVolume_98790(int milliseconds, int volume)//279790
{
	if (musicAble_E37FC && musicActive_E37FD && songCurrentlyPlaying_E3802 && volume <= 0x7Fu && AilSequenceStatus_96170(m_hSequence) != 2)
		AilSetSequenceVolume_96030(volume, milliseconds);
}

//----- (00098AE9) --------------------------------------------------------
__int16 sub_98AE9(__int16* a1, int a2)//279ae9
{
	int v2; // edx
	__int16* v3; // esi
	__int16* v4; // et0
	__int16 v5; // ax
	__int16* v6; // esi
	__int16 v7; // bx
	__int16 v8; // ax
	__int16 v9; // cx
	int v10; // esi
	__int16 result; // ax
	unsigned __int8 v12; // cl
	__int16 v13; // ax
	char v14; // cl

	//fix it
	v12 = 0;
	v14 = 0;
	//fix it

	v4 = a1;
	v2 = a2;
	v3 = v4;
	do
	{
		v5 = *v3;
		v6 = v3 + 1;
		v7 = x_WORD_E3B4C & v5;
		v8 = *v6;
		v3 = v6 + 1;
	} while (v8 != v7);
	v9 = v3[30];
	v10 = v2;
	result = sub_98B2C(HIBYTE(v9), v2);
	if (v12 >= 2u)
	{
		v13 = sub_98B2C(v12 - 1, v10);
		result = (1 << v14) | v13;
	}
	return result;
}

//----- (00084300) --------------------------------------------------------
bool LoadSound_84300(uint8_t soundIndex)//265300
{
	FILE* file;
	long sizeOfFile;
	int16_t lastSoundBank[6];
	int32_t lastSoundBankPos;
	uint8_t soundIndex2 = 0;

	if (soundAble_E3798 && actualSound_E37AD != soundIndex)
	{
		if (soundActiveH_E2A14)
			EndSample_8D8F0();

		std::string soundPath = GetSubDirectoryFile(cdFolder, "SOUND", "SOUND.DAT");
		file = DataFileIO::CreateOrOpenFile(soundPath.c_str(), 512);
		
		if (file != NULL)
		{
			DataFileIO::Seek(file, 0, 2);
			sizeOfFile = myftell(file);
			DataFileIO::Seek(file, sizeOfFile - 4, 0);
			DataFileIO::Read(file, (uint8_t*)&lastSoundBankPos, 4);
			DataFileIO::Seek(file, lastSoundBankPos, 0);
			DataFileIO::Read(file, (uint8_t*)lastSoundBank, 12);
			switch (soundFreqType_E37B6)
			{
				case 800:
					soundIndex2 = 5;//800
					break;
				case 811:
					soundIndex2 = 4;//811
					break;
				case 822:
					soundIndex2 = 3;//822
					break;
				case 1611:
					soundIndex2 = 2;//1611
					break;
				case 1622:
					soundIndex2 = 1;//1622
					break;
				case 1644:
					soundIndex2 = 0;//1644
					break;
			}
			if ((soundIndex + 1) > lastSoundBank[soundIndex2])
			{
				DataFileIO::Close(file);
				return true;
			}
			DataFileIO::Seek(file, 96 * soundIndex, 1);//seek to finded sound
			if (!ReadAndDecompressSound(file, soundIndex2))
			{
				DataFileIO::Close(file);
				return true;
			}
			actualSound_E37AD = soundIndex;
			DataFileIO::Close(file);
		}
	}
	return false;
}

//----- (000844A0) --------------------------------------------------------
void LoadSoundDataFromBuffer_844A0(uint16_t count)//2654a0
{
	int index = indexLoadedSound_180B50;
	if (soundIndex_E37A0 && soundBuffer1_E37A8)
	{
		for (index = 0; index < count; index++)
		{
#ifdef x32_BIT_ENVIRONMENT
			soundIndex_E37A0->str_8.wavs_10[index].wavData_0 = reinterpret_cast<uint32_t>(soundIndex_E37A0->str_8.wavs_10[index].wavData_0) + soundBuffer1_E37A8;
#endif //x32_BIT_ENVIRONMENT
#ifdef x64_BIT_ENVIRONMENT
			soundIndex_E37A0->str_8.wavs_10[index].wavData_0 = reinterpret_cast<uint64_t>(soundIndex_E37A0->str_8.wavs_10[index].wavData_0) + soundBuffer1_E37A8;
#endif //x64_BIT_ENVIRONMENT
		}
	}
	indexLoadedSound_180B50 = index;
}

//----- (000844F0) --------------------------------------------------------
bool ReadAndDecompressSound(FILE* file, uint8_t soundIndex2)//2654f0
{
	type_v8 soundBank2[6];
	
	shadow_type_E37A0_sound_buffer2* shadow_str_E37A0_sound_buffer2=0;//64x fix

	myftell(file);
	//3550d8
	DataFileIO::Read(file, (uint8_t*)soundBank2, 96);
	if (soundBank2[soundIndex2].dword_4 == -1)
		return false;

	if (!soundActiveH_E2A14)
	{
		soundBuffer1_E37A8 = (uint8_t*)Malloc_83CD0(soundBank2[soundIndex2].dword_12 + 256);
		soundIndex_E37A0 = (type_E37A0_sound_buffer2*)Malloc_83CD0(sizeof(type_E37A0_sound_buffer2));
		soundActiveL_E2A14 = 1;
		if (!soundBuffer1_E37A8 || !soundIndex_E37A0)
		{
			FreeMem_83E80(soundBuffer1_E37A8);
			FreeMem_83E80((uint8_t*)soundIndex_E37A0);
			soundActiveL_E2A14 = 0;
			actualSound_E37AD = -1;
			return false;
		}
		soundBufferLen_E2A18 = soundBank2[soundIndex2].dword_12 + 256;
		soundActiveH_E2A14 = 1;
	}
	soundAble_E3798 = false;
	if (!soundBuffer1_E37A8 || !soundIndex_E37A0)
		return false;
	memset((void*)soundBuffer1_E37A8, 0, soundBufferLen_E2A18);

	//64xfix
	shadow_str_E37A0_sound_buffer2 = (shadow_type_E37A0_sound_buffer2*)Malloc_83CD0(sizeof(shadow_type_E37A0_sound_buffer2));
	if (!shadow_str_E37A0_sound_buffer2)
	{
		FreeMem_83E80((uint8_t*)shadow_str_E37A0_sound_buffer2);
		return false;
	}

	memset((void*)shadow_str_E37A0_sound_buffer2, 0, sizeof(shadow_type_E37A0_sound_buffer2));
	numOfLoadedSounds_E37A4 = (soundBank2[soundIndex2].sizeBytes_8) / sizeof(shadow_sub2type_E37A0_sound_buffer2);
	DataFileIO::Seek(file, soundBank2[soundIndex2].dword_4, 0);
	DataFileIO::Read(file, soundBuffer1_E37A8, 8);
	if (soundBuffer1_E37A8[0] != 'R' || soundBuffer1_E37A8[1] != 'N' || soundBuffer1_E37A8[2] != 'C')
	{
		DataFileIO::Read(file, (soundBuffer1_E37A8 + 8), soundBank2[soundIndex2].dword_12 - 8);
	}
	else
	{
		DataFileIO::Read(file, (soundBuffer1_E37A8 + 8), soundBuffer1_E37A8[7] + ((soundBuffer1_E37A8[6] + ((soundBuffer1_E37A8[5] + (soundBuffer1_E37A8[4] << 8)) << 8)) << 8) - 8);
		DataFileIO::Decompress(soundBuffer1_E37A8, soundBuffer1_E37A8);
	}
	DataFileIO::Seek(file, soundBank2[soundIndex2].dword_0, 0);
	DataFileIO::Read(file, (uint8_t*)shadow_str_E37A0_sound_buffer2, 8);
	if (shadow_str_E37A0_sound_buffer2->byte_0 != 'R' || shadow_str_E37A0_sound_buffer2->byte_1 != 'N' || shadow_str_E37A0_sound_buffer2->byte_2 != 'C')//R N C
	{
		DataFileIO::Read(file, (uint8_t*)&shadow_str_E37A0_sound_buffer2->str_8, soundBank2[soundIndex2].sizeBytes_8 - 8);
	}
	else
	{
		DataFileIO::Read(file, (uint8_t*)&shadow_str_E37A0_sound_buffer2->str_8, shadow_str_E37A0_sound_buffer2->byte_7 + ((shadow_str_E37A0_sound_buffer2->byte_6 + ((shadow_str_E37A0_sound_buffer2->byte_5 + (shadow_str_E37A0_sound_buffer2->byte_4 << 8)) << 8)) << 8) - 8);
		DataFileIO::Decompress((uint8_t*)shadow_str_E37A0_sound_buffer2, (uint8_t*)shadow_str_E37A0_sound_buffer2);
	}
	soundIndex_E37A0->byte_0 = shadow_str_E37A0_sound_buffer2->byte_0;
	soundIndex_E37A0->byte_1 = shadow_str_E37A0_sound_buffer2->byte_1;
	soundIndex_E37A0->byte_2 = shadow_str_E37A0_sound_buffer2->byte_2;
	soundIndex_E37A0->byte_3 = shadow_str_E37A0_sound_buffer2->byte_3;
	soundIndex_E37A0->byte_4 = shadow_str_E37A0_sound_buffer2->byte_4;
	soundIndex_E37A0->byte_5 = shadow_str_E37A0_sound_buffer2->byte_5;
	soundIndex_E37A0->byte_6 = shadow_str_E37A0_sound_buffer2->byte_6;
	soundIndex_E37A0->byte_7 = shadow_str_E37A0_sound_buffer2->byte_7;
	for(int i = 0; i < 10; i++)
		soundIndex_E37A0->str_8.stub[i] = shadow_str_E37A0_sound_buffer2->str_8.stub[i];

	for (int i = 0; i < 96; i++)
	{
		soundIndex_E37A0->str_8.wavs_10[i].wavData_0 = (uint8_t*)shadow_str_E37A0_sound_buffer2->str_8.wavs_10[i].wavData_0;
		for (int j = 0; j < 4; j++)
		{
			soundIndex_E37A0->str_8.wavs_10[i].stub_4[j] = shadow_str_E37A0_sound_buffer2->str_8.wavs_10[i].stub_4[j];
		}

		soundIndex_E37A0->str_8.wavs_10[i].wavSize_8 = shadow_str_E37A0_sound_buffer2->str_8.wavs_10[i].wavSize_8;
		soundIndex_E37A0->str_8.wavs_10[i].word_12 = shadow_str_E37A0_sound_buffer2->str_8.wavs_10[i].word_12;
		for (int j = 0; j < 18; j++)
		{
			soundIndex_E37A0->str_8.wavs_10[i].filename_14[j] = shadow_str_E37A0_sound_buffer2->str_8.wavs_10[i].filename_14[j];
		}
	}
	
	for (int i = 0; i < 10; i++)
		soundIndex_E37A0->next_str[i] = shadow_str_E37A0_sound_buffer2->next_str[i];
	FreeMem_83E80((uint8_t*)shadow_str_E37A0_sound_buffer2);
	//64xfix

	LoadSoundDataFromBuffer_844A0(numOfLoadedSounds_E37A4);
	soundAble_E3798 = true;
	return true;
}

//----- (0009E3A0) --------------------------------------------------------
bool AilApiReadIni_9E3A0(AIL_INI* INI, char* filename)//27f3a0
{
	bool result;
	memset(INI->device_name, 0, 280);
	memset(&INI->IO, -1, 24);

	//fix
	if (!strcmp(filename + strlen(filename) - strlen("DIG.INI"), "DIG.INI"))
	{
		strcpy(INI->device_name, "Creative Labs Sound Blaster 16 or AWE32");
		strcpy(INI->driver_name, "SB16.DIG");
		char workingDir[MAX_PATH];
		GetDirectory(workingDir, filename);
		sprintf(INI->driver_path, "%s/%s", workingDir, "SB16.DIG");
		INI->IO.IO = StrToInt("220h", 16);
		INI->IO.IRQ = StrToInt("-1", 10);
		INI->IO.DMA_8_bit = StrToInt("-1", 10);
		INI->IO.DMA_16_bit = StrToInt("-1", 10);
	}
	if (!strcmp(filename + strlen(filename) - strlen("MDI.INI"), "MDI.INI"))
	{
		strcpy(INI->device_name, "Creative Labs Sound Blaster(TM) 16");
		strcpy(INI->driver_name, "SBPRO2.MDI");
			char workingDir[MAX_PATH];
		GetDirectory(workingDir, filename);
		sprintf(INI->driver_path, "%s/%s", workingDir, "SBPRO2.MDI");
		INI->IO.IO = StrToInt((char*)"220h", 16);
		INI->IO.IRQ = StrToInt((char*)"-1", 10);
		INI->IO.DMA_8_bit = StrToInt((char*)"-1", 10);
		INI->IO.DMA_16_bit = StrToInt((char*)"-1", 10);

	}
	//fix

	if (strlen(INI->device_name))
	{
		result = true;
	}
	else
	{
		qmemcpy(textBuffer_181C90, (void*)"Corrupted .INI file\n", 21);
		result = false;
	}
	return result;
}

//----- (0009E6E0) --------------------------------------------------------
void AilSetPreference_9E6E0(uint32_t number, int32_t value)
{
	preference_181DAC[number] = value;
}

//----- (0009E720) --------------------------------------------------------
AIL_DRIVER* AilApiInstallDriver_9E720(uint8_t* driver_image, int32_t n_bytes)//27f720
{
	AIL_DRIVER* result;
	unsigned int i;
	AIL_DRIVER* ailDriver;

	ailDriver = (AIL_DRIVER*)malloc(sizeof(AIL_DRIVER));
	if (ailDriver)
	{
		ailDriver->size_3 = n_bytes;
		if (AllocMem_9D4D0((n_bytes + 15) >> 4, &ailDriver->buf_2, &ailDriver->seg_0, &ailDriver->sel_1))
		{
			memmove((void*)ailDriver->buf_2, (const void*)driver_image, n_bytes);
			//fix it - make buf reader
			ailDriver->VHDR_4 = (VDI_HDR*)ailDriver->buf_2;
			if (!_strnicmp((const char*)ailDriver->VHDR_4->ID_8, "AIL3DIG", 7))
			{
				ailDriver->type_5 = 0;
			}
			else
			{
				if (_strnicmp((char*)ailDriver->VHDR_4->ID_8, "AIL3MDI", 7))
				{
					qmemcpy(textBuffer_181C90, (void*)"Invalid driver type\n", 21);
					FreeMem_9D560(ailDriver->buf_2);
					FreeMem_9D490(ailDriver, 44);
					return 0;
				}
				ailDriver->type_5 = 1;
			}
			for (i = 0; i < 16; i++)
			{
				if (!ailDrivers_181C50[i])
				{
					ailDrivers_181C50[i] = ailDriver;
					ailDriver->VHDR_4->VDI_HDR_var50 = i;
					break;
				}
			}
			if (i != 16)
			{
				ailDriver->VHDR_4->VDI_HDR_var48 = 0;
				ailDriver->initialized_6 = 0;
				ailDriver->PM_ISR_7 = -1;
				ailDriver->VHDR_4->VDI_HDR_var54_prev_ISR = AilGetRealVect_91C10(102);//save old vector
				AilSetRealVect_91D50(102, ailDriver->VHDR_4->VDI_HDR_var52_this_ISR);//set new vector
				ailDriver->destructor_9 = nullptr;
				ailDriver->descriptor_10 = nullptr;
				AilCallDriver_91F70(ailDriver, 768, nullptr, nullptr);
				if (ailDriver->VHDR_4->VDI_HDR_var46 > 0)
				{
					ailDriver->server_8 = 1;//fix;
					if (ailDriver->server_8 == -1)
					{
						qmemcpy(textBuffer_181C90, (void*)"Out of timer handles\n", 22);
						AilSetRealVect_91D50(102, ailDriver->VHDR_4->VDI_HDR_var54_prev_ISR);
						FreeMem_9D560((uint8_t*)ailDriver->buf_2);
						FreeMem_9D490(ailDriver, 44);
						return 0;
					}
				}
				else
				{
					ailDriver->server_8 = -1;
				}
				return ailDriver;
			}
			qmemcpy(textBuffer_181C90, (void*)"Out of driver handles\n", 23);
			FreeMem_9D560((uint8_t*)ailDriver->buf_2);
			FreeMem_9D490(ailDriver, 44);
			result = 0;
		}
		else
		{
			qmemcpy(textBuffer_181C90, (void*)"Insufficient low memory\n", 25);
			FreeMem_9D490(ailDriver, 44);
			result = 0;
		}
	}
	else
	{
		strcpy(textBuffer_181C90, "Insufficient memory for driver descriptor\n");
		result = 0;
	}
	return result;
}

//----- (0009EA60) --------------------------------------------------------
int AilApiUninstallDriver_9EA60(AIL_DRIVER* ailDriver)
{
	if (ailDriver->server_8 != -1)
		;// sub_92DC0_AIL_release_timer_handle(a1->server_8);
	if (ailDriver->initialized_6)
	{
		if (ailDriver->PM_ISR_7 != -1)
			AilRestoreUSE16ISR_91E90(ailDriver->PM_ISR_7);
		AilCallDriver_91F70(ailDriver, 774, nullptr, nullptr);
	}
	AilSetRealVect_91D50(102, ailDriver->VHDR_4->VDI_HDR_var54_prev_ISR);
	FreeMem_9D560(ailDriver->buf_2);
	int result = FreeMem_9D490(ailDriver, 44);
	for (int i = 0; i < 16; i++)
	{
		if (ailDrivers_181C50[i] == ailDriver)
			ailDrivers_181C50[i] = nullptr;
		result = i;
	}
	return result;
}

//----- (0009EB60) --------------------------------------------------------
IO_PARMS* AilApiGetIoEnvironment_9EB60(AIL_DRIVER* drvr)
{
	return &ioParms_181DF8;
}

//----- (0009EC30) --------------------------------------------------------
void sub_9EC30()
{
	AilSetPreference_91A80(0, 200);
	AilSetPreference_91A80(1, 1);
	AilSetPreference_91A80(2, 0x8000);
	AilSetPreference_91A80(3, 100);
	AilSetPreference_91A80(4, 16);
	AilSetPreference_91A80(5, 100);
	AilSetPreference_91A80(6, 655);
	AilSetPreference_91A80(7, 0);
	AilSetPreference_91A80(8, 0);
	AilSetPreference_91A80(9, 1);
	AilSetPreference_91A80(10, 0);
	AilSetPreference_91A80(11, 120);
	AilSetPreference_91A80(12, 8);
	AilSetPreference_91A80(13, 127);
	AilSetPreference_91A80(14, 1);
	AilSetPreference_91A80(15, 0);
	AilSetPreference_91A80(16, 2);
	AilSetPreference_91A80(17, 1);
	AilSetPreference_91A80(18, 1);
	for (int i = 0; i < 16; i++)
		ailDrivers_181C50[i] = nullptr;
	textBuffer_181C90[0] = 0;
	sub_A10F4_sound_proc_irq();
}

//----- (0009ED70) --------------------------------------------------------
void UninstallDrivers_9ED70()
{
	for (int i = 15; i >= 0; i--)
	{
		AIL_DRIVER* ailDriver = ailDrivers_181C50[i];
		if (ailDriver)
			AilUninstallDriver_93160(ailDriver);
	}
	sub_A11E2();
}

//----- (0009EE70) --------------------------------------------------------
int sub_9EE70()
{
	if (!sampleFinished_E3E45 && AilSampleUserData_95480(mainSample_181E10, x_DWORD_E3E40) && (unsigned int)x_DWORD_E3E40 < 8)
	{
		AilInitSample_93830(mainSample_181E10);
		AilRegisterEosCallback_95140(mainSample_181E10, sub_9EE70);
		AilSetSampleFile_938C0(mainSample_181E10, soundIndex_E37A0->str_8.wavs_10[AilSampleUserData_95480(mainSample_181E10, x_DWORD_E3E40)].wavData_0, 1);
		x_DWORD_E3E40++;
		AilStartSample_93B50(mainSample_181E10);
	}
	return 0;
}

//----- (0009F040) --------------------------------------------------------
void sub_9F040()
{
	if (useSampleData_E3E44)
	{
		sampleFinished_E3E45 = true;
		AilEndSample_93D00(mainSample_181E10);
		AilSetSampleUserData_953E0(mainSample_181E10, 0, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 1, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 2, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 3, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 4, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 5, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 6, 0);
		AilSetSampleUserData_953E0(mainSample_181E10, 7, 0);
	}
}

//----- (0009FA80) --------------------------------------------------------
void sub_9FA80()//280a80
{
	if (musicAble_E37FC && !_stricmp(textBuffer_180BE0, "SBAWE32.MDI") && !_stricmp(&musicDriverType_180C84, "w"))
	{
		sub_9F6D0(hMdiMusicDriver, 1);
		sub_9F170(x_DWORD_181E2C, x_WORD_181E30);
		sub_9F170(x_DWORD_181E26, x_WORD_181E2A);
		x_BYTE_E3815 = 0;
	}
}

//----- (0009F280) --------------------------------------------------------
int sub_9F280(HMDIDRIVER hMdiDriver)//a1 bude nejaky driver
{
	VDI_CALL v2; // [esp+0h] [ebp-10h]
	//__int16 v3; // [esp+4h] [ebp-Ch]
	__int16 v4; // [esp+6h] [ebp-Ah]
	__int16 v5; // [esp+8h] [ebp-8h]
	int v6; // [esp+Ch] [ebp-4h]

	//fix it
	v4 = 0;
	v5 = 0;
	//fix it

	//v3 = 16;
	AilCallDriver_91F70(hMdiDriver->drvr_0, 1286, &v2, &v2);
	if (v2.AX == 0)//fix
		v6 = -1;
	else
		v6 = v4 << (v5 + 16);
	return v6;
}

//----- (0009F2E0) --------------------------------------------------------
VDI_CALL sub_9F2E0(HMDIDRIVER hMdiDriver, int  /*a2*/, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	VDI_CALL v7; // [esp+0h] [ebp-10h]

	if (!str_E3E54.par1 && !str_E3E54.par2)
	{
		v4 = sub_9F110(4);
		str_E3E54.par2 = WORD2(v4);
		str_E3E54.par1 = v4;
		if (!(x_DWORD)v4 && !str_E3E54.par2)
		{
			v7.AX = -1;
			return v7;
		}
	}
	__writegsx_WORD(str_E3E54.par1, a4);
	v5 = sub_9F220(a3);
	__writegsx_WORD(str_E3E54.par1 + 2, v5);
	//v8 = 17;
	sub_9F220(str_E3E54.par2);
	//v10 = 0;
	AilCallDriver_91F70(hMdiDriver->drvr_0, 1286, &v7, &v7);
	return v7;
}

//----- (0009F3D0) --------------------------------------------------------
__int64 sub_9F3D0(HMDIDRIVER hMdiDriver, int  /*a2*/, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	int v6; // ebx
	VDI_CALL v8; // [esp+0h] [ebp-14h]
	//__int16 v9; // [esp+4h] [ebp-10h]
	__int16 v10; // [esp+6h] [ebp-Eh]
	__int16 v11; // [esp+8h] [ebp-Ch]
	__int64 v12; // [esp+Ch] [ebp-8h]

	if (str_E3E54.par1
		|| str_E3E54.par2
		|| (v4 = sub_9F110(4), str_E3E54.par2 = WORD2(v4), (str_E3E54.par1 = v4) != 0))
	{
		//__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
		__writegsx_WORD(str_E3E54.par1, a4);
		v5 = sub_9F220(a3);
		//__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
		__writegsx_WORD(str_E3E54.par1 + 2, v5);
		//v9 = 23;
		v10 = sub_9F220(str_E3E54.par2);
		v11 = 0;
		AilCallDriver_91F70(hMdiDriver->drvr_0, 1286, &v8, &v8);
		if (v8.AX != NULL)
		{
			WORD2(v12) = 0;
			LODWORD(v12) = 0;
		}
		else
		{
			v6 = v11;
			WORD2(v12) = sub_9F1D0(v10);
			LODWORD(v12) = v6;
		}
	}
	else
	{
		WORD2(v12) = 0;
		LODWORD(v12) = 0;
	}
	return v12;
}

//----- (0009F4F0) --------------------------------------------------------
VDI_CALL sub_9F4F0(HMDIDRIVER hMdiDriver, int  /*a2*/, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	VDI_CALL result; // [esp+0h] [ebp-10h]
	//__int16 v8; // [esp+4h] [ebp-Ch]
	//__int16 v9; // [esp+6h] [ebp-Ah]
	//__int16 v10; // [esp+8h] [ebp-8h]

	if (!str_E3E54.par1 && !str_E3E54.par2)
	{
		v4 = sub_9F110(4);
		str_E3E54.par2 = WORD2(v4);
		str_E3E54.par1 = v4;
		if (!(x_DWORD)v4 && !str_E3E54.par2)
		{
			result.AX = -1;
			return result;
		}
	}
	//__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(str_E3E54.par1, a4);
	v5 = sub_9F220(a3);
	//__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(str_E3E54.par1 + 2, v5);
	//v8 = 24;
	sub_9F220(str_E3E54.par2);
	//v10 = 0;
	AilCallDriver_91F70(hMdiDriver->drvr_0, 1286, &result, &result);
	return result;
}

//----- (0009F5E0) --------------------------------------------------------
VDI_CALL sub_9F5E0(HMDIDRIVER hMdiDriver, int  /*a2*/, unsigned __int16 a3, unsigned __int16 a4)
{
	signed __int64 v4; // rax
	unsigned __int16 v5; // ax
	VDI_CALL result; // [esp+0h] [ebp-10h]
	//__int16 v8; // [esp+4h] [ebp-Ch]
	//__int16 v9; // [esp+6h] [ebp-Ah]
	//__int16 v10; // [esp+8h] [ebp-8h]

	if (!str_E3E54.par1 && !str_E3E54.par2)
	{
		v4 = sub_9F110(4);
		str_E3E54.par2 = WORD2(v4);
		str_E3E54.par1 = v4;
		if (!(x_DWORD)v4 && !str_E3E54.par2)
		{
			result.AX = -1;
			return result;
		}
	}
	//__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(str_E3E54.par1, a4);
	v5 = sub_9F220(a3);
	//__GS__ = *(x_WORD *)&x_BYTE_E3E54[4];
	__writegsx_WORD(str_E3E54.par1 + 2, v5);
	//v8 = 25;
	sub_9F220(str_E3E54.par2);
	//v10 = 0;
	AilCallDriver_91F70(hMdiDriver->drvr_0, 1286, &result, &result);
	return result;
}

//----- (0009F6D0) --------------------------------------------------------
VDI_CALL sub_9F6D0(HMDIDRIVER hMdiDriver, __int16 a2)
{
	VDI_CALL result; // [esp+0h] [ebp-10h]

	sub_9F170(str_E3E54.par1, str_E3E54.par2);
	str_E3E54.par2 = 0;
	str_E3E54.par1 = 0;
	//v4 = 18;
	//v5 = a2;
	AilCallDriver_91F70(hMdiDriver->drvr_0, 1286, &result, &result);
	return result;
}

//----- (0009F740) --------------------------------------------------------
void InitAWE32_9F740(char* textBullfrog)//280740
{
	signed __int64 v1; // rax
	__int64 v2; // rax
	signed __int64 v3; // rax
	//char v4; // [esp+0h] [ebp-34h]
	type_str_32_16 v5x;
	type_str_32_16 v6x; // [esp+1Ch] [ebp-18h]
	unsigned int v7; // [esp+24h] [ebp-10h]
	char v8; // [esp+28h] [ebp-Ch]
	int v9; // [esp+2Ch] [ebp-8h]
	int i; // [esp+30h] [ebp-4h]

	FILE* file;

	if (musicAble_E37FC && !_stricmp(textBuffer_180BE0, "SBAWE32.MDI") && !_stricmp(&musicDriverType_180C84, "w"))
	{
		if (x_BYTE_E3815 == 1)
		{
			sub_9FA80();
			x_BYTE_E3815 = 0;
		}
		char bulltemp[512] = "Bullfrog";//fix it
		strcpy(bulltemp, (const char*)textBullfrog);
		sprintf(printbuffer, "sound//%s.sbk", bulltemp);
		file = DataFileIO::CreateOrOpenFile(printbuffer, 512);
		if (file != NULL)
		{
			v7 = DataFileIO::FileLengthBytes(file);
			v1 = sub_9F110(512);
			x_WORD_181E30 = WORD2(v1);
			x_DWORD_181E2C = v1;
			if ((x_DWORD)v1 || x_WORD_181E30)
			{
				v9 = sub_9F280(hMdiMusicDriver);
				if (v9 != -1)
				{
					v5x.par2 = x_WORD_181E30;
					v5x.par1 = x_DWORD_181E2C;
					__writegsx_DWORD(x_DWORD_181E2C, 0);
					__writegsx_DWORD(v5x.par1 + 4, v7);
#ifdef TEST_x64
	allert_error();
#endif
					if (sub_9F2E0(hMdiMusicDriver, v5x.par1, v5x.par2, 2u).AX == -1//fix
						&& !dos_read(file, x_DWORD_181E2C, (unsigned __int16)x_WORD_181E30, 512, &v8))
					{
						v2 = sub_9F3D0(hMdiMusicDriver, x_DWORD_181E2C, x_WORD_181E30, 1u);
						v5x.par2 = WORD2(v2);
						v5x.par1 = v2;
						if ((x_DWORD)v2 || v5x.par2)
						{
							DataFileIO::Seek(file, __readgsx_DWORD(v5x.par1 + 2), 0);
							for (i = 0; (signed __int16)__readgsx_WORD(v5x.par1) > i; i++)
							{
								if (dos_read(file, x_DWORD_181E2C, (unsigned __int16)x_WORD_181E30, 512, &v8)
									|| sub_9F4F0(hMdiMusicDriver, x_DWORD_181E2C, x_WORD_181E30, 1u).AX != -1)//fix
								{
									return;
								}
							}
							v3 = sub_9F110(__readgsx_DWORD(v5x.par1 + 10));
							x_WORD_181E2A = WORD2(v3);
							x_DWORD_181E26 = v3;
							if ((x_DWORD)v3 || x_WORD_181E2A)
							{
								DataFileIO::Seek(file, __readgsx_DWORD(v5x.par1 + 6), 0);
								if (!dos_read(
									file,
									x_DWORD_181E26,
									(unsigned __int16)x_WORD_181E2A,
									__readgsx_DWORD(v5x.par1 + 10),
									&v8)
									&& sub_9F5E0(hMdiMusicDriver, x_DWORD_181E26, x_WORD_181E2A, 1u).AX == -1)//fix
								{
									x_BYTE_E3815 = 1;
									DataFileIO::Close(file);
								}
							}
						}
					}
				}
			}
		}
	}
}

//----- (000A0EEC) --------------------------------------------------------
void PlusE3FF2_A0EEC()//281eec
{
	x_DWORD_E3FF2++;
}

//----- (000A0EF9) --------------------------------------------------------
void MinusE3FF2_A0EF9()//281ef9
{
	x_DWORD_E3FF2--;
}

//----- (000A102C) --------------------------------------------------------
char sub_A102C(int a1)//28202c //fix
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	unsigned int v1; // et0
	char result; // al
	unsigned int v3; // [esp-4h] [ebp-10h]
	void* retaddr; // [esp+10h] [ebp+4h]

	//fix it
	retaddr = 0;
	//fix it

	v1 = x__readeflags();
	v3 = v1;
	//_disable();
	//__outx_BYTE(0x43u, 0x36u);
	x_DWORD_E3FE6 = a1;
	//__outx_BYTE(0x40u, a1);
	result = BYTE1(a1);
	//__outx_BYTE(0x40u, BYTE1(a1));
	//_disable();
	if (BYTE1(retaddr) & 2)
		;//_enable();
	x__writeeflags(v3);
	return result;
}

//----- (000A105C) --------------------------------------------------------
char sub_A105C(unsigned int a1)//28205c
{
	int v1; // eax

	v1 = 0;
	if (a1 < 0xD68D)
		v1 = 10000 * (unsigned __int64)a1 / 0x20BC;
	return sub_A102C(v1);
}

//----- (000A108F) --------------------------------------------------------
void sub_A108F()//28208f
{
	unsigned int v0; // ecx
	unsigned int v1; // edi

	PlusE3FF2_91BD0();
	v0 = -1;
	v1 = 0;
	do
	{
		if (x_DWORD_E3E9C[v1] && x_DWORD_E3F1C[v1] < v0)
			v0 = x_DWORD_E3F1C[v1];
		v1++;
	} while (v1 < 16);
	if (v0 != x_DWORD_E3FEA)
	{
		x_DWORD_E3FEA = v0;
		sub_A105C(v0);
		memset(x_DWORD_E3EDC, 0, 64);
	}
	MinusE3FF2_91BF0();
}

//----- (000A10F4) --------------------------------------------------------
void sub_A10F4_sound_proc_irq()//2820f4
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	unsigned __int16 v1 = 0;

	x_DWORD_E3FEE = 0;
	x_DWORD_E3FF2 = 0;
	x_DWORD_E3FEA = -1;
	isr_E3FF8 = -1;
	memset(x_DWORD_E3E9C, 0, 64);
	memset(x_DWORD_E3EDC, 0, 64);
	memset(x_DWORD_E3F1C, 0, 64);
	memset(x_DWORD_E3F5C, 0, 64);

	x_DWORD_E3FDC = 8;
	x_DWORD_E3FE2 = v1;
	x_DWORD_E3E9C[15] = 2;
}

//----- (000A11E2) --------------------------------------------------------
void sub_A11E2()//2821e2
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();
}

//----- (000A121D) --------------------------------------------------------
uint16_t AilApiGetRealVect_A121D(uint32_t vectnum)//28221d
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();
	return ac_get_real_vect(vectnum);
}

//----- (000A1249) --------------------------------------------------------
void AilApiSetRealVect_A1249(uint32_t vectnum, uint16_t real_ptr)//282249
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();
	ac_set_real_vect(vectnum, real_ptr);
}

//----- (000A14DB) --------------------------------------------------------
void AilApiRestoreUSE16IISR_A14DB(int isr)//2824db
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();
	if (isr != -1 && isr == isr_E3FF8)
	{
		isr_E3FF8 = -1;
	}
}

//----- (000A1520) --------------------------------------------------------
unsigned int sub_A1520()//282520
{
	return x__getcallerseflags();
}

//----- (000A1524) --------------------------------------------------------
void sub_A1524(unsigned int a1)//282524
{
	void* retaddr = nullptr;
	if (BYTE1(retaddr) & 2)
		;
	x__writeeflags(a1);
}

//----- (000A158B) --------------------------------------------------------
int32_t AilApiCallDriver_A158B(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* in, VDI_CALL* out)//28258b
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();
	return ac_sound_call_driver(drvr, fn, out);
}

//----- (000A1665) --------------------------------------------------------
void AilApiDelay_A1665(int delay)//282665
{
	if (CommandLineParams.DoShowDebugPerifery())ShowPerifery();

	mydelay(delay);
}

//----- (000A16A2) --------------------------------------------------------
int GetE3FFE_A16A2()//2826a2
{
	return x_DWORD_E3FEE;
}

//----- (000A1798) --------------------------------------------------------
void sub_A1798()//282798
{
	int v0; // esi
	unsigned __int8 v1; // of

	PlusE3FF2_91BD0();
	v0 = 56;
	do
	{
		v1 = __OFSUB__(v0, 4);
		v0 -= 4;
	} while (!((v0 < 0) ^ v1));
	MinusE3FF2_91BF0();
}

//----- (000A2070) --------------------------------------------------------
void sub_A2070(HDIGDRIVER a1)//283070
{
	VDI_CALL v2;

	if (!a1->playing_21)
	{
		AilCallDriver_91F70(a1->drvr_0, 1025, &v2, nullptr);
		a1->playing_21 = 1;
	}
}

//----- (000A20D0) --------------------------------------------------------
void sub_A20D0(HDIGDRIVER a1)//2830d0
{
	if (a1->playing_21)
	{
		AilCallDriver_91F70(a1->drvr_0, 1026, nullptr, nullptr);
		a1->playing_21 = 0;
	}
}

//----- (000A2110) --------------------------------------------------------
void InitSampleVolume_A2110(HSAMPLE S)//283110
{
	int index1;
	int index2;
	int rightPan;
	int leftPan;
	int volumeA;

	if (S->volume_16 <= 127)
	{
		if (S->volume_16 < 0)
			S->volume_16 = 0;
	}
	else
	{
		S->volume_16 = 127;
	}
	if (S->pan_17 <= 127)
	{
		if (S->pan_17 < 0)
			S->pan_17 = 0;
	}
	else
	{
		S->pan_17 = 127;
	}
	volumeA = S->sam_var[26] * S->pan_17 / 127;
	if (volumeA < 0)
		volumeA = 0;
	if (volumeA > 127)
		volumeA = 127;
	if (S->format_13 != 1 && S->format_13 != 3)
	{//set volume table
		if (volumeA)
			volumeA++;
		if (S->sam_var[6] != 2
			&& S->sam_var[6] != 3
			&& (S->sam_var[6] && S->sam_var[6] != 1 || S->format_13 != 2 && S->format_13 != 3))
		{
			index1 = 0;
			if (S->flags_14 & 1)
			{
				for (int i = 0; i < 32768; i += 256)
				{
					S->vol_scale_18[0][index1] = volumeA * i >> 7;
					index1++;
				}
				for (int i = -32768; i < 0; i += 256)
				{
					S->vol_scale_18[0][index1] = volumeA * i >> 7;
					index1++;
				}
			}
			else
			{
				for (int i = -32768; i < 32768; i += 256)
				{
					S->vol_scale_18[0][index1] = volumeA * i >> 7;
					index1++;
				}
			}
		}
		else
		{
			index2 = 0;
			leftPan = x_BYTE_E4A0C[127 - S->pan_17];
			rightPan = x_BYTE_E4A0C[S->pan_17];
			if (S->flags_14 & 1)
			{
				for (int i = 0; i < 32768; i += 256)
				{
					S->vol_scale_18[0][index2] = leftPan * (volumeA * i >> 7) >> 7;
					S->vol_scale_18[1][index2] = rightPan * (volumeA * i >> 7) >> 7;
					index2++;
				}
				for (int i = -32768; i < 0; i += 256)
				{
					S->vol_scale_18[0][index2] = leftPan * (volumeA * i >> 7) >> 7;
					S->vol_scale_18[1][index2] = rightPan * (volumeA * i >> 7) >> 7;
					index2++;
				}
			}
			else
			{
				for (int i = -32768; i < 32768; i += 256)
				{
					S->vol_scale_18[0][index2] = leftPan * (volumeA * i >> 7) >> 7;
					S->vol_scale_18[1][index2] = rightPan * (volumeA * i >> 7) >> 7;;
					index2++;
				}
			}
		}
	}
	else
	{//set fix volume
		S->vol_scale_18[0][0] = volumeA * x_BYTE_E4A0C[127 - S->pan_17] / 127;
		S->sam_var[274] = volumeA * x_BYTE_E4A0C[S->pan_17] / 127;
	}
}

//----- (000A2650) --------------------------------------------------------
bool sub_A2650(HDIGDRIVER digDriver)//283650
{
	unsigned int memType;
	int memIndex[4];

	if (digDriver->playing_21)
	{
		for (int i = 0; i < digDriver->n_samples_24; i++)
		{
			digDriver->samples_23[i].sam_var[548] = digDriver->samples_23[i].status_1;
			if (digDriver->samples_23[i].status_1 == 4)
				digDriver->samples_23[i].status_1 = 8;
		}
		sub_A20D0(digDriver);
	}
	memType = 2 * preference_181DAC[7] | preference_181DAC[8];
	if (memType <= 3)
	{
		switch (memType)
		{
		case 0u:
			memIndex[0] = 0;
			memIndex[1] = 1;
			memIndex[2] = 2;
			memIndex[3] = 3;
			break;
		case 1u:
			memIndex[0] = 1;
			memIndex[1] = 0;
			memIndex[2] = 3;
			memIndex[3] = 2;
			break;
		case 2u:
			memIndex[0] = 2;
			memIndex[1] = 3;
			memIndex[2] = 0;
			memIndex[3] = 1;
			break;
		case 3u:
			memIndex[0] = 3;
			memIndex[1] = 2;
			memIndex[2] = 1;
			memIndex[3] = 0;
			break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (digDriver->DDT_1->format_supported[memIndex[i]])
		{
			digDriver->hw_format_6 = memIndex[i];
			break;
		}
	}
	digDriver->hw_mode_flags_7 = 1;
	digDriver->DMA_rate_5 = 1;
	switch (digDriver->hw_format_6)
	{
	case 0u:
		digDriver->channels_per_sample_15 = 1;
		digDriver->bytes_per_channel_16 = 1;
		break;
	case 1u:
		digDriver->channels_per_sample_15 = 1;
		digDriver->bytes_per_channel_16 = 2;
		break;
	case 2u:
		digDriver->channels_per_sample_15 = 2;
		digDriver->bytes_per_channel_16 = 1;
		break;
	case 3u:
		digDriver->channels_per_sample_15 = 2;
		digDriver->bytes_per_channel_16 = 2;
		break;
	}
	digDriver->half_buffer_size_4 = 2048;
	digDriver->DST_2->DMA_buffer_A = nullptr;
	digDriver->DST_2->DMA_buffer_B = nullptr;
	if (digDriver->playing_21)
	{
		for (int i = 0; i < digDriver->n_samples_24; i++)
			digDriver->samples_23[i].status_1 = digDriver->samples_23[i].sam_var[548];
		sub_A2070(digDriver);
	}
	return true;
}

int16_t envstring[24] = {
	220,7,01,05,-1,-1,-1,-1,
	-1,-1,-1,-1,220,05,01,05,
	-1,-1,-1,-1,-1,-1,-1,-1
};

//----- (000A2C80) --------------------------------------------------------
int InitEnvs_A2C80(HDIGDRIVER hDigDriver, IO_PARMS*  /*a2*/)//283c80
{
	int16_t envs[24];
	VDI_CALL vdiCall;
	qmemcpy((void*)&(hDigDriver->drvr_0->VHDR_4->IO), envs, 24);
	return AilCallDriver_91F70(hDigDriver->drvr_0, 772, &vdiCall, nullptr);
}

//----- (000A2EA0) --------------------------------------------------------
HDIGDRIVER sub_A2EA0(AIL_DRIVER* ailDriver, IO_PARMS IO)//283ea0
{
	VDI_CALL outCall;
	HDIGDRIVER result;
	uint32_t dmaSeg;
	uint32_t dmaSel;
	uint8_t* dmaBuffer;
	
	bool ioInitited = false;
	HDIGDRIVER digDriver = (_DIG_DRIVER*)malloc(sizeof(_DIG_DRIVER));

	digDriver->hw_format_6 = 0;
	digDriver->hw_mode_flags_7 = 0;

	if (digDriver)
	{
		digDriver->drvr_0 = ailDriver;
		if (digDriver->drvr_0->type_5)
		{
			qmemcpy(textBuffer_181C90, (void*)".DIG driver required\n", 22);
			FreeMem_9D490(digDriver, 140);
			result = 0;
		}
		else
		{
			AilCallDriver_91F70(digDriver->drvr_0, 769, nullptr, &outCall);//354fa8
			digDriver->DDT_1 = (DIG_DDT*)sound_buffer[0];
			digDriver->DST_2 = (DIG_DST*)sound_buffer[1];
			digDriver->buffer_flag_13 = digDriver->DST_2->active_buffer;
			digDriver->last_buffer_14 = -1;
			digDriver->playing_21 = 0;
			digDriver->quiet_22 = 0;
			digDriver->n_active_samples_25 = 0;
			digDriver->master_volume_26 = 127;
			memset(&ioParms_181D90, -1, 24);
			ioInitited = false;
			if (&IO)
			{
				qmemcpy(&ioParms_181D90, (void*)&IO, 24);
				if (InitEnvs_A2C80(digDriver, &IO))
				{
					ioInitited = true;
				}
			}
			if (ioInitited)
			{
				ioParms_181D90 = IO;
				digDriver->DMA_buf_10 = nullptr;

				{
					if (!AllocMem_9D4D0(1,&dmaBuffer, &dmaSel, &dmaSeg))
					{
						qmemcpy(textBuffer_181C90, (void*)"Could not allocate DMA buffers\n", 32);
						FreeMem_9D490(digDriver, 140);
						return 0;
					}
					if (digDriver->DMA_buf_10)
					{
						FreeMem_9D560((uint8_t*)digDriver->DMA_buf_10);
					}
					digDriver->DMA_buf_10 = dmaBuffer;
					digDriver->DMA_sel_9 = dmaSel;
					digDriver->DMA_seg_8 = dmaSeg;
				}
				if (sub_A2650(digDriver))
				{
					AilCallDriver_91F70(digDriver->drvr_0, 773, nullptr, nullptr);
					digDriver->drvr_0->initialized_6 = 1;
					digDriver->samples_per_buffer_18 = digDriver->half_buffer_size_4 / (digDriver->bytes_per_channel_16 * digDriver->channels_per_sample_15);
					digDriver->channels_per_buffer_17 = digDriver->half_buffer_size_4 / digDriver->bytes_per_channel_16;
					digDriver->build_size_19 = 4 * digDriver->channels_per_buffer_17;
					digDriver->build_buffer_20 = (int32_t*)malloc(digDriver->build_size_19);
					if (digDriver->build_buffer_20)
					{
						digDriver->n_samples_24 = preference_181DAC[4];
						digDriver->samples_23 = (_SAMPLE*)malloc(sizeof(_SAMPLE) * digDriver->n_samples_24);
						if (digDriver->samples_23)
						{
							for (int i = 0; i < digDriver->n_samples_24; i++)
							{
								digDriver->samples_23[i].status_1 = 1;
								digDriver->samples_23[i].driver_0 = digDriver;
								digDriver->samples_23[i].index_sample = i;//fixed
							}
							digDriver->timer_3 = 1;
							if (digDriver->timer_3 == -1)
							{
								qmemcpy(textBuffer_181C90, (void*)"Out of timer handles\n", 22);
								if (ailDriver->PM_ISR_7 != -1)
									AilRestoreUSE16ISR_91E90(ailDriver->PM_ISR_7);
								AilCallDriver_91F70(digDriver->drvr_0, 774, nullptr, nullptr);
								digDriver->n_samples_24 = 0;
								FreeMem_9D560((uint8_t*)digDriver->DMA_buf_10);
								FreeMem_9D490(digDriver->samples_23, sizeof(_SAMPLE) * digDriver->n_samples_24);
								FreeMem_9D490(digDriver->build_buffer_20, digDriver->build_size_19);
								FreeMem_9D490(digDriver, 140);
								result = 0;
							}
							else
							{
								result = digDriver;
							}
						}
						else
						{
							qmemcpy(textBuffer_181C90, (void*)"Could not allocate SAMPLE structures\n", 38);
							if (ailDriver->PM_ISR_7 != -1)
								AilRestoreUSE16ISR_91E90(ailDriver->PM_ISR_7);
							AilCallDriver_91F70(digDriver->drvr_0, 774, nullptr, nullptr);
							digDriver->n_samples_24 = 0;
							FreeMem_9D560((uint8_t*)digDriver->DMA_buf_10);
							FreeMem_9D490(digDriver->samples_23, digDriver->build_size_19);
							FreeMem_9D490(digDriver, 140);
							result = 0;
						}
					}
					else
					{
						qmemcpy(textBuffer_181C90, (void*)"Could not allocate build buffer\n", 33);
						if (ailDriver->PM_ISR_7 != -1)
							AilRestoreUSE16ISR_91E90(ailDriver->PM_ISR_7);
						AilCallDriver_91F70(digDriver->drvr_0, 774, nullptr, nullptr);
						digDriver->n_samples_24 = 0;
						FreeMem_9D560((uint8_t*)digDriver->DMA_buf_10);
						FreeMem_9D490(digDriver, 140);
						result = 0;
					}
				}
				else
				{
					FreeMem_9D560((uint8_t*)digDriver->DMA_buf_10);
					FreeMem_9D490(digDriver, 140);
					result = 0;
				}
			}
			else
			{
				qmemcpy(textBuffer_181C90, (void*)"Digital sound hardware not found\n", 34);
				FreeMem_9D490(digDriver, 140);
				result = 0;
			}
		}
	}
	else
	{
		qmemcpy(textBuffer_181C90, (void*)"Could not allocate memory for driver\n", 38);
		result = 0;
	}
	return result;
}

//----- (000A3600) --------------------------------------------------------
HDIGDRIVER AilApiInstallDigDriverFile_A3600(char* filename, IO_PARMS* IO)//284600
{
	long fileLenght;
	HDIGDRIVER result;
	uint8_t* fileBuffer;
	HDIGDRIVER hdigDriver;
	AIL_DRIVER* ailDriver;

	fileBuffer = ReadFile_9DEA0(filename, nullptr);
	if (fileBuffer)
	{
		fileLenght = GetFileLenght_9DE20(filename);
		ailDriver = AilInstallDriver_93010(fileBuffer, fileLenght);
		x_free((void*)fileBuffer);
		if (ailDriver)
		{
			//   IO
			hdigDriver = sub_A2EA0(ailDriver, *IO);
			if (!hdigDriver)
				AilUninstallDriver_93160(ailDriver);
			result = hdigDriver;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		strcpy(textBuffer_181C90, "Driver file not found\n");
		result = 0;
	}
	return result;
}

//----- (000A3720) --------------------------------------------------------
int AilApiInstallDigINI_A3720(HDIGDRIVER* dig, char* digPath)//284720
{
	int result;
	AIL_INI ailIni;
	if (AilReadIniFile_92190(&ailIni, digPath))
	{//3550c8 - SB16.DIG  //355148 - 220
		*dig = AilInstallDigDriverFile_93330(ailIni.driver_path, &ailIni.IO);
		if (*dig)
			result = 0;
		else
			result = 2;
	}
	else
	{
		qmemcpy(textBuffer_181C90, (void*)"Unable to open file DIG.INI\n", 29);
		result = 1;
	}
	return result;
}

//----- (000A37A0) --------------------------------------------------------
int sub_A37A0(HDIGDRIVER a1)
{
	return AilUninstallDriver_93160(a1->drvr_0);
}

//----- (000A3820) --------------------------------------------------------
HSAMPLE sub_A3820_allocate_sample_handle(HDIGDRIVER dig)//284820
{
	HSAMPLE hSample1;
	HSAMPLE result;
	int i;

	PlusE3FF2_91BD0();
	for (i = 0; i < dig->n_samples_24 && dig->samples_23[i].status_1 != 1; i++)
		;
	if (i == dig->n_samples_24)
	{
		qmemcpy(textBuffer_181C90, (void*)"Out of sample handles\n", 23);
		MinusE3FF2_91BF0();
		result = 0;
	}
	else
	{
		hSample1 = &dig->samples_23[i];
		AilInitSample_93830(hSample1);
		MinusE3FF2_91BF0();
		result = hSample1;
	}
	return result;
}

//----- (000A38E0) --------------------------------------------------------
void InitSample_A38E0(HSAMPLE S)//2848e0
{
	if (S)
	{
		S->status_1 = 2;
		S->start_2_3[0] = 0;
		S->start_2_3[1] = 0;
		S->len_4_5[0] = 0;
		S->len_4_5[1] = 0;
		S->pos_6_7[0] = 0;
		S->pos_6_7[1] = 0;
		S->done_8_9[0] = 0;
		S->done_8_9[1] = 1;
		S->current_buffer_10 = 0;
		S->last_buffer_11 = -2;
		S->loop_count_12 = 1;
		S->format_13 = 0;
		S->flags_14 = 0;
		S->playback_rate_15 = 11025;
		S->volume_16 = preference_181DAC[5];
		if (S->pos_6_7[0] && S->pos_6_7[0] != 1)
			S->pan_17 = 64;
		else
			S->pan_17 = 0;
		S->sam_var[530] = 0;
		S->sam_var[531] = 0;
		S->sam_var532_EOS_ptr = 0;
		InitSampleVolume_A2110(S);
	}
}

void InitHqsound(HSAMPLE S) {
	//test mark
	bool same_mark = true;
	int comparesize = sample_mark;
	if (comparesize > S->len_4_5[0])comparesize = S->len_4_5[0];
	for (int i = 0; i < comparesize; i++)
	{
		if (S->mark44mark[i] != ((uint8_t*)S->start_2_3[0])[i])
		{
			same_mark = false;
			break;
		}
	}
	//test mark

	if (!same_mark)
	{
		for (int i = 0; i < comparesize; i++)
		{
			S->mark44mark[i] = ((uint8_t*)S->start_2_3[0])[i];
		}

		if (S->start_44mhz != nullptr)
			free(S->start_44mhz);
		if (fixspeedsound)
			S->start_44mhz = malloc(S->len_4_5[0] * 2 * 2 * 2 * 2);
		else
			S->start_44mhz = malloc(S->len_4_5[0] * 2 * 2 * 2);

		uint16_t lastval = ((uint8_t*)S->start_2_3[0])[0] * 256;
		uint16_t actval;
		int16_t val1, val3;
		for (int i = 0; i < S->len_4_5[0]; i++)
		{
			actval = ((uint8_t*)S->start_2_3[0])[i] * 256;
			val1 = lastval - 0x8000;
			val3 = (lastval * 0.5 + actval * 0.5) - 0x8000;
			if (fixspeedsound) {
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[0 + i * 16]) = val1;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[2 + i * 16]) = val1;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[4 + i * 16]) = val1;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[6 + i * 16]) = val1;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[8 + i * 16]) = val3;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[10 + i * 16]) = val3;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[12 + i * 16]) = val3;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[14 + i * 16]) = val3;
			}
			else {
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[0 + i * 8]) = val1;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[2 + i * 8]) = val1;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[4 + i * 8]) = val3;
				(*(int16_t*)&((uint8_t*)S->start_44mhz)[6 + i * 8]) = val3;
			}
			if (i < S->len_4_5[0] + 1)lastval = actval;
		}
		lastval = actval;
	}
};

//----- (000A3A30) --------------------------------------------------------
void SetSampleAddress_A3A30(HSAMPLE S, uint8_t* start, uint32_t len)
{
	if (S)
	{
		S->start_2_3[0] = start;
		S->start_2_3[1] = 0;
		S->len_4_5[0] = len;
		S->len_4_5[1] = 0;
		if (hqsound)
			InitHqsound(S);
	}
}

//----- (000A3A70) --------------------------------------------------------
void SetSampleType_A3A70(HSAMPLE S, int32_t channels, uint32_t byteSampleType)//284a70
{
	if (S)
	{
		if (channels != S->format_13 || (byteSampleType != S->flags_14))
		{
			S->format_13 = channels;
			S->flags_14 = byteSampleType;
			InitSampleVolume_A2110(S);
		}
	}
}

//----- (000A3AC0) --------------------------------------------------------
int32_t GetSamplePlaybackRate_A3AC0(HSAMPLE S)//284ac0
{
	int32_t result;
	if (S)
		result = S->playback_rate_15;
	else
		result = 0;
	return result;
}

//----- (000A3AF0) --------------------------------------------------------
void SetSamplePlaybackRate_A3AF0(HSAMPLE S, int32_t playback_rate)//284af0
{
	if (S)
	{
		S->playback_rate_15 = playback_rate;
	}
}

//----- (000A3B10) --------------------------------------------------------
int32_t GetSampleVolume_A3B10(HSAMPLE S)//284b10
{
	int32_t result;

	if (S)
		result = S->volume_16;
	else
		result = 0;
	return result;
}

//----- (000A3B40) --------------------------------------------------------
void SetSampleVolume_A3B40(HSAMPLE S, int32_t volume) //284b40
{
	if (S)
	{
		if (volume != S->volume_16)
		{
			S->volume_16 = volume;
			InitSampleVolume_A2110(S);
		}
	}
}

//----- (000A3B80) --------------------------------------------------------
int32_t GetSampleVolumePan_A3B80(HSAMPLE S)//284b80
{
	int32_t result;

	if (S)
		result = S->pan_17;
	else
		result = 0;
	return result;
}

//----- (000A3BB0) --------------------------------------------------------
void SetSampleVolumePan_A3BB0(HSAMPLE S, int32_t volume)//284bb0
{
	if (S)
	{
		if (volume != S->pan_17)
		{
			S->pan_17 = volume;
			InitSampleVolume_A2110(S);
		}
	}
}

//----- (000A3C90) --------------------------------------------------------
void SetSampleLoopCount_A3C90(HSAMPLE S, int loop_count)//284c90
{
	if (S)
	{
		S->loop_count_12 = loop_count;
	}
}

void ApiAilStartSample_A3CB0(HSAMPLE S)//284cb0
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			S->status_1 = 4;
			SOUND_start_sample(S);
		}
	}
}

void ApiAilEndSample_A3DA0(HSAMPLE S)//284da0
{
	if (S)
	{
		if (S->status_1 != 1)
		{
			if (S->status_1 != 2)
			{
				S->status_1 = 2;
				SOUND_end_sample(S);
			}
		}
	}
}

//----- (000A3EB0) --------------------------------------------------------
EOS_callback_type RegisterEosCallback_A3EB0(HSAMPLE S, EOS_callback_type EOS)//284eb0
{
	EOS_callback_type result; // ST04_4
	if (!S)
		return 0;
	result = S->sam_var532_EOS_ptr;
	S->sam_var532_EOS_ptr = EOS;
	return result;
}

//----- (000A3F00) --------------------------------------------------------
void SetSampleUserData_A3F00(HSAMPLE S, int index, int  /*a3*/)//284f00
{
	//int result; // eax

	if (S)
	{
		//result = a3;
		//*(x_DWORD *)(4 * a2 + S + 2132) = a3;
		//S->sam_var[533 + index];
	}
	//return result;
}

//----- (000A3F30) --------------------------------------------------------
int SampleUserData_A3F30(HSAMPLE S, int index)//284f30
{
	int v3; // [esp+0h] [ebp-4h]

	if (S)
		v3 = S->sam_var[533 + index];
	else
		v3 = 0;
	return v3;
}

//----- (000A3F70) --------------------------------------------------------
void SetDigitalMasterVolume_A3F70(HDIGDRIVER driver, int a2)//284f70
{
	driver->master_volume_26 = a2;
	for (int i = 0;i< driver->n_samples_24; i++)
	{		
		if (driver->samples_23[i].len_4_5[0] != 1)
			InitSampleVolume_A2110(&driver->samples_23[i]);
	}
}

//----- (000A43E0) --------------------------------------------------------
void sub_A43E0(HSAMPLE sample)//2853e0
{
}

//----- (000A47C0) --------------------------------------------------------
void SubSetSampleFile_A47C0(uint8_t* pWaveData, HSAMPLE S)//2857c0
{
	int index = 12;

	for (int i = _strnicmp((const char*)&pWaveData[12], "fmt ", 4); i; i = _strnicmp((const char*)&pWaveData[index], "fmt ", 4))
		index += (*(int32_t*)&pWaveData[index +4] & 1) + *(int32_t*)&pWaveData[index + 4] + 8;

	int16_t numChannels = *(int16_t*)&pWaveData[index + 10];
	int16_t bytesPerSample = *(int16_t*)&pWaveData[index + 22];
	int32_t sampleRate = *(int32_t*)&pWaveData[index + 12];

	if (numChannels == 1 && bytesPerSample == 8)
	{
		AilSetSampleType_93AB0(S, 0, 0);
	}
	else if (numChannels == 2 && bytesPerSample == 8)
	{
		AilSetSampleType_93AB0(S, 2, 0);
	}
	else if (numChannels == 1 && bytesPerSample == 16)
	{
		AilSetSampleType_93AB0(S, 1, 1);
	}
	else if (numChannels == 2 && bytesPerSample == 16)
	{
		AilSetSampleType_93AB0(S, 3, 1);
	}
	AilSetSamplePlaybackRate_93D90(S, sampleRate);

	index = 12;
	for (int j = _strnicmp((const char*)&pWaveData[12], "data", 4); j; j = _strnicmp((const char*)&pWaveData[index], "data", 4))
		index += (*(int32_t*)&pWaveData[index + 4] & 1) + *(int32_t*)&pWaveData[index + 4] + 8;

	AilSetSampleAddress_93A10(S, &pWaveData[index + 8], *(int32_t*)&pWaveData[index + 4]);
}

//----- (000A4B20) --------------------------------------------------------
bool SetSampleFile_A4B20(HSAMPLE S, uint8_t* pWavData, int32_t block)//285b20
{
	if (pWavData != NULL)
	{
		bool soundStringOk;
		if (!_strnicmp((const char*)&pWavData[0], "Creative", 8))
		{
			soundStringOk = false;
		}
		else
		{
			if (_strnicmp((const char*)&pWavData[8], "WAVE", 4))
			{
				qmemcpy(textBuffer_181C90, (void*)"Unrecognized digital audio file type\n", 38);
				return false;
			}
			soundStringOk = true;
		}
		if (soundStringOk)
		{
			if (soundStringOk)
			{
				S->sam_var[547] = 0;
				SubSetSampleFile_A47C0(pWavData, S);
			}
		}
		else
		{
			S->sam_var542 = &pWavData[*(uint16_t*)&pWavData[20]];
			S->sam_var[545] = block;
			S->sam_var[546] = block == -1;
			S->sam_var[547] = 0;
			sub_A43E0(S);
		}
		if (S->sam_var[547] != -1)
			return true;
		strcpy(textBuffer_181C90, "Invalid or missing data block\n");
	}
	return false;
}

//----- (000A4E10) --------------------------------------------------------
signed int sub_A4E10(char input)//285e10
{
	switch (input & 0xF0)
	{
		case 0x0:
		case 0x10:
		case 0x20:
		case 0x30:
		case 0x40:
		case 0x50:
		case 0x60:
		case 0x70:
		case 0xF0:
			return 0;
		case 0xD0:
			return 2;
	}
	return 3;
}

//----- (000A4EB0) --------------------------------------------------------
void sub_A4EB0(HMDIDRIVER hMdiDriver)//285eb0
{
	VDI_CALL vdiCall;
	if (hMdiDriver->var106_aildrv > 0)
	{
		AilCallDriver_91F70(hMdiDriver->drvr_0, 1282, &vdiCall, nullptr);
		hMdiDriver->var106_aildrv = 0;
		hMdiDriver->var107_aildrv = 0;
	}
}

//----- (000A4F10) --------------------------------------------------------
void sub_A4F10(HMDIDRIVER hMdiDriver, char par1, char par2, char par3)//285f10
{
	int locPar4 = sub_A4E10(par1);
	if (locPar4 + hMdiDriver->var107_aildrv > 0x200)
		sub_A4EB0(hMdiDriver);
	hMdiDriver->var106_aildrv++;
	hMdiDriver->var107_aildrv++;
}

//----- (000A4FD0) --------------------------------------------------------
void sub_A4FD0(HMDIDRIVER a1, int  /*a2*/, unsigned int a3)//285fd0
{
	sub_A4EB0(a1);
	if (a3 <= 0x200)
		;
	else
		;
	sub_A4EB0(a1);
}

//----- (000A50A0) --------------------------------------------------------
unsigned int sub_A50A0(int a1)
{
	return ((a1 & 0xFF000000) >> 24)
		+ ((a1 & 0xFF0000u) >> 8)
		+ ((unsigned __int16)(a1 & 0xFF00) << 8)
		+ ((unsigned __int8)a1 << 24);
}

//----- (000A50F0) --------------------------------------------------------
uint8_t* sub_A50F0(uint8_t* a1, int a2)
{
	uint8_t* v3; // [esp+0h] [ebp-Ch]
	uint8_t* v4; // [esp+4h] [ebp-8h]
	unsigned int v5; // [esp+8h] [ebp-4h]
	uint8_t* i; // [esp+1Ch] [ebp+10h]

	v5 = 0;
	do
	{
		a1 += v5;
		if (strncmp((const char*)a1, "FORM", 4) && strncmp((const char*)a1, "CAT ", 4))
			return 0;
		v5 = sub_A50A0(*(x_DWORD*)(a1 + 4)) + 8;
	} while (strncmp((const char*)(a1 + 8), "XMID", 4));
	if (!strncmp((const char*)a1, "FORM", 4))
	{
		if (a2)
			v3 = 0;
		else
			v3 = a1;
	}
	else
	{
		v4 = v5 + a1;
		for (i = a1 + 12; i < v4; i += sub_A50A0(*(x_DWORD*)(i + 4)) + 8)
		{
			if (!strncmp((const char*)(i + 8), "XMID", 4) && --a2 == -1)
				return i;
		}
		v3 = 0;
	}
	return v3;
}
// 99682: using guessed type x_DWORD strncmp(x_DWORD, x_DWORD, x_DWORD);

//----- (000A5210) --------------------------------------------------------
int sub_A5210(int a1, char a2, unsigned int a3)
{
	int v4; // [esp+0h] [ebp-Ch]
	int v5; // [esp+4h] [ebp-8h]
	unsigned int v6; // [esp+8h] [ebp-4h]

	v6 = a2 & 0xF0;
	v5 = a2 & 0xF;
	if (v6 < 0xC0)
	{
		if (v6 != 176)
			return -1;
		if (a3 < 0x5D)
		{
			if (a3 < 0xA)
			{
				if (a3 < 6)
				{
					if (a3 == 1)
						return *(x_DWORD*)(a1 + 4 * v5 + 640);
				}
				else
				{
					if (a3 <= 6)
						return *(x_DWORD*)(a1 + 4 * v5 + 1088);
					if (a3 == 7)
						return *(x_DWORD*)(a1 + 4 * v5 + 704);
				}
			}
			else
			{
				if (a3 <= 0xA)
					return *(x_DWORD*)(a1 + 4 * v5 + 768);
				if (a3 < 0x40)
				{
					if (a3 == 11)
						return *(x_DWORD*)(a1 + 4 * v5 + 832);
				}
				else
				{
					if (a3 <= 0x40)
						return *(x_DWORD*)(a1 + 4 * v5 + 896);
					if (a3 == 91)
						return *(x_DWORD*)(a1 + 4 * v5 + 960);
				}
			}
		}
		else
		{
			if (a3 <= 0x5D)
				return *(x_DWORD*)(a1 + 4 * v5 + 1024);
			if (a3 < 0x70)
			{
				if (a3 >= 0x6E)
				{
					if (a3 <= 0x6E)
						v4 = *(x_DWORD*)(a1 + 4 * v5 + 192);
					else
						v4 = *(x_DWORD*)(a1 + 4 * v5 + 256);
					return v4;
				}
				if (a3 == 107)
					return *(x_DWORD*)(a1 + 4 * v5 + 320);
			}
			else
			{
				if (a3 <= 0x70)
					return *(x_DWORD*)(a1 + 4 * v5 + 384);
				if (a3 < 0x73)
				{
					if (a3 == 114)
						return *(x_DWORD*)(a1 + 4 * v5 + 448);
				}
				else
				{
					if (a3 <= 0x73)
						return *(x_DWORD*)(a1 + 4 * v5 + 512);
					if (a3 == 119)
						return *(x_DWORD*)(a1 + 4 * v5 + 576);
				}
			}
		}
		return -1;
	}
	if (v6 <= 0xC0)
		return *(x_DWORD*)(a1 + 4 * v5);
	if (v6 != 224)
		return -1;
	return (*(x_DWORD*)(a1 + 4 * v5 + 128) << 7) | *(x_DWORD*)(a1 + 4 * v5 + 64);
}

//----- (000A5530) --------------------------------------------------------
void sub_A5530(CTRL_LOG* a1x, char a2, unsigned int a3, unsigned __int8 a4)
{
	x_DWORD* result;
	int v5;
	unsigned int v6;

	v6 = a2 & 0xF0;
	result = (x_DWORD*)(a2 & 0xF);
	v5 = a2 & 0xF;
	if (v6 < 0xC0)
	{
		if (v6 == 176)
		{
			if (a3 < 0x5D)
			{
				if (a3 < 0xA)
				{
					if (a3 < 6)
					{
						if (a3 == 1)
						{
							result = &a1x->indirect[v5];
							result[160] = a4;
						}
					}
					else if (a3 <= 6)
					{
						result = &a1x->indirect[v5];
						result[272] = a4;
					}
					else if (a3 == 7)
					{
						result = &a1x->indirect[v5];
						result[176] = a4;
					}
				}
				else if (a3 <= 0xA)
				{
					result = &a1x->indirect[v5];
					result[192] = a4;
				}
				else if (a3 < 0x40)
				{
					if (a3 == 11)
					{
						result = &a1x->indirect[v5];
						result[208] = a4;
					}
				}
				else if (a3 <= 0x40)
				{
					result = &a1x->indirect[v5];
					result[224] = a4;
				}
				else if (a3 == 91)
				{
					result = &a1x->indirect[v5];
					result[240] = a4;
				}
			}
			else if (a3 <= 0x5D)
			{
				result = &a1x->indirect[v5];
				result[256] = a4;
			}
			else if (a3 < 0x70)
			{
				if (a3 < 0x6E)
				{
					if (a3 == 107)
					{
						result = &a1x->indirect[v5];
						result[80] = a4;
					}
				}
				else if (a3 <= 0x6E)
				{
					result = &a1x->indirect[v5];
					result[48] = a4;
				}
				else
				{
					result = &a1x->indirect[v5];
					result[64] = a4;
				}
			}
			else if (a3 <= 0x70)
			{
				result = &a1x->indirect[v5];
				result[96] = a4;
			}
			else if (a3 < 0x73)
			{
				if (a3 == 114)
				{
					result = &a1x->indirect[v5];
					result[112] = a4;
				}
			}
			else if (a3 <= 0x73)
			{
				result = &a1x->indirect[v5];
				result[128] = a4;
			}
			else if (a3 == 119)
			{
				result = &a1x->indirect[v5];
				result[144] = a4;
			}
		}
	}
	else if (v6 <= 0xC0)
	{
		result = &a1x->indirect[v5];
		*result = (unsigned __int8)a3;
	}
	else if (v6 == 224)
	{
		a1x->indirect[v5 + 8] = (unsigned __int8)a3;
		result = &a1x->indirect[v5];
		result[32] = a4;
	}
}

//----- (000A5850) --------------------------------------------------------
void sub_A5850(HSEQUENCE hSequence, char a2, unsigned int a3, signed int a4, int a5)//286850
{
	HMDIDRIVER mdiDriver;
	int indexCh;
	int chMap;
	signed int i;
	signed int j;
	int v12;
	int indexChH;

	mdiDriver = hSequence->driver_0;
	indexChH = a2 & 0xF0;
	indexCh = a2 & 0xF;
	chMap = hSequence->chan_map_37[indexCh];
	if (indexChH == 176 || indexChH == 192 || indexChH == 224)
		sub_A5530(&hSequence->shadow_53, indexCh | indexChH, a3, a4);
	if (indexChH != 176)
		goto LABEL_79;
	if (a5)
	{
		if (hSequence->shadow_53.indirect[indexCh] != -1)
		{
			a4 = hSequence->shadow_53.indirect[indexCh];
			hSequence->shadow_53.indirect[indexCh] = -1;
		}
	}
	if (a3 < 0x6F)
	{
		if (a3 < 0x6C)
		{
			if (a3 >= 6)
			{
				if (a3 <= 6)
				{//6
					sub_A5850(hSequence, indexCh | 0xB0, 0x64u, 0, 0);
					sub_A5850(hSequence, indexCh | 0xB0, 0x65u, 0, 0);
					sub_A5850(hSequence, indexCh | 0xB0, 0x26u, 0, 0);
				}
				else if (a3 == 7)
				{//7
					a4 = hSequence->volume_14 * a4 * mdiDriver->var108_aildrv / 16129;
					if (a4 > 127)
						a4 = 127;
					if (a4 < 0)
						a4 = 0;
				}
			}
			goto LABEL_79;
		}
		if (a3 <= 0x6C)
		{//6c
			if (hSequence->prefix_callback_7)
				hSequence->shadow_53.indirect[indexCh] = (*(int(**)(HSEQUENCE, int, signed int))(hSequence->prefix_callback_7))(hSequence, indexCh, a4);
			goto LABEL_79;
		}
		if (a3 <= 0x6D)
		{//6d
			AilBranchIndex_97480(hSequence, a4);
		}
		else if (a4 < 64)
		{//
			if (mdiDriver->lock[chMap] == 1)
			{
				sub_A5F30(hSequence, indexCh);
				AilReleaseChannel_980D0(mdiDriver, chMap + 1);
				AilMapSequenceChannel_98170(hSequence, indexCh + 1, indexCh + 1);
			}
		}
		else
		{
			if (mdiDriver->lock[chMap] != 1)
			{
				v12 = AilLockChannel_97F90(mdiDriver);
				if (v12)
				{
					AilMapSequenceChannel_98170(hSequence, indexCh + 1, v12);
					mdiDriver->var23_aildrvx[hSequence->driver_0->drvr_0->seg_0] = hSequence;
				}
			}
		}
	}
	else if (a3 <= 0x6F)
	{//6f
		if (mdiDriver->lock[chMap] != 1)
		{
			if (a4 >= 64)
			{
				mdiDriver->lock[chMap] = 2;
			}
			else
			{
				mdiDriver->lock[chMap] = 0;
			}
		}
	}
	else if (a3 < 0x75)
	{
		if (a3 < 0x73)
			goto LABEL_79;
		if (a3 <= 0x73)
		{//73
			*(x_DWORD*)(hSequence + 4 * indexCh + 724) = *(unsigned __int8*)(a4 + *(x_DWORD*)(hSequence + 24));
		LABEL_79://end
			if (mdiDriver->lock[chMap] != 1 || mdiDriver->locker[chMap] == hSequence)
			{
				if (indexChH == 144)
				{
					mdiDriver->notes[chMap]++;
				}
				else if (indexChH == 128)
				{
					mdiDriver->notes[chMap]--;
				}
				mdiDriver->user[chMap] = hSequence;
				if (indexChH != 144 || hSequence->shadow_53.c_mute[indexCh] < 64)
				{
					if (!mdiDriver->event_trap
						|| ((HMDIDRIVER)(x_DWORD*)((int(*)(HMDIDRIVER, HSEQUENCE, int, unsigned int, signed int))mdiDriver->event_trap)(
							mdiDriver,
							hSequence,
							chMap | indexChH,
							a3,
							(int)a4)) == 0)
					{
						sub_A4F10(mdiDriver, chMap | indexChH, a3, a4);
					}
				}
			}
			return;
		}
		for (i = 0; i < 4; i++)
		{
			if (hSequence->FOR_loop_count_33[i] == -1)
				break;
		}
		if (i != 4)
		{
			hSequence->FOR_loop_count_33[i] = a4;
			hSequence->FOR_ptrs[i] = hSequence->EVNT_ptr_5;
		}
	}
	else if (a3 <= 0x75)
	{//75
		if (a4 >= 64)
		{
			for (j = 3; j >= 0; j--)
			{
				if (hSequence->FOR_loop_count_33[j] != -1)
					break;
			}
			if (j != -1)
			{
				if (hSequence->FOR_loop_count_33[j])
				{
					if (--hSequence->FOR_loop_count_33[j])
					{
						hSequence->EVNT_ptr_5 = hSequence->FOR_ptrs[j];
					}
					else
					{
						hSequence->FOR_loop_count_33[j] = -1;
					}
				}
				else
				{
					hSequence->EVNT_ptr_5 = (uint8_t*)hSequence->FOR_loop_count_33[j];
				}
			}
		}
	}
	else if (a3 <= 0x76)
	{//76
		*(x_DWORD*)(hSequence + 92) = 0;
		*(x_DWORD*)(hSequence + 96) = 0;
		*(x_DWORD*)(hSequence + 108) = 0;
		*(x_DWORD*)(hSequence + 108) -= *(x_DWORD*)(hSequence + 104);
		if (hSequence->beat_callback_9)
			(*(int(**)(HMDIDRIVER, HSEQUENCE, x_DWORD, x_DWORD))(hSequence->beat_callback_9))(mdiDriver, hSequence, 0, 0);
	}
	else
	{
		if (a3 != 119)
			goto LABEL_79;
		if (hSequence->trigger_callback_8_32)
			(*(int(**)(HSEQUENCE, int, signed int))(hSequence->trigger_callback_8_32))(hSequence, indexCh, a4);
	}
}

//----- (000A5E50) --------------------------------------------------------
void sub_A5E50(HSEQUENCE hSequence)
{
	int v1;
	int v3;

	v3 = 0;
	for (int i = 0; i < 32; i++)
	{
		if (hSequence->seq_342[i] != -1)
		{
			sub_A5850(hSequence, hSequence->seq_342[i] | 0x80, hSequence->seq_374[i], 0, 0);
			hSequence->seq_342[i] = -1;
			v3++;
		}
	}
	hSequence->seq_341 = 0;
	sub_A4EB0((HMDIDRIVER)hSequence->driver_0);
	if (v3)
	{
		v1 = sub_92160();
		if (!v1)
			AilDelay_920D0(3);
	}
}

//----- (000A5F30) --------------------------------------------------------
void sub_A5F30(HSEQUENCE hSequence, int a2)
{
	for (int i = 0; i < 32; i++)
	{
		if (hSequence->note_chan[i] == a2)
		{
			sub_A5850(hSequence, hSequence->note_chan[i] | 0x80, hSequence->note_num[i], 0, 0);
			hSequence->note_chan[i] = -1;
		}
	}
	return sub_A4EB0(hSequence->driver_0);
}

//----- (000A5FD0) --------------------------------------------------------
void sub_A5FD0(HSEQUENCE hSequence, int a2)
{
}

//----- (000A6370) --------------------------------------------------------
void sub_A6370(HSEQUENCE hSequence)//287370
{
	for (int i = 0; i < 16; i++)
		hSequence->chan_map_37[i] = i;
	memset((void*)&hSequence->shadow_53, -1, sizeof(CTRL_LOG));
	for (int i = 0; i < 4; i++)
		hSequence->FOR_loop_count_33[i] = -1;
	for (int i = 0; i < 32; i++)
		hSequence->seq_342[i] = -1;
	hSequence->seq_341 = 0;
	hSequence->interval_count_12 = 0;
	hSequence->beat_count_23 = 0;
	hSequence->measure_count_24 = -1;
	hSequence->beat_fraction_27 = 0;
	hSequence->time_fraction_26 = 0;
	hSequence->time_numerator_25 = 4;
	hSequence->time_per_beat_28 = 8000000;
	hSequence->interval_num_13 = 0;
}

//----- (000A6490) --------------------------------------------------------
void sub_A6490(HSEQUENCE hSequence)
{
	sub_A6370(hSequence);
	hSequence->EVNT_ptr_5 = (uint8_t*)hSequence->EVNT_4 + 8;
}

//----- (000A64C0) --------------------------------------------------------
void sub_A64C0(HSEQUENCE hSequence)
{
	for (int32_t i = 0; i < 16; i++)
	{
		if (hSequence->shadow_53.pan[i] != -1)
			sub_A5850(hSequence, i | 0xB0, 7u, hSequence->shadow_53.pan[i], 0);
	}
}

//----- (000A6E00) --------------------------------------------------------
int SetIoParameters_A6E00(HMDIDRIVER hMdiDriver, IO_PARMS* ioParms)//287e00
{
	IO_PARMS locIoParms;
	IO_PARMS preIoParms;

	qmemcpy(&locIoParms, (void*)ioParms, sizeof(IO_PARMS));
	preIoParms.IO = 0x220;
	preIoParms.IRQ = -1;
	preIoParms.DMA_8_bit = -1;
	preIoParms.DMA_16_bit = -1;
	preIoParms.IO_reserved[0] = -1;
	preIoParms.IO_reserved[1] = -1;
	preIoParms.IO_reserved[2] = -1;
	preIoParms.IO_reserved[3] = -1;
	if (locIoParms.IO < 1)
		locIoParms.IO = preIoParms.IO;
	if (locIoParms.IRQ < 1)
		locIoParms.IRQ = preIoParms.IRQ;
	if (locIoParms.DMA_8_bit < 1)
		locIoParms.DMA_8_bit = preIoParms.DMA_8_bit;
	if (locIoParms.DMA_16_bit < 1)
		locIoParms.DMA_16_bit = preIoParms.DMA_16_bit;
	for (int i = 0; i < 4; i++)
	{
		if (locIoParms.IO_reserved[i] < 1)
			locIoParms.IO_reserved[i] = preIoParms.IO_reserved[i];
	}
	qmemcpy((void*)&hMdiDriver->drvr_0->VHDR_4->IO, &locIoParms, sizeof(IO_PARMS));
	return AilCallDriver_91F70(hMdiDriver->drvr_0, 772, nullptr, nullptr);
}

//----- (000A6FB0) --------------------------------------------------------
HMDIDRIVER InitAilDriver_A6FB0(AIL_DRIVER* ailDriver, IO_PARMS* io)//287fb0
{
	IO_PARMS ioParams;
	VDI_CALL vdiOut;
	const char* oplExt = ".OPL\0";
	bool ioIsSet;

	IO_PARMS* ioParams2 = nullptr;
	HMDIDRIVER result = nullptr;

	HMDIDRIVER hMdiDriver = (HMDIDRIVER)malloc(sizeof(_MDI_DRIVER));
	if (hMdiDriver)
	{
		hMdiDriver->drvr_0 = ailDriver;
		if (hMdiDriver->drvr_0->type_5 == 1)
		{
			AilCallDriver_91F70(hMdiDriver->drvr_0, 769, nullptr, &vdiOut);
			hMdiDriver->DST_2 = (MDI_DST*)sound_buffer[0];
			
			if (oplExt && oplExt[0])
			{
				hMdiDriver->DST_2->GTL_filename = aSamplesPath + std::string((const char*)oplExt);
			}			
			memset(&ioParms_181D90, -1, sizeof(IO_PARMS));
			ioIsSet = false;
			if (io)
			{
				qmemcpy(&ioParms_181D90, (void*)io, sizeof(IO_PARMS));
				if (SetIoParameters_A6E00(hMdiDriver, io))
				{
					ioIsSet = true;
					qmemcpy(&ioParams, (void*)io, sizeof(IO_PARMS));
				}
			}
			if (ioIsSet)
			{
				qmemcpy(&ioParms_181D90, &ioParams, sizeof(IO_PARMS));
				AilCallDriver_91F70(hMdiDriver->drvr_0, 773, nullptr, nullptr);
				hMdiDriver->drvr_0->initialized_6 = 1;
				AilCallDriver_91F70(hMdiDriver->drvr_0, 1281, nullptr, &vdiOut);
				if (vdiOut.AX)
				{
					hMdiDriver->n_sequences_7 = preference_181DAC[12];
					hMdiDriver->sequences_6 = (HSEQUENCE)malloc(sizeof(SEQUENCE) * hMdiDriver->n_sequences_7);
					if (hMdiDriver->sequences_6)
					{
						for (int i = 0; i < hMdiDriver->n_sequences_7; i++)
						{
							hMdiDriver->sequences_6[i].status_1 = 1;
							hMdiDriver->sequences_6[i].driver_0 = hMdiDriver;
						}
						hMdiDriver->var104_aildrv = 0;
						hMdiDriver->var105_aildrv = 0;
						hMdiDriver->var106_aildrv = 0;
						hMdiDriver->var107_aildrv = 0;
						hMdiDriver->interval_time_4 = 1000000 / preference_181DAC[11];
						hMdiDriver->disable_5 = 0;
						hMdiDriver->var108_aildrv = 127;
						hMdiDriver->timer_3 = 1;
						hMdiDriver->drvr_0->destructor_9 = sub_A6F30;
						hMdiDriver->drvr_0->descriptor_10 = hMdiDriver;
						for (int i = 0; i < 16; i++)
						{
							sub_A4F10(hMdiDriver, i | 0xB0, 114, 0);
							sub_A4F10(hMdiDriver, i | 0xC0, 0, 0);
							sub_A4F10(hMdiDriver, i | 0xE0, 0, 64);
							sub_A4F10(hMdiDriver, i | 0xB0, 112, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 1, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 7, preference_181DAC[13]);
							sub_A4F10(hMdiDriver, i | 0xB0, 10, 64);
							sub_A4F10(hMdiDriver, i | 0xB0, 11, 127);
							sub_A4F10(hMdiDriver, i | 0xB0, 64, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 91, 40);
							sub_A4F10(hMdiDriver, i | 0xB0, 93, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 100, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 101, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 38, 0);
							sub_A4F10(hMdiDriver, i | 0xB0, 6, preference_181DAC[16]);
							sub_A4EB0(hMdiDriver);
							if (!(i & 3))
								AilDelay_920D0(3);
						}
						result = hMdiDriver;
					}
					else
					{
						qmemcpy(textBuffer_181C90, (void*)"Could not allocate SEQUENCE structures\n", 40);
						AilCallDriver_91F70(hMdiDriver->drvr_0, 774, nullptr, nullptr);
						hMdiDriver->drvr_0->initialized_6 = 0;
						FreeMem_9D490(hMdiDriver, 468);
						result = nullptr;
					}
				}
				else
				{
					qmemcpy(textBuffer_181C90, (void*)"Could not initialize instrument manager\n", 41);
					AilCallDriver_91F70(hMdiDriver->drvr_0, 774, nullptr, nullptr);
					hMdiDriver->drvr_0->initialized_6 = 0;
					FreeMem_9D490(hMdiDriver, 468);
					result = nullptr;
				}
			}
			else
			{
				qmemcpy(textBuffer_181C90, (void*)"XMIDI sound hardware not found\n", 32);
				FreeMem_9D490(hMdiDriver, 468);
				result = nullptr;
			}
		}
		else
		{
			qmemcpy(textBuffer_181C90, (void*)".MDI driver required\n", 24);
			FreeMem_9D490(hMdiDriver, 468);
			result = nullptr;
		}
	}
	else
	{
		qmemcpy(textBuffer_181C90, (void*)"Could not allocate memory for driver\n", 38);
		result = nullptr;
	}

	return result;
}

//----- (000A77D0) --------------------------------------------------------
HMDIDRIVER AilApiInstallMDIIni_A77D0(char* filename, IO_PARMS* IO)//2887d0
{
	HMDIDRIVER result;
	HMDIDRIVER hMdiDriver;
	AIL_DRIVER* ailDriver;

	uint8_t* fileBuffer = ReadFile_9DEA0(filename, nullptr);
	if (fileBuffer)
	{
		int fileLenght = GetFileLenght_9DE20(filename);
		ailDriver = AilInstallDriver_93010(fileBuffer, fileLenght);
		x_free((void*)fileBuffer);
		if (ailDriver)
		{
			hMdiDriver = InitAilDriver_A6FB0(ailDriver, IO);
			if (!hMdiDriver)
				AilUninstallDriver_93160(ailDriver);
			result = hMdiDriver;
		}
		else
		{
			result = nullptr;
		}
	}
	else
	{
		strcpy(textBuffer_181C90, "Driver file not found\n");
		result = nullptr;
	}
	return result;
}

//----- (000A78F0) --------------------------------------------------------
int32_t AilApiInstallMDIIni_A78F0(HMDIDRIVER* mdi, char* fileName) //2888f0
{
	AIL_INI ailIni;
	int result;
	if (AilReadIniFile_92190(&ailIni, fileName))
	{
		*mdi = AilInstakkMidiDriverFile_95850(ailIni.driver_path, &ailIni.IO);
		if (*mdi)
			result = 0;
		else
			result = 2;
	}
	else
	{
		qmemcpy(textBuffer_181C90, (void*)"Unable to open file MDI.INI\n", 29);
		result = 1;
	}
	return result;
}

//----- (000A7970) --------------------------------------------------------
int sub_A7970_AIL_API_uninstall_MDI_driver(HMDIDRIVER mdi)//288970
{
	return AilUninstallDriver_93160(mdi->drvr_0);
	return 0;
}

//----- (000A7990) --------------------------------------------------------
int32_t sub_A7990_AIL_API_MDI_driver_type(HMDIDRIVER mdi)//288990
{
	const char* oplExt = ".OPL";

	if (mdi->drvr_0->VHDR_4->driver_version_8 >= 0x112u)
	{
		if (!_stricmp(mdi->drvr_0->VHDR_4->dev_name, "Tandy 3-voice music"))
			return 3;
		if (!_stricmp(mdi->drvr_0->VHDR_4->dev_name, "IBM internal speaker music"))
			return 3;
	}
	if (!_stricmp(oplExt, ".AD"))
		return mdi->drvr_0->size_3 <= 0x5000u;
	if (!_stricmp(oplExt, ".OPL"))
		return 2;
	return 0;
}

//----- (000A7AA0) --------------------------------------------------------
void AilApiSetGTLFilenamePrefix_A7AA0(char* samplesPath)//288aa0
{
	aSamplesPath = std::string(samplesPath);
}

//----- (000A7B30) --------------------------------------------------------
HSEQUENCE AilApiAllocateSequenceHandle_A7B30(HMDIDRIVER mdi)//288b30
{
	HSEQUENCE hSequence;
	HSEQUENCE result = nullptr;
	int i;

	PlusE3FF2_91BD0();
	for (i = 0; i < mdi->n_sequences_7 && mdi->sequences_6[i].status_1 != 1; i++)
		;
	if (i == mdi->n_sequences_7)
	{
		qmemcpy(textBuffer_181C90, (void*)"Out of sequence handles\n", 25);
		MinusE3FF2_91BF0();
		result = nullptr;
	}
	else
	{
		hSequence = &mdi->sequences_6[i];
		hSequence->status_1 = 2;
		sub_A6370(hSequence);
		hSequence->EOS_10 = 0;
		MinusE3FF2_91BF0();
		result = hSequence;
	}
	return result;
}

//----- (000A7BF0) --------------------------------------------------------
void sub_A7BF0_sound_proc33(HSEQUENCE hSequence)//288bf0
{
	if (hSequence)
	{
		AilStopSequence_95DE0(hSequence);
		hSequence->status_1 = 1;
	}
}

//----- (000A7C20) --------------------------------------------------------
int32_t AilApiInitSequence_A7C20(HSEQUENCE hSequence, void*  /*start*/, int32_t sequence_num, uint32_t track) {
	hSequence->loop_count_11 = 1;
	hSequence->volume_14 = preference_181DAC[13];
	hSequence->volume_target_15 = preference_181DAC[13];
	hSequence->volume_period_17 = 0;
	hSequence->volume_accum_16 = 0;
	hSequence->sequence_num = sequence_num;
	SOUND_init_MIDI_sequence(musicData_E3810, musicHeader_E3808, track - 1);
	return 1;
}

//----- (000A8010) --------------------------------------------------------
void AilApiStartSequence_A8010(HSEQUENCE hSequence, uint32_t track)//289010
{
	if (hSequence)
	{
		if (hSequence->status_1 != 1)
		{
			AilStopSequence_95DE0(hSequence);
			SOUND_start_sequence(track - 1);
			hSequence->status_1 = 4;
		}
	}
}

//----- (000A8010) --------------------------------------------------------
void sub_A8010_AIL_API_start_sequence_orig(HSEQUENCE hSequence)//289010
{
	if (hSequence)
	{
		if (hSequence->status_1 != 1)
		{
			AilStopSequence_95DE0(hSequence);
			sub_A6490(hSequence);
			hSequence->status_1 = 4;
		}
	}
}

void sub_A8050_AIL_API_stop_sequence(HSEQUENCE hSequence)//289050
{
	if (hSequence)
	{
		if (hSequence->status_1 == 4)
		{
			hSequence->status_1 = 8;
			SOUND_stop_sequence(hSequence->sequence_num);
		}
	}
}

void sub_A8180_AIL_API_resume_sequence(HSEQUENCE hSequence)//289180
{
	if (hSequence && hSequence->status_1 == 8)
	{
		hSequence->status_1 = 4;
		SOUND_resume_sequence(hSequence->sequence_num);
	}
}



void AilApiEndSequence_A8250(HSEQUENCE hSequence)//289250
{
	if (hSequence)
	{
		if (hSequence->status_1 != 1)
		{
			AilStopSequence_95DE0(hSequence);
			hSequence->status_1 = 2;
			SOUND_stop_sequence(hSequence->sequence_num);
		}
	}
}

//----- (000A8360) --------------------------------------------------------
void AilAPISetSequenceVolume_A8360(HSEQUENCE hSequence, int32_t volume, int32_t milliseconds)//289360
{
	if (hSequence)
	{
		hSequence->driver_0->disable_5++;
		hSequence->volume_target_15 = volume;
		if (hSequence->volume_14 == hSequence->volume_target_15)
		{
			hSequence->driver_0->disable_5--;
		}
		else
		{
			if (milliseconds)
			{
				hSequence->volume_period_17 = 1000 * milliseconds / abs(hSequence->volume_14 - hSequence->volume_target_15);
				hSequence->volume_accum_16 = 0;
			}
			else
			{
				hSequence->volume_14 = hSequence->volume_target_15;
			}
			sub_A64C0(hSequence);
			hSequence->driver_0->disable_5--;
		}
	}
}
// A0D1F: using guessed type x_DWORD abs(x_DWORD);

//----- (000A8410) --------------------------------------------------------
int AilApiSequenceStatus_A8410(HSEQUENCE hSequence)
{
	int result;

	if (hSequence)
		result = hSequence->status_1;
	else
		result = 0;
	return result;
}

//----- (000A8830) --------------------------------------------------------
void sub_A8830(HSEQUENCE hSequence, x_DWORD* a2, x_DWORD* a3)
{
	int result; // eax
	int v4; // [esp+0h] [ebp-10h]
	int i; // [esp+4h] [ebp-Ch]
	int v6; // [esp+8h] [ebp-8h]
	int v7; // [esp+Ch] [ebp-4h]

	if (hSequence)
	{
		hSequence->tempo_accum_20++;
		v7 = hSequence->beat_count_23;
		v6 = hSequence->measure_count_24;
		v4 = hSequence->beat_fraction_27;
		for (i = 0; i < preference_181DAC[14]; i++)
		{
			v4 += hSequence->time_fraction_26;
			if (v4 >= hSequence->time_per_beat_28)
			{
				v4 -= hSequence->time_per_beat_28;
				if (++v7 >= hSequence->time_numerator_25)
				{
					v7 = 0;
					++v6;
				}
			}
		}
		if (v6 < 0)
			v6 = 0;
		if (a3)
			*a3 = v6;
		if (a2)
			*a2 = v7;
		hSequence->tempo_accum_20--;		
	}
}

//----- (000A8900) --------------------------------------------------------
void sub_A8900(HSEQUENCE hSequence, int marker)
{
	int v3;
	signed int v5;
	int i;

	if (hSequence)
	{
		if (hSequence->RBRN_3)
		{
			v5 = *(signed __int16*)(&hSequence->RBRN_3[8]);
			v3 = hSequence->RBRN_3[10];
			for (i = 0; i < v5 && *(unsigned __int16*)(v3 + 6 * i) != marker; i++)
				;
			if (i != v5)
			{
				hSequence->interval_count_12 = 0;
				if (!preference_181DAC[15])
				{
					for (int j = 0; j < 4; j++)
					{
						hSequence->FOR_loop_count_33[j] = -1;
					}
				}
			}
		}
	}
}

//----- (000A8A20) --------------------------------------------------------
void AilApiTriggerCallback_A8A20(HSEQUENCE hSequence, xCALLBACK callback)
{
	if (!hSequence)
		return;
	hSequence->trigger_callback_8_32 = callback;
}

//----- (000A8BF0) --------------------------------------------------------
int AilApiLockChannel_97F90_A8BF0(MDI_DRIVER* mdi)
{
	int v2;
	HSEQUENCE v3;
	signed int v4;
	signed int v5;
	int v9;

	++mdi->disable_5;
	v5 = -1;
	v4 = 0x7FFFFFFF;
	for (int i = 8; i >= 1; i--)
	{
		if (i != 9 && mdi->lock[i] != 1 && mdi->lock[i] != 2 && mdi->notes[i] < v4)
		{
			v4 = mdi->notes[i];
			v5 = i;
		}
	}
	if (v5 == -1)
	{
		for (int j = 8; j >= 1; j--)
		{
			if (j != 9 && mdi->lock[j] != 1 && mdi->notes[j] < v4)
			{
				v4 = mdi->notes[j];
				v5 = j;
			}
		}
	}
	if (v5 == -1)
	{
		--mdi->disable_5;
		v2 = 0;
	}
	else
	{
		sub_A4F10(mdi, v5 | 0xB0, 64, 0);
		v9 = mdi->n_sequences_7;
		v3 = mdi->sequences_6;
		while (v9)
		{
			if (*(x_DWORD*)(v3 + 4) != 1)
			{
				for (int k = 0; k < 32; k++)
				{
					if (*(x_DWORD*)(v3 + 4 * k + 1368) != -1 && *(x_DWORD*)(v3 + 4 * *(x_DWORD*)(v3 + 4 * k + 1368) + 148) == v5)
					{
						sub_A5850(v3, *(unsigned int*)(v3 + 4 * k + 1368) | 0x80, *(x_DWORD*)(v3 + 4 * k + 1496), 0, 0);
						*(x_DWORD*)(v3 + 4 * k + 1368) = -1;
					}
				}
			}
			--v9;
			v3 += 1816;
		}
		mdi->state[v5] = mdi->lock[v5];
		mdi->lock[v5] = 1;
		mdi->locker[v5] = nullptr;
		mdi->owner[v5] = mdi->user[v5];
		--mdi->disable_5;
		v2 = v5 + 1;
	}
	return v2;
}

//----- (000A8EA0) --------------------------------------------------------
void sub_A8EA0(HMDIDRIVER mdi, int a2)
{
}

//----- (000A9080) --------------------------------------------------------
void sub_A9080(HSEQUENCE sequence, int a2, int a3)
{
	HSEQUENCE result = nullptr;
	if (sequence)
	{
		sequence->chan_map_37[a2] = a3 - 1;
		sequence->driver_0->notes[a3];
		if (sequence->FOR_loop_count_33[a3] == 1)
		{
			result = sequence->driver_0->var23_aildrvx[a3];
			if (result != sequence)
			{
				sequence->driver_0->var23_aildrvx[a3] = sequence;
			}
		}
	}
}

void AilAPISendChannelVoiceMessage_A9140(HMDIDRIVER mdi, HSEQUENCE hSequence, int32_t status, int32_t data_1, int32_t data_2)//28a140
{
}

IO_PARMS* AilGetIoEnvironment_92ED0(AIL_DRIVER* drvr)
{
	IO_PARMS* result; // [esp+Ch] [ebp-4h]

	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_get_IO_environment(0x%X)\n", drvr);
	result = AilApiGetIoEnvironment_9EB60(drvr);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00093010) --------------------------------------------------------
AIL_DRIVER* AilInstallDriver_93010(uint8_t* driver_image, int32_t n_bytes)//274010
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_install_driver(0x%X,%u)\n", driver_image, n_bytes);
	AIL_DRIVER* result = AilApiInstallDriver_9E720(driver_image, n_bytes);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = 0x%X\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00093160) --------------------------------------------------------
int AilUninstallDriver_93160(AIL_DRIVER* drvr)
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_uninstall_driver(0x%X)\n", drvr);
	int result = AilApiUninstallDriver_9EA60(drvr);
	ailIndent_181C04--;
	return result;
}

//----- (000931F0) --------------------------------------------------------
int AilInstallDigIni_931F0(HDIGDRIVER* dig, char* digPath)//2741f0
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_install_DIG_INI(0x%X)\n", dig);
	int result = AilApiInstallDigINI_A3720(dig, digPath);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00098B2C) --------------------------------------------------------
__int16 sub_98B2C(unsigned __int8 a1, int a2)//180804 279b2c
{
	unsigned __int8 v2;
	unsigned __int16 v3;
	unsigned __int16 v4;
	char v5;
	__int16 v7;

	v2 = a1;
	v3 = x_WORD_E3B4E;
	v4 = x_WORD_E3B4C;
	v7 = v4 & ((1 << v2) - 1);
	v5 = x_BYTE_E3B51 - v2;
	if (x_BYTE_E3B51 < v2)
	{
		v4 = __ROR2__(x_WORD_E3B4E & ((1 << x_BYTE_E3B51) - 1), x_BYTE_E3B51) |
			((unsigned __int16)x_WORD_E3B4C >> x_BYTE_E3B51);
		v3 = *(x_WORD*)(a2 + 2);
		v2 -= x_BYTE_E3B51;
		v5 = 16 - v2;
	}
	x_WORD_E3B4E = v3 >> v2;
	x_WORD_E3B4C = __ROR2__(v3 & ((1 << v2) - 1), v2) | (v4 >> v2);
	x_BYTE_E3B51 = v5;
	return v7;
}

bool DebugSoundTimer_916F0()
{
	PlusE3FF2_A0EEC();
	time_t timer = x_DWORD_181C0C;
	struct tm* timeptr = x_DWORD_181C08;
	MinusE3FF2_A0EF9();
	if (ailIndent_181C04 == 1)
	{
		dbgfprintf(ailDebufFile_181BF0, "[%.02u:%.02u:%.02u.%.02u] ", x_DWORD_181BFC, timer, timeptr, x_DWORD_181C10);
	}
	else
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
	}
	for (int j = 1; j < ailIndent_181C04; j++)
		dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
	return true;
}

//----- (0009E2B0) --------------------------------------------------------
int StrToInt(const char* text, int maxDigits)
{
	int value = 0;
	int sign = 1;
	for (int i = 0; strlen(text) > i; i++)
	{
		if (text[i] == '-')
		{
			sign = -sign;
		}
		else if (!(IsTable[text[i] + 1] & 2))
		{
			int j;
			for (j = 0; j < maxDigits; j++)
			{
				if (x_toupper(text[i]) == x_BYTE_D3450[j])
				{
					value = maxDigits * value + j;
					break;
				}
			}
			if (j == maxDigits)
				break;
		}
	}
	return sign * value;
}

//----- (0009D4D0) --------------------------------------------------------
signed int AllocMem_9D4D0(int size16, uint8_t** buffer, uint32_t* offset, uint32_t* selector)//27e4d0
{
	uint8_t* locBuffer;

	locBuffer = (uint8_t*)Malloc_83D70(16 * size16);
	if (locBuffer == nullptr)
		return 0;
	*offset = 0;//(int)v6 >> 4 << 16;//fixed - remove segment
	*buffer = locBuffer;
	*selector = 1232;
	return 1;
}

//----- (0009D560) --------------------------------------------------------
void FreeMem_9D560(uint8_t* a1)//27e560
{
	FreeMem_83E80(a1);
}

//----- (00091F20) --------------------------------------------------------
unsigned int sub_91F20()
{
	return sub_A1520();
}

void AIL_fix() {
	myprintf("Unsupported sound call");
	exit(0);
};

//----- (0009F110) --------------------------------------------------------
signed __int64 sub_9F110(int a1)//dpmi_real_malloc see: https://github.com/videogamepreservation/descent2/blob/master/SOURCE/BIOS/DPMI.C
{
	int v4; // [esp+Ch] [ebp-18h]
	int v5; // [esp+18h] [ebp-Ch]
	unsigned int v6; // [esp+20h] [ebp-4h]

	//fix it
	v4 = 0;
	v5 = 0;
	//fix it

	if (v5)
		LOWORD(v6) = 0;
	else
		LOWORD(v6) = v4;
	return __PAIR__(v6, 0);
}

//----- (0009F170) --------------------------------------------------------
int sub_9F170(int  /*a1*/, unsigned __int16 a2)//dpmi_real_free see: https://github.com/videogamepreservation/descent2/blob/master/SOURCE/BIOS/DPMI.C
{
	int v5; // [esp+18h] [ebp-8h]
	int v6; // [esp+1Ch] [ebp-4h]

	//fix it
	v5 = 0;
	//fix it

	if (v5)
		v6 = -1;
	else
		v6 = 0;
	return v6;
}

//----- (0009F1D0) --------------------------------------------------------
int sub_9F1D0(int a1)//see: https://www.delorie.com/djgpp/doc/dpmi/api/310002.html
{
	int v2; // [esp+0h] [ebp-20h]
	int v4; // [esp+18h] [ebp-8h]
	int v5; // [esp+1Ch] [ebp-4h]

	//fix it
	v4 = 0;
	//fix it

	v2 = 2;

	if (v4)
		v5 = -1;
	else
		v5 = (unsigned __int16)v2;
	return v5;
}

//----- (0009F220) --------------------------------------------------------
int sub_9F220(int a1)//serial_dpmi_lock_linear_memory/serial_dpmi_unlock_linear_memory see:https://github.com/kstenerud/DOS-Serial-Library/blob/master/serial.c
{
	unsigned __int16 v4;
	unsigned __int16 v5;
	int v6;
	int v7;
	//fix it
	v4 = 0;
	//fix it

	//fix it
	v5 = 0;
	v6 = 0;
	//fix it

	if (v6)
		v7 = -1;
	else
		v7 = ((signed int)v5 >> 4) + (v4 << 12);
	return v7;
}

//----- (0008EAD0) --------------------------------------------------------
bool InitMusicBank_8EAD0(int channel)//26fad0
{
	int16_t driverarray[4];
	FILE* filehandle;
	int datapos;
	int finaldrivernumber = 0;
	int filelenght;
	int channellplus;
	char musicPath[MAX_PATH];
	if (!musicAble_E37FC)
		return false;
	StopMusic_8E020();

	sprintf(musicPath, "%s/SOUND/MUSIC.DAT", cdDataPath.c_str());
	filehandle = DataFileIO::CreateOrOpenFile(musicPath, 512);
	if (!filehandle)
		return false;
	DataFileIO::Seek(filehandle, 0, 2);
	filelenght = myftell(filehandle);
	DataFileIO::Seek(filehandle, filelenght - 4, 0);
	DataFileIO::Read(filehandle, (uint8_t*)&datapos, 4);
	DataFileIO::Seek(filehandle, datapos, 0);
	DataFileIO::Read(filehandle, (uint8_t*)driverarray, 8);
	channellplus = channel + 1;

	switch (musicDriverType_180C84)
	{
		case 'g':
		case 'G':
			finaldrivernumber = 0;
			break;
		case 'r':
		case 'R':
			finaldrivernumber = 1;
			break;
		case 'f':
		case 'F':
			finaldrivernumber = 2;
			break;
		case 'w':
		case 'W':
			finaldrivernumber = 3;
			break;
	}

	if (channellplus <= driverarray[finaldrivernumber])
	{
		DataFileIO::Seek(filehandle, (channellplus - 1) << 6, 1);
		if (!LoadMusicTrack(filehandle, finaldrivernumber))
		{
			DataFileIO::Close(filehandle);
			return true;
		}
		DataFileIO::Close(filehandle);
		return false;
	}
	DataFileIO::Close(filehandle);
	return true;
}

//----- (0008EC90) --------------------------------------------------------
void GetMusicSequenceCount()//26fc90 // set index
{
	if (musicHeader_E3808 && musicData_E3810)
	{
		for (m_iNumberOfTracks = 0; m_iNumberOfTracks < countOfMusicTracks_E380C; m_iNumberOfTracks++)
		{
			uint8_t* zero_pointer = 0;
			musicHeader_E3808->str_8.track_10[m_iNumberOfTracks].xmiData_0 = &musicData_E3810[musicHeader_E3808->str_8.track_10[m_iNumberOfTracks].xmiData_0 - zero_pointer];
		}
	}
}

//----- (0008ED00) --------------------------------------------------------
bool LoadMusicTrack(FILE* filehandle, uint8_t drivernumber)//26fd00
{
	type_v8 headerx[4];
	int rncsize;

	myftell(filehandle);
	DataFileIO::Read(filehandle, (uint8_t*)headerx, 64);
	if (headerx[drivernumber].dword_4 == -1)
		return false;
	if (musicData_E3810)
	{
		FreeMem_83E80(musicData_E3810);
		musicAble_E37FC = false;
	}
	if (musicHeader_E3808)
	{
		FreeMem_83E80((uint8_t*)musicHeader_E3808);
		musicAble_E37FC = false;
	}
	musicData_E3810 = (uint8_t*)Malloc_83CD0(headerx[drivernumber].dword_12);
	if (!musicData_E3810)
		return false;
	musicHeader_E3808 = (type_E3808_music_header*)Malloc_83CD0(sizeof(type_E3808_music_header));
	if (!musicHeader_E3808)
	{
		FreeMem_83E80(musicData_E3810);
		return false;
	}

	//64xbit fix
	shadow_type_E3808_music_header * shadow_str_E3808_music_header = (shadow_type_E3808_music_header*)Malloc_83CD0(sizeof(shadow_type_E3808_music_header));
	if (!shadow_str_E3808_music_header)
	{
		FreeMem_83E80((uint8_t*)shadow_str_E3808_music_header);
		return false;
	}
	//64xbit fix
	
	countOfMusicTracks_E380C = (headerx[drivernumber].sizeBytes_8) / sizeof(sub2type_E37A0_sound_buffer2);
	DataFileIO::Seek(filehandle, headerx[drivernumber].dword_4, 0);
	DataFileIO::Read(filehandle, musicData_E3810, 8);
	if (musicData_E3810[0] == 'R' && musicData_E3810[1] == 'N' && musicData_E3810[2] == 'C')//RNC
	{
		rncsize = musicData_E3810[4];
		rncsize <<= 8;
		rncsize += musicData_E3810[5];
		rncsize <<= 8;
		rncsize += musicData_E3810[6];
		rncsize <<= 8;
		rncsize += musicData_E3810[7];
		DataFileIO::Read(filehandle, (uint8_t*)&musicData_E3810[8], rncsize - 8);
		DataFileIO::Decompress(musicData_E3810, musicData_E3810);
	}
	else
	{
		DataFileIO::Read(filehandle, (uint8_t*)&musicData_E3810[8], headerx[drivernumber].dword_12 - 8);
	}
	DataFileIO::Seek(filehandle, headerx[drivernumber].dword_0, 0);

	//64x fix shadow_str_E3808_music_header
	DataFileIO::Read(filehandle, (uint8_t*)shadow_str_E3808_music_header, 8);
	if (shadow_str_E3808_music_header->byte_0 == 'R' && shadow_str_E3808_music_header->byte_1 == 'N' && shadow_str_E3808_music_header->byte_2 == 'C')//RNC
	{
		rncsize = shadow_str_E3808_music_header->byte_4;
		rncsize <<= 8;
		rncsize += shadow_str_E3808_music_header->byte_5;
		rncsize <<= 8;
		rncsize += shadow_str_E3808_music_header->byte_6;
		rncsize <<= 8;
		rncsize += shadow_str_E3808_music_header->byte_7;
		DataFileIO::Read(filehandle, (uint8_t*)(&shadow_str_E3808_music_header->str_8), rncsize - 8);
		DataFileIO::Decompress((uint8_t*)shadow_str_E3808_music_header, (uint8_t*)shadow_str_E3808_music_header);
	}
	else
	{
		DataFileIO::Read(filehandle, (uint8_t*)(&shadow_str_E3808_music_header->str_8), headerx[drivernumber].sizeBytes_8 - 8);
	}
	musicHeader_E3808->byte_0 = shadow_str_E3808_music_header->byte_0;
	musicHeader_E3808->byte_1 = shadow_str_E3808_music_header->byte_1;
	musicHeader_E3808->byte_2 = shadow_str_E3808_music_header->byte_2;
	musicHeader_E3808->byte_3 = shadow_str_E3808_music_header->byte_3;
	musicHeader_E3808->byte_4 = shadow_str_E3808_music_header->byte_4;
	musicHeader_E3808->byte_5 = shadow_str_E3808_music_header->byte_5;
	musicHeader_E3808->byte_6 = shadow_str_E3808_music_header->byte_6;
	musicHeader_E3808->byte_7 = shadow_str_E3808_music_header->byte_7;
	for (int i = 0; i < 10; i++)
		musicHeader_E3808->str_8.stub[i] = shadow_str_E3808_music_header->str_8.stub[i];
	for (int i = 0; i < 6; i++)
	{
		musicHeader_E3808->str_8.track_10[i].xmiData_0 = shadow_str_E3808_music_header->str_8.track_10[i].xmiData_0 + Zero_pointer;
		for (int j = 0; j < 4; j++)
			musicHeader_E3808->str_8.track_10[i].stub_4[j] = shadow_str_E3808_music_header->str_8.track_10[i].stub_4[j];
		musicHeader_E3808->str_8.track_10[i].xmiSize_8 = shadow_str_E3808_music_header->str_8.track_10[i].xmiSize_8;
		musicHeader_E3808->str_8.track_10[i].word_12 = shadow_str_E3808_music_header->str_8.track_10[i].word_12;
		for (int j = 0; j < 18; j++)
			musicHeader_E3808->str_8.track_10[i].filename_14[j] = shadow_str_E3808_music_header->str_8.track_10[i].filename_14[j];
	}
	for (int i = 0; i < 14; i++)
		musicHeader_E3808->str_8.stubb[i] = shadow_str_E3808_music_header->str_8.stubb[i];
	FreeMem_83E80((uint8_t*)shadow_str_E3808_music_header);
	//64x fix
	GetMusicSequenceCount();

	for (int i = 1; i <= m_iNumberOfTracks; i++)//2b4804
		AilInitSequence_95C00(m_hSequence, musicHeader_E3808->str_8.track_10[i].xmiData_0, 0, i);
	musicAble_E37FC = true;
	return true;
}

//----- (0008F0AB) --------------------------------------------------------
int sub_8F0AB(FILE* a1, /*int a2,*/ int a3)//26f0ab
{
	int result = DataFileIO::Read(a1, (uint8_t*)x_DWORD_E387C, a3);
	x_DWORD_E3880 = x_DWORD_E387C;
	x_DWORD_E3884 = result;
	x_DWORD_E3888 = 0;
	return result;
}

//----- (0008F100) --------------------------------------------------------
void sub_8F100_sound_proc19(uint32_t flags, __int16 index, int volume, int volumePan, unsigned __int16 playRate, char loopCount, unsigned __int8 playType)//270100
{
	bool bool1; // [esp+0h] [ebp-18h]

	HSAMPLE* soundBuffer1 = nullptr;
	HSAMPLE* soundBuffer2 = nullptr;

	if (!soundAble_E3798
		|| !soundActive_E3799
		|| index > (signed int)indexLoadedSound_180B50
		|| !_stricmp((const char*)&soundIndex_E37A0->str_8.wavs_10[index -1].filename_14, "null.wav"))
	{
		return;
	}
	bool1 = false;
	switch (playType)
	{
	case 1:
		{
			soundBuffer1 = NULL;
			for (int i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
			{
				if (AilSampleStatus_94010(SoundBuffer3_180750[i]) == 2)
				{
					soundBuffer1 = &SoundBuffer3_180750[i];
					soundBuffer2 = nullptr;
					break;
				}
			}
			break;
		}
	case 2:
		{
			soundBuffer2 = nullptr;
			for (int j = 0; j < x_DWORD_180B4C_end_sound_buffer3_endindex; j++)
			{
				if (SoundBuffer3_180750[j]->flags_14 == flags && SoundBuffer3_180750[j]->vol_scale_18[0][0] == index && AilSampleStatus_94010(SoundBuffer3_180750[j]) != 2)
				{
					soundBuffer2 = &SoundBuffer3_180750[j];
					break;
				}
			}
			if (!soundBuffer2)
			{
				soundBuffer1 = nullptr;
				for (int k = 0; k < x_DWORD_180B4C_end_sound_buffer3_endindex; k++)
				{
					if (AilSampleStatus_94010(SoundBuffer3_180750[k]) == 2)
					{
						soundBuffer1 = &SoundBuffer3_180750[k];
						break;
					}
				}
			}
			break;
		}
	case 3:
		{
			bool1 = false;
			for (int l = 0; l < x_DWORD_180B4C_end_sound_buffer3_endindex; l++)
			{
				if (SoundBuffer3_180750[l]->flags_14 == flags && SoundBuffer3_180750[l]->vol_scale_18[0][0] == index)
				{
					soundBuffer1 = &SoundBuffer3_180750[l];
					soundBuffer2 = nullptr;
					bool1 = true;
					break;
				}
			}
			if (!bool1)
			{
				soundBuffer1 = nullptr;
				for (int m = 0; m < x_DWORD_180B4C_end_sound_buffer3_endindex; m++)
				{
					if (AilSampleStatus_94010(SoundBuffer3_180750[m]) == 2)
					{
						soundBuffer1 = &SoundBuffer3_180750[m];
						soundBuffer2 = nullptr;
						break;
					}
				}
			}
			break;
		}
	}
	if (!soundBuffer1 || soundBuffer2)
		return;
	if (!bool1)
	{
		AilInitSample_93830(*soundBuffer1);
#ifdef DEBUG_SOUND
		if (debug_first_sound) {
			uint8_t* debug_sound_buff = soundIndex_E37A0->str_8.wavs_10[index].wavData_0;
			debug_printf("sub_8F100_sound_proc19:buff:\n");
			for (int i = 0; i < 100; i++)
				debug_printf("%02X", debug_sound_buff[i]);
			debug_printf("\n");
		}
#endif //DEBUG_SOUND
		AilSetSampleFile_938C0(*soundBuffer1, soundIndex_E37A0->str_8.wavs_10[index].wavData_0, 1);
	}
	AilSetSampleVolume_93E30(*soundBuffer1, volume);
	AilSetSampleVolumePan_93ED0(*soundBuffer1, volumePan);
	AilSetSamplePlaybackRate_93D90(*soundBuffer1, soundFrequence_E37BC * playRate / 100);
	AilSetSampleLoopCount_93F70(*soundBuffer1, loopCount + 1);
#ifdef DEBUG_SOUND
	if (debug_first_sound) {
		debug_printf("sub_8F100_sound_proc19:44mhz:\n");
		debug_printf("\n");
		debug_printf("sub_8F100_sound_proc19:rate:%d\n", (*soundBuffer1)->playback_rate_15);
	}
#endif //DEBUG_SOUND
	AilStartSample_93B50(*soundBuffer1);
	(*soundBuffer1)->flags_14 = flags;
	(*soundBuffer1)->vol_scale_18[0][0] = index;
	(*soundBuffer1)->status_1 = volume;
	(*soundBuffer1)->len_4_5[1] = volumePan;
	(*soundBuffer1)->vol_scale_18[0][2] = 0;
	(*soundBuffer1)->vol_scale_18[0][3] = 0;
}

//----- (0008F420) --------------------------------------------------------
void sub_8F420_sound_proc20(int a1, __int16 a2)//270420
{
	if (soundAble_E3798 && soundActive_E3799)
	{
		for (int i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
		{
			if (SoundBuffer3_180750[i]->flags_14 == a1 && SoundBuffer3_180750[i]->vol_scale_18[0][0] == a2 && AilSampleStatus_94010(SoundBuffer3_180750[i]) != 2)
			{
				AilEndSample_93D00(SoundBuffer3_180750[i]);
				return;
			}
		}
	}
}

//----- (0008F710) --------------------------------------------------------
void sub_8F710_sound_proc21(int flags, __int16 index, int loopCount, unsigned __int8 initTimers, char volScale)//270710
{
	if (soundAble_E3798 && soundActive_E3799 && index <= indexLoadedSound_180B50)
	{
		for (int i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
		{
			if (SoundBuffer3_180750[i]->flags_14 == flags && SoundBuffer3_180750[i]->vol_scale_18[0][0] == index && AilSampleStatus_94010(SoundBuffer3_180750[i]) != 2)
			{
				if (loopCount > 127)
					loopCount = 127;
				if (loopCount != SoundBuffer3_180750[i]->status_1)
				{
					SoundBuffer3_180750[i]->vol_scale_18[0][2] = 0;
					SoundBuffer3_180750[i]->loop_count_12 = loopCount;
					SoundBuffer3_180750[i]->vol_scale_18[0][3] = volScale;
					if (loopCount > SoundBuffer3_180750[i]->status_1)
						SoundBuffer3_180750[i]->vol_scale_18[0][2] = 1;
					else
						SoundBuffer3_180750[i]->vol_scale_18[0][2] = 2;
					if (!TimerRun_E388D)
					{
						TimerRun_E388D = true;
						if (initTimers <= 4u)
						{
							//sub_92930_AIL_set_timer_frequency(x_DWORD_180CA0[0], 30 * initTimers);
							//sub_92BA0_AIL_start_timer(x_DWORD_180CA0[0]);
						}
					}
				}
				return;
			}
		}
	}
}

//----- (0008F850) --------------------------------------------------------
void StopTimer_8F850()//270850
{
	if (TimerRun_E388D)
	{
		//sub_92DC0_AIL_release_timer_handle(x_DWORD_180CA0[0]);
		TimerRun_E388D = false;
		for (uint32_t i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
		{
			SoundBuffer3_180750[i]->vol_scale_18[0][2] = 0;
			SoundBuffer3_180750[i]->vol_scale_18[0][3] = 0;
		}
	}
}

//----- (00091F70) --------------------------------------------------------
int32_t AilCallDriver_91F70(AIL_DRIVER* drvr, int32_t fn, VDI_CALL* in, VDI_CALL* out)//272f70
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_call_driver(0x%X,0x%X,0x%X,0x%X)\n", drvr, fn, in, out);
	int result = AilApiCallDriver_A158B(drvr, fn, in, out);
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i = 0; i < 14; i++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int j = 1; j < ailIndent_181C04; j++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %d\n", result);
	}
	ailIndent_181C04--;
	return result;
}

//----- (000920D0) --------------------------------------------------------
void AilDelay_920D0(int delay)//AIL_delay
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_delay(%d)\n", delay);
	AilApiDelay_A1665(delay);
	ailIndent_181C04--;
}

//----- (00092190) --------------------------------------------------------
bool AilReadIniFile_92190(AIL_INI* INI, char* filename)//273190
{
	ailIndent_181C04++;
	if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2() && DebugSoundTimer_916F0())
		dbgfprintf(ailDebufFile_181BF0, "AIL_API_read_INI(0x%X,%s)\n", INI, filename);
	bool result = AilApiReadIni_9E3A0(INI, filename);
	if (result)
	{
		if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
		{
			for (int i = 0; i < 14; i++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int j = 1; j < ailIndent_181C04; j++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "Driver = %s\n", INI->driver_name);
			for (int k = 0; k < 14; k++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int l = 1; l < ailIndent_181C04; l++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "Device = %s\n", INI->device_name);
			for (int m = 0; m < 14; m++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int n = 1; n < ailIndent_181C04; n++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "IO     = %X\n", INI->IO.IO);//*(signed __int16 *)(a1 + 256)
			for (int ii = 0; ii < 14; ii++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int jj = 1; jj < ailIndent_181C04; jj++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "IRQ    = %d\n", INI->IO.IRQ);//*(signed __int16 *)(a1 + 258)
			for (int kk = 0; kk < 14; kk++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int ll = 1; ll < ailIndent_181C04; ll++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "DMA_8  = %d\n", INI->IO.DMA_8_bit);//*(signed __int16 *)(a1 + 260)
			for (int mm = 0; mm < 14; mm++)
				dbgfprintf(ailDebufFile_181BF0, " \0");
			for (int nn = 1; nn < ailIndent_181C04; nn++)
				dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
			dbgfprintf(ailDebufFile_181BF0, "DMA_16 = %d\n", INI->IO.DMA_16_bit);//*(signed __int16 *)(a1 + 262)
		}
	}
	else if (x_DWORD_181BF4 && (ailIndent_181C04 == 1 || x_DWORD_181BF8) && !GetE3FFE_A16A2())
	{
		for (int i1 = 0; i1 < 14; i1++)
			dbgfprintf(ailDebufFile_181BF0, " \0");
		for (int i2 = 1; i2 < ailIndent_181C04; i2++)
			dbgfprintf(ailDebufFile_181BF0, "\0xfa\0");
		dbgfprintf(ailDebufFile_181BF0, "Result = %u\n", 0);
	}
	ailIndent_181C04--;
	return result;
}

//----- (00099970) --------------------------------------------------------
void sub_99970(char a1, unsigned __int8 a2)//27a970
{
	if (pcSpeakerSoundDev_E3819 && musicAble_E37FC && musicActive_E37FD && songCurrentlyPlaying_E3802 && AilSequenceStatus_96170(m_hSequence) != 2 && x_BYTE_E3817 != a1)
	{
		if (x_BYTE_E3818)
			;// sub_92DC0_AIL_release_timer_handle(x_DWORD_180C80);
		x_BYTE_E3817 = a1;
		x_BYTE_E381A = -x_BYTE_E381A;
		x_BYTE_E3818 = 1;
		if (a2 <= 4u && a2 >= 1u)
		{
			//x_DWORD_180C80 = sub_92600_AIL_register_timer(sub_99830);
			//sub_92930_AIL_set_timer_frequency(x_DWORD_180C80, 30 * a2);
			//sub_92BA0_AIL_start_timer(x_DWORD_180C80);
		}
		else
		{
			//x_DWORD_180C80 = sub_92600_AIL_register_timer(sub_99830);
			//sub_92930_AIL_set_timer_frequency(x_DWORD_180C80, 0x1Eu);
			//sub_92BA0_AIL_start_timer(x_DWORD_180C80);
		}
	}
}

//----- (00099E8E) --------------------------------------------------------
signed int sub_99E8E(char* a1, char** a2, signed int a3, int a4)//27ae8e // some for VR?
{
	signed int v4; // edi
	char* i; // ebx
	signed int result; // eax
	unsigned __int8 v8; // cl
	unsigned int v9; // esi
	int v10; // eax
	unsigned int v11; // edx
	char* v12; // [esp+0h] [ebp-1Ch]
	char v13; // [esp+4h] [ebp-18h]
	unsigned __int8 v14; // [esp+8h] [ebp-14h]

	v4 = a3;
	if (a2)
		*a2 = a1;
	for (i = a1; IsTable[(unsigned __int8)(*i + 1)] & 2; i++)
		;
	v14 = *i;
	if (*i == '+' || *i == '-')
		++i;
	if (!a3)
	{
		if (*i != '0' || (i[1] != 'x' && i[1] != 'X'))
		{
			if (*i == '0')
				v4 = 8;
			else
				v4 = 10;
		}
		else
		{
			v4 = 16;
		}
	}
	if (v4 >= 2 && v4 <= 36)
	{
		if (v4 == 16 && *i == '0')
		{
			v8 = i[1];
			if (v8 == 'x' || v8 == 'X')
				i += 2;
		}
		v12 = i;
		v9 = 0;
		v13 = 0;
		while (1)
		{
			v10 = sub_99FF5(*i);
			if (v10 >= v4)
				break;
			v11 = v9;
			v9 = v10 + v4 * v9;
			if (v9 < v11)
				v13 = 1;
			++i;
		}
		if (i == v12)
			i = a1;
		if (a2)
			*a2 = i;
		if (a4 == 1 && v9 >= 0x80000000 && (v9 != 0x80000000 || v14 != 45))
			v13 = 1;
		if (v13)
		{
			//_set_errno(14);
			if (a4)
			{
				if (v14 == 45)
					result = 2147483648;
				else
					result = 0x7FFFFFFF;
			}
			else
			{
				result = -1;
			}
		}
		else
		{
			if (v14 == 45)
				v9 = 0 - v9;
			result = v9;
		}
	}
	else
	{
		//_set_errno(13);
		result = 0;
	}
	return result;
}
// 9D195: using guessed type x_DWORD _set_errno(x_DWORD);

//----- (00099FF5) --------------------------------------------------------
int sub_99FF5(unsigned __int8 a1)//27aff5 // some for VR?
{
	unsigned __int8 v2; // al

	if (a1 >= '0' && a1 <= '9')
		return a1 - '0';
	v2 = x_tolower(a1);
	if (v2 >= 'a' && v2 <= 'i')
		return v2 - 'a' + 10;
	if (v2 >= 'j' && v2 <= 'r' || v2 >= 's' && v2 <= 'z')
		return v2 - 'a' + 10;
	return '%';
}

//----- (0009A230) --------------------------------------------------------
void SetSoundFreq_9A230(int freq)//27B230
{
	switch (freq) {
		case 800:
		case 811:
		case 822:
		case 1611:
		case 1622:
		case 1644:
			soundFreqType2_E37B4 = freq;
			break;
		default:
			soundFreqType2_E37B4 = 0;
	};
}

void WriteWaveToFile(wav_t* wav, const char* name)
{
	if (wav != nullptr && wav->data_44 != nullptr)
	{
		std::string path = GetSubDirectoryPath("BufferOut");
		if (myaccess(path.c_str(), 0) < 0)
		{
			std::string exepath = get_exe_path();
			mymkdir((exepath + "/" + std::string("BufferOut")).c_str());
		}

		if (_stricmp(name, "") == 0)
		{
			name = "UNKNOWN.WAV";
		}

		path = GetSubDirectoryFilePath("BufferOut", name);
		FILE* wavFile = fopen(path.c_str(), "wb");
		fwrite((uint8_t*)wav, 1, WAVE_HEADER_SIZE_BYTES, wavFile);
		fwrite((uint8_t*)wav->data_44, 1, wav->dataSizeBytes_40, wavFile);

		fclose(wavFile);
	}
}