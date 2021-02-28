#include "Animation.h"

__int16 x_WORD_E12FC = 1; // weak
__int16 x_WORD_D4004 = 0; // weak//index of act key in anim
__int16 x_WORD_17DB58; // weak
uint16_t ActualKeyframe_17DB60; // weak
TColor* x_DWORD_E12F4x = 0; // weak
FILE* x_DWORD_17DB38_intro_file_handle; // weak
uint8_t unk_17DB40[12]; // weak

int16_t LastKeyframe_17DB46; // weak
int16_t x_WORD_17DB48; // weak
int16_t x_WORD_17DB4A; // weak

int x_DWORD_E1300 = 0; // weak

__int16 x_WORD_17DB5A; // weak

__int16 x_WORD_17DB5C; // weak

int x_DWORD_EA3B4; // bool

__int16 x_WORD_F42AE; // weak

__int16 x_WORD_F42A8; // weak

char x_BYTE_D41C0 = 0; // weak

int x_DWORD_17D730; // weak

uint32_t x_DWORD_17D720[4]; // weak 0

uint16_t x_WORD_17D724; // weak 4
uint16_t x_WORD_17D726; // weak 6

int x_DWORD_17DB54_game_turn2; // weak

char x_BYTE_17D738[0x100]; // idb

__int16 x_WORD_E12FE = 0; // weak

