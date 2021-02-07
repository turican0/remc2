#include "Sound.h"


//----- (0008D290) --------------------------------------------------------
void sub_8D290_init_sound(/*char* a1*//*, int a2, int a3*/)//26e290
{
	//int (*v3)(int); // eax
	//x_DWORD *v4; // eax
	IO_PARMS v5 = {}; // [esp+4h] [ebp-3Ch]
	char v6; // [esp+1Ch] [ebp-24h]
	//errno_t v7x; // [esp+2Ch] [ebp-14h]
	char* v7;
	uint32_t v8; // [esp+34h] [ebp-Ch]
	int v9; // [esp+38h] [ebp-8h]
	char v10; // [esp+3Ch] [ebp-4h]

	char soundGamePath[MAX_PATH];

	sprintf(soundGamePath, "%s/SOUND", gameDataPath);
	//fix it
	//v7 = 0;
	//v5 = 0;
	//fix it

	//char v8x[200];
	//IO_PARMS v9x = {};

	v10 = 0;
	if (!x_BYTE_E3798_sound_active2)
		return;
	x_WORD_E37B6_sound_number = x_WORD_E37B4;
	if (!x_WORD_E37B4)
	{
		if (!x_BYTE_E37FC_music)
		{
			if (x_BYTE_E37B8)
				sub_919C0();
		}
		x_BYTE_E3798_sound_active2 = 0;
		x_BYTE_E3799_sound_card = 0;
		myprintf("\nCould not load any digital samples\n");
		return;
	}
	if (!x_BYTE_E37B8)
	{
		//sub_9D3F0_set_malloc_type(sub_83CD0_malloc2);
		//v3 = sub_9D420(( (*)(void*))sub_83E80_freemem4);
		//((void (*)(int (*)(int)))loc_917D0)(v3);
		//test removing sub_917D0(/*v3*/);
		//fixed
		x_DWORD_181DAC[0] = 0xc8;
		x_DWORD_181DAC[1] = 0x01;//DB0
		x_DWORD_181DAC[2] = 0x800;//DB4
		x_DWORD_181DAC[3] = 0x64;//DB8
		x_DWORD_181DAC[4] = 0x10;//DBC
		x_DWORD_181DAC[5] = 0x64;//DC0
		x_DWORD_181DAC[6] = 0x28f;//DC4
		x_DWORD_181DAC[7] = 0x00;//DC8
		x_DWORD_181DAC[8] = 0x00;//DCC
		x_DWORD_181DAC[9] = 0x01;//DD0
		x_DWORD_181DAC[10] = 0x00;//DD4
		x_DWORD_181DAC[11] = 0x78;//DD8
		x_DWORD_181DAC[12] = 0x08;//DDC
		x_DWORD_181DAC[13] = 0x7f;//DE0
		x_DWORD_181DAC[14] = 0x01;//DE4
		x_DWORD_181DAC[15] = 0x00;//DE8
		x_DWORD_181DAC[16] = 0x02;//DEC
		x_DWORD_181DAC[17] = 0x01;//DF0
		x_DWORD_181DAC[18] = 0x01;//DF4

		//x_DWORD_181DB4 = 0x800;//fixed
		//x_DWORD_181DB8 = 0x64;//fixed
		//x_DWORD_181DBC = 0x10;//fixed
		//x_DWORD_181DC0 = 0x64;//fixed
		//x_DWORD_181DC4 = 0x028f;//fixed
		//x_DWORD_181DC8 = 0x00;//fixed
		//x_DWORD_181DCC = 0x00;//fixed
		//x_DWORD_181DD0 = 0x01;//fixed
		//x_DWORD_181DD4 = 0x00;//fixed
		//x_DWORD_181DD8 = 0x78;//fixed
		//x_DWORD_181DDC = 0x08;//fixed
		//x_DWORD_181DE0 = 0x7f;//fixed
		//x_DWORD_181DE4 = 0x01;//fixed
		//x_DWORD_181DE8 = 0x00;//fixed
		//x_DWORD_181DEC = 0x02;//fixed
		//x_DWORD_181DF0 = 0x01;//fixed
		//x_DWORD_181DF4 = 0x01;//fixed

		//x_DWORD_181DF8 = 0x00;//fixed
		//x_DWORD_181DFC = 0x00;//fixed
		//x_DWORD_181E00 = 0x00;//fixed
		//fixed

		x_BYTE_E37B8 = 1;
	}

	if (!DirExists(soundGamePath))
	{
		x_BYTE_E3798_sound_active2 = 0;
		x_BYTE_E3799_sound_card = 0;
		if (x_BYTE_E37FC_music && x_BYTE_E37FE)
		{
			sub_99C90();
		}
		else
		{
			x_BYTE_E37FC_music = 0;
			x_BYTE_E37FD = 0;
			sub_919C0();
		}
		return;
	}
	size_t sz = 0;
	//v7x = _dupenv_s(&v7,&sz, "MDSOUND");//fix v7
	v7 = mygetenv("MDSOUND");
	if (v7)
	{
		sscanf((char* const)v7, "%s %x %d %d %d", (unsigned int)&v6);
		hDigSoundEffectsDriver = sub_93330_AIL_install_DIG_driver_file(/*(int)a1, */&v6, &v5);
		if (!hDigSoundEffectsDriver)
		{
			if (!x_BYTE_E37FC_music)
				sub_919C0();
			x_BYTE_E3798_sound_active2 = 0;
			x_BYTE_E3799_sound_card = 0;
			return;
		}
		sprintf(soundEffectsAILSettings.driver_name, "%s", &v6);
		x_WORD_180B30 = v5.IO;//fixed
		sub_93480_AIL_uninstall_DIG_driver((unsigned int*)hDigSoundEffectsDriver);
		v10 = 1;
	}
	else
	{
		/*x_WORD_E2A14_sound_active = 1;// sound fixed
		x_WORD_E3804 = 6;// sound fixed
		return; // sound fixed*/
		char digPath[MAX_PATH];
		sprintf(digPath, "%s/DIG.INI", soundGamePath);
		if (LoadAILFromFile(&soundEffectsAILSettings, digPath)) // sound driver
		{
			if (!_stricmp(soundEffectsAILSettings.driver_name, "None"))//351ab0
			{
				if (!x_BYTE_E37FC_music)
					sub_919C0();
				x_BYTE_E3798_sound_active2 = 0;
				x_BYTE_E3799_sound_card = 0;
				return;
			}
			if (!sub_931F0_AIL_install_DIG_INI(/*(int)a1, */&hDigSoundEffectsDriver, digPath))//351b48
			{
				sub_93480_AIL_uninstall_DIG_driver((unsigned int*)hDigSoundEffectsDriver);
				v10 = 1;
			}
			//fix hqsound
			for (int i = 0; i < hDigSoundEffectsDriver->n_samples_24; i++)
				hDigSoundEffectsDriver->samples_23[i].start_44mhz = NULL;
			//fix hqsound
		}
		if (!v10)
		{
			if (!x_BYTE_E37FC_music)
				sub_919C0();
			x_BYTE_E3798_sound_active2 = 0;
			x_BYTE_E3799_sound_card = 0;
			return;
		}
	}

	sub_8D800_sound_proc2();
	if (x_WORD_E37B6_sound_number)
	{
		if (x_WORD_E37B6_sound_number < 0x336u)
		{
			if (x_WORD_E37B6_sound_number < 0x320u)
			{
				if (!x_WORD_E37B6_sound_number)
				{
					x_BYTE_E3798_sound_active2 = 0;
					x_BYTE_E3799_sound_card = 0;
					myprintf("\nCould not load any digital samples\n");
					return;
				}
			}
			else if (x_WORD_E37B6_sound_number <= 0x320u)
			{
				x_DWORD_E37BC_sound_frequence = 11025;
				x_BYTE_E37AE = 0;
			}
			else if (x_WORD_E37B6_sound_number == 811)
			{
				x_DWORD_E37BC_sound_frequence = 11025;
				x_BYTE_E37AE = 0;
			}
		}
		else if (x_WORD_E37B6_sound_number <= 0x336u)
		{
			x_DWORD_E37BC_sound_frequence = 22050;
			x_BYTE_E37AE = 0;
		}
		else if (x_WORD_E37B6_sound_number < 0x656u)
		{
			if (x_WORD_E37B6_sound_number == 1611)
			{
				x_DWORD_E37BC_sound_frequence = 11025;
				x_BYTE_E37AE = 1;
			}
		}
		else if (x_WORD_E37B6_sound_number <= 0x656u)
		{
			x_DWORD_E37BC_sound_frequence = 22050;
			x_BYTE_E37AE = 1;
		}
		else if (x_WORD_E37B6_sound_number == 1644)
		{
			x_DWORD_E37BC_sound_frequence = 44100;
			x_BYTE_E37AE = 1;
		}
		sub_91A80_AIL_set_preference(0, 200);
		sub_91A80_AIL_set_preference(1, x_DWORD_E37BC_sound_frequence);
		sub_91A80_AIL_set_preference(3, 100);
		sub_91A80_AIL_set_preference(4, x_DWORD_E3794_sound_buffer3_lenght + 1);
		sub_91A80_AIL_set_preference(5, 127);
		sub_91A80_AIL_set_preference(6, 655);
		if (x_BYTE_E379C)
			sub_91A80_AIL_set_preference(7, 1);
		else
			sub_91A80_AIL_set_preference(7, 0);
		if (!x_BYTE_E379B)
			sub_91A80_AIL_set_preference(17, 0);
		if (x_BYTE_E37AE)
		{
			sub_91A80_AIL_set_preference(8, 1);
			sub_91A80_AIL_set_preference(9, 1);
		}
		else
		{
			sub_91A80_AIL_set_preference(8, 0);
			sub_91A80_AIL_set_preference(9, 0);
		}

		hDigSoundEffectsDriver = sub_93330_AIL_install_DIG_driver_file(soundEffectsAILSettings.driver_path, &unk_181D90);
		if (hDigSoundEffectsDriver)//
		{
			if (x_BYTE_E3798_sound_active2)
			{
				//x_DWORD_180B4C_end_sound_buffer3 = (uint8_t*)unk_180750_sound_buffer3 + 23 * x_DWORD_E3794_sound_buffer3_lenght;
				x_DWORD_180B4C_end_sound_buffer3_endindex = x_DWORD_E3794_sound_buffer3_lenght;
				//v8 = unk_180750_sound_buffer3;
				v8 = 0;
				v9 = 0;
				while (v8 < x_DWORD_180B4C_end_sound_buffer3_endindex)
				{
					unk_180750_sound_buffer3[v8] = sub_93510_AIL_allocate_sample_handle(hDigSoundEffectsDriver);
					unk_180750_sound_buffer3[v8]->len_4_5[1] = 0;
					unk_180750_sound_buffer3[v8]->start_44mhz = NULL;
					v8++;
					v9++;
				}
				x_BYTE_E379A = 1;
				sub_8E470_sound_proc17_volume(x_DWORD_E37B0);
			}
		}
		else
		{
			if (!x_BYTE_E37FC_music)
				sub_919C0();
			x_BYTE_E3798_sound_active2 = 0;
			x_BYTE_E3799_sound_card = 0;
			sub_83E80_freemem4(x_DWORD_E37A0_sound_buffer2);
			x_DWORD_E37A4_sound_buffer3 = 0;
			sub_83E80_freemem4(x_DWORD_E37A8_sound_buffer1);
		}
	}
	else
	{
		if (!x_BYTE_E37FC_music)
			sub_919C0();
		x_BYTE_E3798_sound_active2 = 0;
		x_BYTE_E3799_sound_card = 0;
		myprintf("\nCould not load any digital samples\n");
	}
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 98F5D: using guessed type x_DWORD stricmp(x_DWORD, x_DWORD);
// 99E33: using guessed type x_DWORD getenv(x_DWORD);
// 9E187: using guessed type x_DWORD sscanf(x_DWORD, x_DWORD, char);
// E3794: using guessed type int x_DWORD_E3794_sound_buffer3_lenght;
// E3798: using guessed type char x_BYTE_E3798_sound_active2;
// E3799: using guessed type char x_BYTE_E3799_sound_card;
// E379A: using guessed type char x_BYTE_E379A;
// E379B: using guessed type char x_BYTE_E379B;
// E379C: using guessed type char x_BYTE_E379C;
// E37A0: using guessed type int x_DWORD_E37A0_sound_buffer2;
// E37A4: using guessed type int x_DWORD_E37A4_sound_buffer3;
// E37A8: using guessed type int x_DWORD_E37A8_sound_buffer1;
// E37AE: using guessed type char x_BYTE_E37AE;
// E37B0: using guessed type int x_DWORD_E37B0;
// E37B4: using guessed type __int16 x_WORD_E37B4;
// E37B6: using guessed type __int16 x_WORD_E37B6_sound_number;
// E37B8: using guessed type char x_BYTE_E37B8;
// E37BC: using guessed type int x_DWORD_E37BC_sound_frequence;
// E37FC: using guessed type char x_BYTE_E37FC;
// E37FD: using guessed type char x_BYTE_E37FD;
// E37FE: using guessed type char x_BYTE_E37FE;
// 180B30: using guessed type __int16 x_WORD_180B30;
// 180B48: using guessed type int x_DWORD_180B48;
// 180B4C: using guessed type int x_DWORD_180B4C_end_sound_buffer3;

//----- (0008D800) --------------------------------------------------------
void sub_8D800_sound_proc2()//26E800
{
	signed int v0; // [esp+4h] [ebp-4h]

	v0 = 1;
	while (v0)
	{
		v0 = sub_84300_load_sound(x_BYTE_E37AC);
		if (v0)
		{
			if (x_WORD_E37B6_sound_number < 0x336u)
			{
				if (x_WORD_E37B6_sound_number >= 0x320u)
				{
					if (x_WORD_E37B6_sound_number <= 0x320u)
					{
						x_WORD_E37B6_sound_number = 0;
						return;
					}
					if (x_WORD_E37B6_sound_number == 811)
						x_WORD_E37B6_sound_number = 800;
				}
			}
			else if (x_WORD_E37B6_sound_number <= 0x336u)
			{
				x_WORD_E37B6_sound_number = 1611;
			}
			else if (x_WORD_E37B6_sound_number < 0x656u)
			{
				if (x_WORD_E37B6_sound_number == 1611)
					x_WORD_E37B6_sound_number = 811;
			}
			else if (x_WORD_E37B6_sound_number <= 0x656u)
			{
				x_WORD_E37B6_sound_number = 822;
			}
			else if (x_WORD_E37B6_sound_number == 1644)
			{
				x_WORD_E37B6_sound_number = 1622;
			}
		}
	}
}
// E37AC: using guessed type char x_BYTE_E37AC;
// E37B6: using guessed type __int16 x_WORD_E37B6_sound_number;

//----- (0008D8F0) --------------------------------------------------------
void sub_8D8F0_sound_proc3_endsample()//26e8f0
{
	uint32_t i; // [esp+0h] [ebp-4h]
	//i fix
	if (x_BYTE_E3798_sound_active2 && x_BYTE_E3799_sound_card)//ma to byt 0 a 0 2b4798
	{
		sub_8F850_sound_proc22();//270850
		for (i = 0; i < x_DWORD_180B4C_end_sound_buffer3_endindex; i++)
		{
			sub_93D00_AIL_end_sample(unk_180750_sound_buffer3[i]);//274d00
			/*//*((x_WORD *)i + 9) = 0;
			unk_180750_sound_buffer3[i]->sam_var[4] &= 0x0000ffff;
			//*(int *)((char *)i + 14) = 0;
			unk_180750_sound_buffer3[i]->sam_var[3] &= 0x0000ffff;
			unk_180750_sound_buffer3[i]->sam_var[4] &= 0xffff0000;
			//*((x_BYTE *)i + 20) = 0;
			//*((x_BYTE *)i + 21) = 0;
			unk_180750_sound_buffer3[i]->sam_var[5] &= 0xffff0000;*/
			unk_180750_sound_buffer3[i]->vol_scale_18[0][0] = 0;//18
			unk_180750_sound_buffer3[i]->flags_14 = 0;//14
			unk_180750_sound_buffer3[i]->vol_scale_18[0][2] = 0;//20
			unk_180750_sound_buffer3[i]->vol_scale_18[0][3] = 0;//21
		}
	}
	/*return */sub_9F040();
}
// E3798: using guessed type char x_BYTE_E3798_sound_active2;
// E3799: using guessed type char x_BYTE_E3799_sound_card;
// 180B4C: using guessed type int x_DWORD_180B4C_end_sound_buffer3;

//----- (0008D970) --------------------------------------------------------
void /*__fastcall*/ sub_8D970_init_music(/*char* a1*//*int a1, int a2, char* a3*/)//26e970
{
	//int(*v3)(int); // eax
	//char v4; // [esp+0h] [ebp-C8h]
	IO_PARMS v5 = {}; // [esp+90h] [ebp-38h]
	char v6; // [esp+A8h] [ebp-20h]
	char* v7; // [esp+B8h] [ebp-10h]
	char v8; // [esp+C0h] [ebp-8h]
	char soundCdPath[MAX_PATH];
	char soundGamePath[MAX_PATH];

	sprintf(soundCdPath, "%s/SOUND", cdDataPath);
	sprintf(soundGamePath, "%s/SOUND", gameDataPath);

	//errno_t v7x;
	//fix it
	v7 = 0;
	//fix it

	//char v8x[200];
	//IO_PARMS v9x = {};

	v8 = 0;
	if (!x_BYTE_E37FC_music)
		return;
	if (!x_BYTE_E37B8)
	{
		//sub_9D3F0_set_malloc_type(sub_83CD0_malloc2); fix it
		//v3 = sub_9D420((int (*)(int))sub_83E80_freemem4);
		//((void (/*__fastcall*/ *)(int (*)(int)))loc_917D0)(v3);
		x_BYTE_E37B8 = 1;
	}
	sub_91A80_AIL_set_preference(11, 120);
	sub_91A80_AIL_set_preference(12, 1);
	sub_91A80_AIL_set_preference(13, 127);
	sub_91A80_AIL_set_preference(14, 1);
	sub_91A80_AIL_set_preference(15, 0);
	sub_91A80_AIL_set_preference(16, 12);
	if (!x_BYTE_E379B)
		sub_91A80_AIL_set_preference(17, 0);

	char samplePath[MAX_PATH];
	sprintf(samplePath, "%s/SAMPLE", soundCdPath);
	sub_96AE0_AIL_set_GTL_filename_prefix((uint8_t*)samplePath);
	if (!DirExists(soundCdPath))
	{
		x_BYTE_E37FC_music = 0;
		x_BYTE_E37FD = 0;
		if (x_BYTE_E3798_sound_active2 && x_BYTE_E379A)
		{
			sub_99C10_sound_proc4();
		}
		else
		{
			x_BYTE_E3798_sound_active2 = 0;
			x_BYTE_E3799_sound_card = 0;
			sub_919C0();
		}
		return;
	}
	size_t sz = 0;
	//_dupenv_s(&v7,&sz, "MDMUSIC");
	v7 = mygetenv("MDMUSIC");
	if (v7)
	{
		sscanf((char* const)v7, "%s %x %d %d %d", (unsigned int)&v6);
		hMdiMusicDriver = sub_95850_AIL_install_MDI_driver_file(/*a1,*/ &v6, &v5);
		if (!hMdiMusicDriver)
		{
			if (!x_BYTE_E3798_sound_active2)
				sub_919C0();
			x_BYTE_E37FC_music = 0;
			x_BYTE_E37FD = 0;
			return;
		}
		sprintf(unk_180BE0, "%s", &v6);
		v8 = 1;
	}
	else
	{
		char musicMdiPath[MAX_PATH];
		sprintf(musicMdiPath, "%s/MDI.INI", soundGamePath);
		if (LoadAILFromFile(&musicAILSettings, musicMdiPath)) //driver
		{
			if (!_stricmp(musicAILSettings.driver_name, "None"))
			{
				if (!x_BYTE_E3798_sound_active2)
					sub_919C0();
				x_BYTE_E37FC_music = 0;
				x_BYTE_E37FD = 0;
				return;
			}
			if (!sub_95710_AIL_install_MDI_INI(&hMdiMusicDriver, musicMdiPath))
				v8 = 1;
		}
		if (!v8)
		{
			if (!x_BYTE_E3798_sound_active2)
				sub_919C0();
			x_BYTE_E37FC_music = 0;
			x_BYTE_E37FD = 0;
			return;
		}
	}

	m_hSequence = sub_95A30_AIL_allocate_sequence_handle(hMdiMusicDriver);//driver
	if (!_stricmp(musicAILSettings.driver_name, "ADLIB.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
	LABEL_69:
		if (x_BYTE_E37FC_music && LoadMusic(x_BYTE_E3814))
		{
			if (x_BYTE_E3815 == 1)
			{
				sub_9FA80();
				x_BYTE_E3815 = 0;
			}
			sub_959A0_AIL_uninstall_MDI_driver(hMdiMusicDriver);
			if (!x_BYTE_E3798_sound_active2)
				sub_919C0();
			x_BYTE_E37FC_music = 0;
			x_BYTE_E37FD = 0;
			myprintf("\nError opening music files\n");
		}
		else
		{
			x_BYTE_E37FE = 1;
			sub_8E410_sound_proc16_xmidivolume(x_DWORD_E37F8_midi_volume);
		}
		return;
	}
	if (!_stricmp(musicAILSettings.driver_name, "ADLIBG.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "OPL3.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "PAS.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "PASPLUS.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBLASTER.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBPRO1.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBPRO2.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "ESFM.MDI"))
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SBAWE32.MDI"))
	{
		x_BYTE_180C84_drivertype = 'w';
		sub_9F740((char*)"Bullfrog");
		if (!x_BYTE_E3815)
			x_BYTE_180C84_drivertype = 'g';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "MPU401.MDI"))
	{
		x_BYTE_180C84_drivertype = 'g';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "SNDSCAPE.MDI"))
	{
		x_BYTE_180C84_drivertype = 'g';
		goto LABEL_69;
	}
	if (!_stricmp(musicAILSettings.driver_name, "MT32MPU.MDI"))
	{
		x_BYTE_180C84_drivertype = 'r';
		goto LABEL_69;
	}
	if (!sub_969A0_AIL_MDI_driver_type(hMdiMusicDriver))
	{
		x_BYTE_180C84_drivertype = 'g';
		goto LABEL_69;
	}
	if (sub_969A0_AIL_MDI_driver_type(hMdiMusicDriver) == 1 || sub_969A0_AIL_MDI_driver_type(hMdiMusicDriver) == 2)
	{
		x_BYTE_180C84_drivertype = 'f';
		goto LABEL_69;
	}
	sub_959A0_AIL_uninstall_MDI_driver(hMdiMusicDriver);
	if (!x_BYTE_E3798_sound_active2)
		sub_919C0();
	x_BYTE_E37FC_music = 0;
	x_BYTE_E37FD = 0;
	myprintf("\nError driver not supported\n");
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// 98F5D: using guessed type x_DWORD stricmp(x_DWORD, x_DWORD);
// 99E33: using guessed type x_DWORD getenv(x_DWORD);
// 9A050: using guessed type x_DWORD strcmp(x_DWORD, x_DWORD);
// 9E187: using guessed type x_DWORD sscanf(x_DWORD, x_DWORD, char);
// E3798: using guessed type char x_BYTE_E3798_sound_active2;
// E3799: using guessed type char x_BYTE_E3799_sound_card;
// E379A: using guessed type char x_BYTE_E379A;
// E379B: using guessed type char x_BYTE_E379B;
// E37B8: using guessed type char x_BYTE_E37B8;
// E37F8: using guessed type int x_DWORD_E37F8_midi_volume;
// E37FC: using guessed type char x_BYTE_E37FC;
// E37FD: using guessed type char x_BYTE_E37FD;
// E37FE: using guessed type char x_BYTE_E37FE;
// E3814: using guessed type char x_BYTE_E3814;
// E3815: using guessed type char x_BYTE_E3815;
// 180C78: using guessed type int x_DWORD_180C78;
// 180C7C: using guessed type int x_DWORD_180C7C;
// 180C84: using guessed type char x_BYTE_180C84_drivertype;
// 181BE0: using guessed type int x_DWORD_181BE0;

//----- (0008E020) --------------------------------------------------------
void sub_8E020_sound_proc14_stopsequence()//26f020
{
	if (x_BYTE_E37FC_music && x_BYTE_E37FD && x_WORD_E3802)
	{
		if (x_BYTE_E3818)
			;// sub_92DC0_AIL_release_timer_handle(x_DWORD_180C80);
		x_BYTE_E3819 = 0;
		x_BYTE_E3818 = 0;
		x_BYTE_E3817 = 1;
		x_BYTE_E3816 = 0;
		x_BYTE_E381A = -1;
		if (sub_96170_AIL_sequence_status(m_hSequence) != 2)
		{
			sub_95DE0_AIL_stop_sequence(m_hSequence);
			sub_95F00_AIL_end_sequence(m_hSequence);
		}
		x_WORD_E3802 = 0;
	}
}
// E37FC: using guessed type char x_BYTE_E37FC;
// E37FD: using guessed type char x_BYTE_E37FD;
// E3802: using guessed type __int16 x_WORD_E3802;
// E3816: using guessed type char x_BYTE_E3816;
// E3817: using guessed type char x_BYTE_E3817;
// E3818: using guessed type char x_BYTE_E3818;
// E3819: using guessed type char x_BYTE_E3819;
// E381A: using guessed type char x_BYTE_E381A;
// 180C78: using guessed type int x_DWORD_180C78;
// 180C80: using guessed type int x_DWORD_180C80;

//----- (0008E160) --------------------------------------------------------
void sub_8E160_sound_proc15_startsequence(__int16 track, unsigned __int8 volume)//26f160
{
	//unsigned __int8 i; // [esp+0h] [ebp-4h]

	if (x_BYTE_E37FC_music && x_BYTE_E37FD && track <= m_iNumberOfTracks && x_WORD_E3802 != track)
	{
		if (x_BYTE_E3818)
			;// sub_92DC0_AIL_release_timer_handle(x_DWORD_180C80);
		x_BYTE_E3819 = 0;
		x_BYTE_E3818 = 0;
		x_BYTE_E3817 = 1;
		x_BYTE_E3816 = 0;
		x_BYTE_E381A = -1;
		if (x_WORD_E3802)
		{
			if (sub_96170_AIL_sequence_status(m_hSequence) != 2)
			{
				sub_95DE0_AIL_stop_sequence(m_hSequence);
				sub_95F00_AIL_end_sequence(m_hSequence);
			}
			x_WORD_E3802 = 0;
		}
		sub_95C00_AIL_init_sequence(m_hSequence, (uint8_t*) * (uint32_t*)(x_DWORD_E3808_music_header + 32 * track + 18), 0, track);
		sub_97670_AIL_register_trigger_callback(m_hSequence, reinterpret_cast<void*>(sub_8E0D0));
		/*for ( i = 0; i < 0x10u; i++ )
		{
		  x_BYTE_180C90[i] = 0;
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xB0, 0, 0);
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xB0, 7u, 0);
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xE0, 0, 64);
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xB0, 0xBu, 127);
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xB0, 1u, 0);
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xB0, 0x5Bu, 0);
		  sub_98360_AIL_send_channel_voice_message(x_DWORD_180C7C, x_DWORD_180C78, i | 0xB0, 0x5Du, 0);
		}*/
		if (volume < 0x7Fu)
			SetAilSequenceVolume(m_hSequence, volume, 0);

		sub_95D50_AIL_start_sequence(m_hSequence, track);
		x_WORD_E3800 = 100;
		x_WORD_E3802 = track;
	}
}
// 8E0D0: using guessed type int sub_8E0D0();
// E37FC: using guessed type char x_BYTE_E37FC;
// E37FD: using guessed type char x_BYTE_E37FD;
// E3800: using guessed type __int16 x_WORD_E3800;
// E3802: using guessed type __int16 x_WORD_E3802;
// E3804: using guessed type __int16 x_WORD_E3804;
// E3808: using guessed type int x_DWORD_E3808_music_header;
// E3816: using guessed type char x_BYTE_E3816;
// E3817: using guessed type char x_BYTE_E3817;
// E3818: using guessed type char x_BYTE_E3818;
// E3819: using guessed type char x_BYTE_E3819;
// E381A: using guessed type char x_BYTE_E381A;
// 180C78: using guessed type int x_DWORD_180C78;
// 180C7C: using guessed type int x_DWORD_180C7C;
// 180C80: using guessed type int x_DWORD_180C80;

//----- (0008E410) --------------------------------------------------------
void sub_8E410_sound_proc16_xmidivolume(int32_t master_volume)//26f410
{
	if (x_BYTE_E37FC_music)
	{
		if (x_BYTE_E37FE)
		{
			if (master_volume != x_DWORD_E37F8_midi_volume && master_volume <= 127 && master_volume >= 0)
			{
				sub_96670_AIL_set_XMIDI_master_volume(hMdiMusicDriver, master_volume);
				x_DWORD_E37F8_midi_volume = master_volume;
			}
		}
	}
}
// E37F8: using guessed type int x_DWORD_E37F8_midi_volume;
// E37FC: using guessed type char x_BYTE_E37FC;
// E37FE: using guessed type char x_BYTE_E37FE;
// 180C7C: using guessed type int x_DWORD_180C7C;

//----- (0008E470) --------------------------------------------------------
int sub_8E470_sound_proc17_volume(int a1)//26f470
{
	int result = 0; // eax

	if (x_BYTE_E3798_sound_active2)
	{
		if (x_BYTE_E379A)//2b479a
		{
			result = a1;
			if (a1 != x_DWORD_E37B0 && a1 <= 127 && a1 >= 0)
			{
				sub_94650_AIL_set_digital_master_volume((x_DWORD*)hDigSoundEffectsDriver, a1);
				result = a1;
				x_DWORD_E37B0 = a1;
			}
		}
	}
	return result;
}
// E3798: using guessed type char x_BYTE_E3798_sound_active2;
// E379A: using guessed type char x_BYTE_E379A;
// E37B0: using guessed type int x_DWORD_E37B0;
// 180B48: using guessed type int x_DWORD_180B48;