//----- (00076160) --------------------------------------------------------
void PlayInfoFmv(__int16 a1, __int16 a2, type_E17CC_0* a3x, char* path)//257160
{
	//fix
	//memset(&pdwScreenBuffer[320 * 200], 0, 320*200);
	//fix
	FILE* tempfile; // eax

	x_WORD_E12FC = a2;
	x_WORD_D4004 = 0;
	x_WORD_17DB58 = 0;
	ActualKeyframe_17DB60 = 0;
	x_DWORD_E12F4x = (TColor*)pdwScreenBuffer;
	tempfile = DataFileIO::CreateOrOpenFile(path, 512);
	x_DWORD_17DB38_intro_file_handle = tempfile;
	if (tempfile)
	{
		DataFileIO::Read(tempfile, unk_17DB40, 12);//ecx=12
		LastKeyframe_17DB46 = *(int16_t*)&unk_17DB40[6];
		x_WORD_17DB48 = *(int16_t*)&unk_17DB40[8];
		x_WORD_17DB4A = *(int16_t*)&unk_17DB40[0xa];

		x_WORD_180744_mouse_right_button = 0;
		x_WORD_180746_mouse_left_button = 0;
		x_DWORD_E1300 += 12;
		LastPressedKey_1806E4 = 0;
		x_WORD_17DB5A = 0;
		FlvInitSet_473B0();//2283b0
		x_WORD_17DB5C = a1;
		do
		{
			if (x_WORD_17DB5A)
				break;
			if (ActualKeyframe_17DB60 >= LastKeyframe_17DB46 - 1)//34eb60 a 34eb46
				break;
			sub_1B280_run_intro_events(a3x);//1fc280 - draw text maybe
			sub_75DB0();//256db0 - read header

			sub_75E70();//256e70 - draw intro frame

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
void sub_1B280_run_intro_events(type_E17CC_0* a1x)//1fc280
{
	int v1; // eax
	//type_E17CC_0* v2x; // edx
	uint8_t v3; // dl

	while (1)
	{
		v1 = 7 * x_WORD_D4004;//2a5004
		//v2x = &a1x[x_WORD_D4004];
		if (ActualKeyframe_17DB60 != a1x[x_WORD_D4004].word_0)
			break;
		v1 = a1x[x_WORD_D4004].dword_3;
		if (v1 == -1)
			break;
		v3 = a1x[x_WORD_D4004].byte_2 - 0x41;
		if (v3 <= 0x39u)
		{
			switch (v3 * 4) {
			case 0x00:
				sub_loc_1B2E6();
				break;
			case 0x04:
				sub_loc_1B2F7(a1x);
				break;
			case 0x08:
				sub_loc_1B5CB();
				break;
			case 0x0c:
				sub_loc_1B51E(a1x);
				break;
			case 0x10:
				sub_loc_1B316(v1);//test it
				break;
			case 0x14:
				sub_loc_1B461(v1, a1x);//test it
				break;
			case 0x18:
				sub_loc_1B5CB();
				break;
			case 0x1c:
				sub_loc_1B5CB();
				break;
			case 0x20:
				sub_loc_1B5CB();
				break;
			case 0x24:
				sub_loc_1B5CB();
				break;
			case 0x28:
				sub_loc_1B36C();
				break;
			case 0x2c:
				sub_loc_1B334(a1x);
				break;
			case 0x30:
				sub_loc_1B352(a1x);
				break;
			case 0x34:
				sub_loc_1B5CB();
				break;
			case 0x38:
				sub_loc_1B4DF(v1, a1x);
				break;
			case 0x3c:
				sub_loc_1B4A0(v1, a1x);
				break;
			case 0x40:
				sub_loc_1B37D(a1x);
				break;
			case 0x44:
				sub_loc_1B398(a1x);
				break;
			case 0x48:
				sub_loc_1B3C9(a1x);
				break;
			case 0x4c:
				sub_loc_1B424(v1, a1x);
				break;
			case 0x50:
				sub_loc_1B5BC(v1);
				break;
			case 0x54:
				sub_loc_1B589();
				break;
			case 0x58:
				sub_loc_1B36C();
				break;
			case 0x5c:
				sub_loc_1B450();
				break;
			case 0x60:
				sub_loc_1B5A7();
				break;
			case 0x64:
				sub_loc_1B352(a1x);
				break;
			case 0x68:
				sub_loc_1B5CB();
				break;
			case 0x6c:
				sub_loc_1B5CB();
				break;
			case 0x70:
				sub_loc_1B5CB();
				break;
			case 0x74:
				sub_loc_1B5CB();
				break;
			case 0x78:
				sub_loc_1B5CB();
				break;
			case 0x7c:
				sub_loc_1B5CB();
				break;
			case 0x80:
				sub_loc_1B2E6();
				break;
			case 0x84:
				sub_loc_1B2F7(a1x);
				break;
			case 0x88:
				sub_loc_1B5CB();
				break;
			case 0x8c:
				sub_loc_1B51E(a1x);
				break;
			case 0x90:
				sub_loc_1B316(v1);
				break;
			case 0x94:
				sub_loc_1B461(v1, a1x);
				break;
			case 0x98:
				sub_loc_1B5CB();
				break;
			case 0x9c:
				sub_loc_1B54A(v1, a1x);
				break;
			case 0xa0:
				sub_loc_1B5CB();
				break;
			case 0xa4:
				sub_loc_1B5CB();
				break;
			case 0xa8:
				sub_loc_1B5CB();
				break;
			case 0xac:
				sub_loc_1B334(a1x);
				break;
			case 0xb0:
				sub_loc_1B352(a1x);
				break;
			case 0xb4:
				sub_loc_1B5CB();
				break;
			case 0xb8:
				sub_loc_1B4DF(v1, a1x);
				break;
			case 0xbc:
				sub_loc_1B4A0(v1, a1x);
				break;
			case 0xc0:
				sub_loc_1B5CB();
				break;
			case 0xc4:
				sub_loc_1B398(a1x);
				break;
			case 0xc8:
				sub_loc_1B3C9(a1x);
				break;
			case 0xcc:
				sub_loc_1B424(v1, a1x);
				break;
			case 0xd0:
				sub_loc_1B5CB();
				break;
			case 0xd4:
				sub_loc_1B5CB();
				break;
			case 0xd8:
				sub_loc_1B5CB();
				break;
			case 0xdc:
				sub_loc_1B450();
				break;
			case 0xe0:
				sub_loc_1B5CB();
				break;
			case 0xe4:
				sub_loc_1B352(a1x);
				break;
			default:
				exit(0);
			}
		}
		x_WORD_D4004++;
	}
}

//int tempedx, tempeax,tempebx;
//eax=d4004*7
//edx=esi+d4004*7
//dl=[edx+2] - +2
//eax=tempebx +3
//void sub_loc_1B289() {}
void sub_loc_1B2E6() {//1fc2e6
	//x_DWORD_E3844, eax
	x_DWORD_E3844 = 0;
	//x_WORD_D4004++;
}
void sub_loc_1B2F7(type_E17CC_0* a2x) {//1fc2f7//LoadMusic
	sub_8E020_sound_proc14_stopsequence();
	//LoadMusic(*(uint32_t*)(a2 + x_WORD_D4004 * 7 + 3));
	LoadMusic(a2x[x_WORD_D4004].dword_3);
	//x_WORD_D4004++;
}
void sub_loc_1B316(uint8_t a1) {//1fc316//LoadSound
	sub_8D8F0_sound_proc3_endsample();
	sub_84300_load_sound(a1);
	//x_WORD_D4004++;
}
void sub_loc_1B334(type_E17CC_0* a2x) {//1fc334
	if (x_DWORD_E387C)sub_loc_1B352(a2x);
	else
	{
		sub_8F0AB(x_FILE_E3840, /**(uint32_t*)(a2 + x_WORD_D4004 * 7 + 2),*/a2x[x_WORD_D4004].dword_3/* *(uint32_t*)(a2 + x_WORD_D4004 * 7 + 3)*/);
	}
}
void sub_loc_1B352(type_E17CC_0* a2x) {//1fc352
	sub_8E160_sound_proc15_startsequence(a2x[x_WORD_D4004].dword_3, 0x7f);
	//x_WORD_D4004++;
}
void sub_loc_1B36C() {//1fc36c
	sub_2EC30_clear_img_mem();
	//x_WORD_D4004++;
}
void sub_loc_1B37D(type_E17CC_0* a2x) {//1fc37d // Draw text
	sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[a2x[x_WORD_D4004].byte_2]);
	//x_WORD_D4004++;
}
void sub_loc_1B398(type_E17CC_0* a2x) {//1fc398
	if (x_BYTE_E3798_sound_active2)sub_loc_1B5CB();
	sub_8F100_sound_proc19(0, a2x[x_WORD_D4004].dword_3, 0x7f, 0x40, 0x64, 0xFFFFFFFF, 2);
	//x_WORD_D4004++;
}
void sub_loc_1B3C9(type_E17CC_0* a1x) {//1fc3c9
	if (x_BYTE_E3798_sound_active2)sub_loc_1B5CB();
	//uint8_t* v1 = a1 + x_WORD_D4004 * 7 + 3;
	if (a1x[x_WORD_D4004].dword_3 == 0)sub_loc_1B413();//tempesi = 2b27cc
	sub_8F100_sound_proc19(0, a1x[x_WORD_D4004].dword_3, 0x7f, 0x40, 0x64, 0, 2);
	//x_WORD_D4004++;
}
void sub_loc_1B413() {//1fc413
	sub_8D8F0_sound_proc3_endsample();
	//x_WORD_D4004++;
}
void sub_loc_1B424(uint16_t a1, type_E17CC_0* a2x) {//1fc424
	if (x_BYTE_E3798_sound_active2 == 0)sub_loc_1B5BC(a1);
	if (a2x[x_WORD_D4004].dword_3)sub_loc_1B413();
	sub_8F420_sound_proc20(0, a2x[x_WORD_D4004].dword_3);
	//x_WORD_D4004++;
}
void sub_loc_1B450() {//1fc450
	sub_8E020_sound_proc14_stopsequence();
	//x_WORD_D4004++;
}
void sub_loc_1B461(uint16_t a1, type_E17CC_0* a2x) {//1fc461
	if (x_BYTE_E3798_sound_active2 == 0)sub_loc_1B5BC(a1);
	sub_8F710_sound_proc21(0, a2x[x_WORD_D4004].dword_3, 0, 4, 0x64/*, 1, 2*/);
	/*cseg01 : 0001B46E                 movsx   edx, word_D4004
	cseg01 : 0001B475                 push    1
	cseg01 : 0001B477                 lea     eax, ds : 0[edx * 8]
	cseg01 : 0001B47E                 push    4
	cseg01 : 0001B480                 sub     eax, edx
	cseg01 : 0001B482                 push    0
	cseg01 : 0001B484                 movsx   eax, word ptr[esi + eax + 3]
	cseg01 : 0001B489                 push    eax
	cseg01 : 0001B48A                 push    0
	cseg01 : 0001B48C                 call    sub_8F710_sound_proc21*/
	//x_WORD_D4004++;
}
void sub_loc_1B4A0(uint16_t a1, type_E17CC_0* a2x) {//1fc4a0
	if (x_BYTE_E3798_sound_active2 == 0)sub_loc_1B5BC(a1);
	sub_8F710_sound_proc21(0, a2x[x_WORD_D4004].dword_3, 50, 2, 0);
	/*cseg01 : 0001B4AD                 movsx   edx, word_D4004
	cseg01 : 0001B4B4                 push    0
	cseg01 : 0001B4B6                 lea     eax, ds : 0[edx * 8]
	cseg01 : 0001B4BD                 push    2
	cseg01 : 0001B4BF                 sub     eax, edx
	cseg01 : 0001B4C1                 push    50h; 'P'
	cseg01:0001B4C3                 movsx   eax, word ptr[esi + eax + 3]
	cseg01 : 0001B4C8                 push    eax
	cseg01 : 0001B4C9                 push    0
	cseg01 : 0001B4CB                 call    sub_8F710_sound_proc21*/
	//x_WORD_D4004++;
}
void sub_loc_1B4DF(uint16_t a1, type_E17CC_0* a2x) {//1fc4df
	if (x_BYTE_E3798_sound_active2 == 0)sub_loc_1B5BC(a1);
	sub_8F710_sound_proc21(0, a2x[x_WORD_D4004].dword_3, 0x7f, 2, 0);
	/*	cseg01 : 0001B4EC                 movsx   edx, word_D4004
		cseg01 : 0001B4F3                 push    0
		cseg01 : 0001B4F5                 lea     eax, ds : 0[edx * 8]
		cseg01 : 0001B4FC                 push    2
		cseg01 : 0001B4FE                 sub     eax, edx
		cseg01 : 0001B500                 push    7Fh
		cseg01 : 0001B502                 movsx   eax, word ptr[esi + eax + 3]
		cseg01 : 0001B507                 push    eax
		cseg01 : 0001B508                 push    0
		cseg01 : 0001B50A                 call    sub_8F710_sound_proc21*/
		//x_WORD_D4004++;
}
void sub_loc_1B51E(type_E17CC_0* a1x) {//1fc51e
	sub_8E160_sound_proc15_startsequence(a1x[x_WORD_D4004].dword_3, 0x64/*, tempedx * 7*/);
	//x_WORD_D4004++;
}
void sub_loc_1B54A(uint16_t a1, type_E17CC_0* a2x) {//1fc54a
	if (x_BYTE_E3798_sound_active2 == 0)sub_loc_1B5BC(a1);
	sub_8F100_sound_proc19(0, a2x[x_WORD_D4004].dword_3, 0, 0x40, 0x64, 0xFFFFFFFF, 2);
	//x_WORD_D4004++;
}
void sub_loc_1B589() {//1fc589//Draw 40 lines after normal image
	if (x_BYTE_D41C0 == 0)sub_loc_1B5CB();
	else
	{
		sub_2EB60();//maybe must fix
		//x_WORD_D4004++;
	}
}
void sub_loc_1B5A7() {//1fc5a7
	sub_2EC60();
	//x_WORD_D4004++;
}
void sub_loc_1B5BC(uint16_t a1) {//1fc5bc
	//memset(pdwScreenBuffer, 200, 320*200);//debug
	//Set_basic_pallette();
	if ((a1 != 1) && (a1 != 0xe))//fixed write pause text
		sub_2EBB0_draw_text_with_border_630x340(x_DWORD_E9C4C_langindexbuffer[a1]);//fix it - edx
	//sub_90478_VGA_Blit320();//debug
}
void sub_loc_1B5CB() {//1fc5cb
	//x_WORD_D4004++;
}
//void sub_loc_1B5D7() {}

//----- (00075DB0) --------------------------------------------------------
void sub_75DB0()//256db0
{
	x_DWORD_17D730 = x_DWORD_E1300;
	DataFileIO::Read(x_DWORD_17DB38_intro_file_handle, (uint8_t*)x_DWORD_17D720, 16);
	x_WORD_17D724 = x_DWORD_17D720[1] & 0xffff;
	x_WORD_17D726 = (x_DWORD_17D720[1] & 0xffff0000) >> 16;
	while (x_WORD_17D724 != 0xf1fa/*-3590*/)
		myprintf("ERROR UNKNOWN FRAME TYPE\n");
	DataFileIO::Read(x_DWORD_17DB38_intro_file_handle, x_DWORD_E9C38_smalltit, x_DWORD_17D720[0] - 16);
	x_DWORD_E1300 += x_DWORD_17D720[0];
}

int(/*__fastcall*/ *x_DWORD_17DB3C)(); // weak

//----- (00075E70) --------------------------------------------------------
void /*__fastcall*/ sub_75E70()//256e70
{
	//int v1; // eax
	unsigned int v2; // ebx
	char* v3; // esi
	char* v4; // edi
	//char v5; // al
	//char v6; // al
	char* v7; // esi
	char* v8; // esi
	char* v9; // edi
	//char v10; // al
	//char v11; // al
	char* v12; // esi
	char* v13; // edi
	//char v14; // al
	//char v15; // al
	char* v16; // edi
	//char v17; // al
	//char v18; // al
	char v19; // al
	int16_t v20x[3]; // [esp+0h] [ebp-10h]
	//uint32_t v20y; // [esp+0h] [ebp-10h]
	int v21; // [esp+4h] [ebp-Ch]
	uint8_t* v22; // [esp+8h] [ebp-8h]
	char v23; // [esp+Ch] [ebp-4h]

	//fix it
	v21 = 0;
	x_DWORD_17DB54_game_turn2 = 0x40;
	//fix it

	//HIBYTE(a1) = 0;
	v23 = 0;
	x_DWORD_17DB50 = x_DWORD_E9C38_smalltit;
	x_BYTE_17D738[0] = 0;
	if (x_WORD_17D724 == 0xf100)
	{
		sub_75D70(0, x_DWORD_17D720[0] - 16);
		/*v1 = */sub_75DB0();
		/*a1 = 0;*/sub_75E70(/*v1*/);
	}
	else if (x_WORD_17D724 == 0xF1FA)
	{
		v2 = 0;
		while (1)
		{
			//a1 = x_WORD_17D726;
			if (v2 >= x_WORD_17D726)
				break;
			v22 = x_DWORD_17DB50;
			sub_75D70((int8_t*)v20x, 6);
			//v20y = *(uint32_t*)&v20x;			
			switch (v20x[2])//4? b
			{
			case 4:
				v3 = (char*)"COLOUR256 ";
				sub_76260_read_intro_pallette(0x100);
				v4 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v4,v3);
				v23 = 1;
				break;
			case 7:
				v7 = (char*)"SS2 ";
				sub_76300();//257300 - uz by mel byt vykreslen text
				v16 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v16, v7);
				break;
			case 0xB:
				sub_76260_read_intro_pallette(0x40);
				v8 = (char*)"COLOUR ";
				v9 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v9, v8);
				v23 = 1;
				break;
			case 0xC:
				v7 = (char*)"LC ";
				sub_76430();
				v16 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v16, v7);
				break;
			case 0xD:
				memset((void*)x_DWORD_E12F4x, 0, x_WORD_17DB48 * x_WORD_17DB4A);
				v7 = (char*)"BLACK ";
				v16 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v16, v7);
				break;
			case 0xF:
				v7 = (char*)"BRUN ";//ok
				sub_76540();//257540
				v16 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v16, v7);
				break;
			case 0x10:
				sub_75D70((int8_t*)x_DWORD_E12F4x, x_WORD_17DB4A * x_WORD_17DB48);
				v12 = (char*)"COPY ";
				v13 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v12, v13);
				//v20y = x_WORD_17DB4A * x_WORD_17DB48;
				break;
			case 0x12:
				/*fix
					sub_75D70(0, *(uint32_t*)&v20x - 6);
				*///may be is problem
				v7 = (char*)"PSTAMP ";
			//LABEL_23:
				v16 = &x_BYTE_17D738[strlen(x_BYTE_17D738)];
				strcpy(v16, v7);
				break;
			default:
				break;
			}
			v2++;
			x_DWORD_17DB50 = *(uint32_t*)&v20x + v22;
		}
	}
	if (x_DWORD_17DB3C)
		x_DWORD_17DB3C();
	sub_75CB0();//256cb0
	if (v23)
	{
		sub_9A0FC_wait_to_screen_beam();//27b0fc
		if (x_WORD_E12FC)
		{
			/*uint8_t origbyte = 0;
			uint8_t remakebyte = 0;
			long compar = compare_with_snapshot((char*)"0160-00256E70", unk_17D838, 0x34e838, 0x300, &origbyte, &remakebyte);
			*/

			sub_41A90_VGA_pallette_install(unk_17D838x);
			v19 = sub_5BE80_test_pallette(unk_17D838x, 0x3Fu, 0x3Fu, 0x3Fu);
			sub_2EC90(v19);//20fc90 -zde se prekresli texty
		}
	}
	int tempSpeed = speedGame;
	speedGame = speedAnim;
	if (x_BYTE_D41C1)
	{
		pdwScreenBuffer += 0x26C0;
		sub_90478_VGA_Blit320();
		pdwScreenBuffer -= 0x26C0;
	}
	else
	{
		sub_90478_VGA_Blit320();
	}
	speedGame = tempSpeed;
}

//----- (0002EC60) --------------------------------------------------------
void sub_2EC60()//20fc60
{
	if (x_BYTE_D41CE)
	{
		sub_2EC30_clear_img_mem();
		x_BYTE_D41C1 = 0;
		x_BYTE_D41CE = 0;
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
	/*result = */sub_75D70((int8_t*)&v5, 2u);
	v2 = (uint8_t*)x_DWORD_E12F4x;//2b22f4
	if (v5 > 0u)
	{
		do
		{
			v3 = v2;
			sub_75D70((int8_t*)&v9, 2u);
			if ((v9 & 0x8000) == 0)
			{
				//v6 = v9;
				for (i = 0; i < v9; i++)
				{
					sub_75D70((int8_t*)&v11, 1u);
					v3 += v11;
					sub_75D70((int8_t*)&v10, 1u);
					if (v10 >= 0)
					{
						if (v10 > 0)
						{
							sub_75D70((int8_t*)v3, 2 * v10);
							v3 += 2 * v10;
						}
					}
					else
					{
						sub_75D70((int8_t*)&v8, 2u);
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
				v2 += x_WORD_17DB48 * (abs(v9) - 1);
			}
			else
			{
				*(x_BYTE*)(v2 + x_WORD_17DB48 - 1) = v9;
			}
			//result = x_WORD_17DB48;
			v0++;
			v2 += x_WORD_17DB48;
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

	sub_75D70((int8_t*)&v6, 2u);
	v0 = x_WORD_17DB48 * v6 + (uint8_t*)x_DWORD_E12F4x;
	sub_75D70((int8_t*)&v6, 2u);
	v5 = 0;
	result = 0;
	if (v6 > 0u)
	{
		do
		{
			v2 = (char*)v0;
			v3 = 0;
			sub_75D70((int8_t*)&v9, 1u);
			while (v3 < (signed int)v9)
			{
				sub_75D70((int8_t*)&v7, 1u);
				v2 += v7;
				sub_75D70((int8_t*)&v8, 1u);
				if (v8 >= 0)
				{
					if (v8 <= 0)
						goto LABEL_8;
					sub_75D70((int8_t*)v2, v8);
					v4 = v8;
				}
				else
				{
					sub_75D70((int8_t*)&v10, 1u);
					memset(v2, v10, abs(v8));
					v4 = abs(v8);
				}
				v2 += v4;
			LABEL_8:
				++v3;
			}
			++v5;
			v0 += (unsigned __int16)x_WORD_17DB48;
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

	v0 = (uint8_t*)x_DWORD_E12F4x;
	v4 = 0;
	while (1)
	{
		result = v4;
		if (v4 >= x_WORD_17DB4A)
			break;
		v1 = (char*)v0;
		v2 = 0;
		sub_75D70(0, 1u);
		while (v2 < x_WORD_17DB48)
		{
			sub_75D70((int8_t*)&v6, 1u);
			if (v6 >= 0)
			{
				if (v6 > 0)
				{
					sub_75D70((int8_t*)&v5, 1u);
					memset(v1, v5, v6);
				}
			}
			else
			{
				v6 = abs(v6);
				sub_75D70((int8_t*)v1, v6);
			}
			v2 += v6;
			v1 += v6;
		}
		++v4;
		v0 += x_WORD_17DB48;
	}
	return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// E12F4: using guessed type int x_DWORD_E12F4;
// 17DB48: using guessed type __int16 x_WORD_17DB48;
// 17DB4A: using guessed type __int16 x_WORD_17DB4A;

//----- (00075CB0) --------------------------------------------------------
void sub_75CB0()//256cb0
{
	if (x_WORD_E12FE && sub_473E0())
	{
		x_WORD_17DB5A = 1;
	}
	else if (x_WORD_17DB5C && (LastPressedKey_1806E4 || x_WORD_180746_mouse_left_button || x_WORD_180744_mouse_right_button))
	{
		x_WORD_17DB5A = 1;
	}
	else
	{
		while (x_DWORD_17DB54_game_turn2 < x_DWORD_E3844)
		{
			if (x_WORD_E12FE && sub_473E0())
			{
				x_WORD_17DB5A = 1;
				return;
			}
			if (x_WORD_17DB5C && (LastPressedKey_1806E4 || x_WORD_180746_mouse_left_button || x_WORD_180744_mouse_right_button))
			{
				x_WORD_17DB5A = 1;
				return;
			}
		}
		x_DWORD_17DB54_game_turn2 = 0;
	}
}
// E12FE: using guessed type __int16 x_WORD_E12FE;
// E3844: using guessed type int x_DWORD_E3844;
// 17DB54: using guessed type int x_DWORD_17DB54_game_turn2;
// 17DB5A: using guessed type __int16 x_WORD_17DB5A;
// 17DB5C: using guessed type __int16 x_WORD_17DB5C;
// 1806E4: using guessed type char x_BYTE_1806E4;
// 180744: using guessed type __int16 x_WORD_180744_mouse_right_button;
// 180746: using guessed type __int16 x_WORD_180746_mouse_left_button;

//----- (0009A0FC) --------------------------------------------------------
void sub_9A0FC_wait_to_screen_beam()//27B0fc
{
	/*unsigned __int8 result; // al

	do
	  result = __inx_BYTE(0x3DAu);
	while ( !(result & 8) );
	return result;*/
	mydelay(10);
}

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
		if (pressedKeys_180664[0x2a] || pressedKeys_180664[0x36])
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


