#include "sub_main.h"

#include "engine/CommandLineParser.h"
#include "engine/GameUI.h"
#include "engine/Graphics.h"
#include "engine/MenusAndIntros.h"
#include "engine/Network.h"
#include "engine/Player.h"
#include "engine/PlayerInput.h"
#include "engine/engine_support.h"
#include "engine/CompareMaps.h"
#include "engine/Events.h"
#include "engine/Level.h"
#include "utilities/StateMonitor.h"
#include "utilities/RendererTests.h"
#include "engine/ConvertMapInfo.h"
#include "engine/LevelInit.h"
#include "engine/DatTabIndexes.h"
#include "engine/MouseOnOff.h"
#include "engine/PrintMessages.h"
#include "engine/ReadAndDecompress.h"

/*

//2541e7
mainConnection_E12AA 2b22aa->326f0 7f0300 .. 03
after NetworkTestCall_72FBB 350000 .. 00
after NetworkCancel_748F7 not changed

connection_E12AE[] 2b22ae->34340 b0160002 .. 16
after NetworkTestCall_72FBB 900b73 .. 0b
after NetworkCancel_748F7 not changed

*/

#if defined(__linux__) || defined(__APPLE__)
#include <strings.h>
#include <cstdlib>
#include <cstring>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define strnicmp strncasecmp
#define __cdecl
#include <ctype.h>
#include <iostream>
#include <functional>
#include <type_traits>
#else
#endif //__linux__

#include <filesystem>
#include "./engine/EventsFunctions.h"

InputRecorder* m_InputRecorder = nullptr;
Scene m_CurrentScene = Scene::PREAMBLE_MENU;

void preconvert() {
	/*
	int16_t word_0;
	int16_t word_2;
	int16_t word_4;
	int16_t word_6;
	int16_t word_8;
	int16_t word_10;
	int16_t word_12;
	int16_t word_14;
	int16_t word_16;
	int16_t word_18;
	int16_t word_20;
}
type_D404C;
#pragma pack(pop)

type_D404C str_D404C[5];

uint8_t ar_D404C[22 * 5]//particles table*/
/*
//type_E24BCx textBoxStr_E24BCx[32];
memcpy((type_E17CC_0*)str_E1328, array_E1328 , 0x100*sizeof(type_E17CC_0));
char outname[512];
char buffer[512];
sprintf(outname, "c:\\prenos\\remc2\\outstr.txt");
FILE* file = fopen(outname, "wt");
sprintf(buffer, "type_E17CC_0 str_E17CC_0[48] = {\n");
fwrite(buffer, strlen(buffer), 1, file);
for (int i = 0; i < 0x100; i++)
{
	sprintf(buffer, "{0x%04X,0x%02X,0x%08X},\n",
		str_E1328[i].word_0, str_E1328[i].byte_2, str_E1328[i].dword_3
		);

	fwrite(buffer, strlen(buffer), 1, file);
}
sprintf(buffer, "};\n");
fwrite(buffer, strlen(buffer), 1, file);
fclose(file);
*/


/*
memcpy((uint8_t*)str_x_DWORD_D4C52ar_0x2F22, x_DWORD_D4C52ar + 0x2F22, 4000);
char outname[512];
char buffer[512];
sprintf(outname, "c:\\prenos\\remc2\\outstr.txt");
FILE* file = fopen(outname, "wt");
sprintf(buffer, "type_mapScreenPortals_E17CC mapScreenPortals_E17CC[168] = {\n");
fwrite(buffer, strlen(buffer), 1, file);
for (int i = 0; i < 168; i++)
{
	sprintf(buffer, "{0x%04X,0x%02X,0x%04X,0x%04X},\n",
		str_x_DWORD_D4C52ar_0x2F22[i].dword_0, str_x_DWORD_D4C52ar_0x2F22[i].word_4, str_x_DWORD_D4C52ar_0x2F22[i].adress_6,
		str_x_DWORD_D4C52ar_0x2F22[i].dword_10);
	fwrite(buffer, strlen(buffer), 1, file);
}
sprintf(buffer, "};\n");
fwrite(buffer, strlen(buffer), 1, file);
fclose(file);*/
/*
#pragma pack(push, 1)
typedef struct {//lenght 22
	int16_t word_0;
	int16_t word_2;
	int16_t word_4;
	int16_t word_6;
	int16_t word_8;
	int16_t word_10;
	int16_t word_12_x; //X
	int16_t word_14_y; //Y
	int16_t word_16;
	int8_t byte_18_act;//1-hidden 2-activated
	int8_t byte_19;
	int16_t word_20;
}type_mapScreenPortals_E17CC;
#pragma pack(pop)
	type_mapScreenPortals_E17CC mapScreenPortals_E17CC[168] = {
	//0 2  4    6       8   10      12      14        16    18 19 20 stub
	{0, 0, 0x74, 0x1de, 28, 0xa40,   0x341, 0x213,      0,    2,  0, 0, 0 }
};

uint8_t unk_E17CC_0x194x[0xE6C] = {
	*/

	/*memcpy((uint8_t*)x_BYTE_E25ED_db_str, x_BYTE_E25ED_db, 0x1E0);
	char outname[512];
	char buffer[512];
	sprintf(outname, "c:\\prenos\\remc2\\outstr.txt");
	FILE* file = fopen(outname, "wt");
	sprintf(buffer, "type_x_BYTE_E25ED_db_str x_BYTE_E25ED_db_str[16]={\n");
	fwrite(buffer, strlen(buffer), 1, file);
	for (int i = 0; i < 17; i++)
	{
		sprintf(buffer, "{0x%08X,0x%08X,0x%04X,0x%04X,0x%04X,0x%04X,0x%04X,0x%04X,0x%02X,0x%02X,0x%04X,0x%04X,0x%04X,0x%04X},\n", x_BYTE_E25ED_db_str[i].dword_0, x_BYTE_E25ED_db_str[i].dword_4, x_BYTE_E25ED_db_str[i].word_8, x_BYTE_E25ED_db_str[i].word_10,
			x_BYTE_E25ED_db_str[i].word_12, x_BYTE_E25ED_db_str[i].word_14, x_BYTE_E25ED_db_str[i].word_16, x_BYTE_E25ED_db_str[i].word_18,
			x_BYTE_E25ED_db_str[i].byte_20, x_BYTE_E25ED_db_str[i].byte_21, x_BYTE_E25ED_db_str[i].word_22, x_BYTE_E25ED_db_str[i].word_24,
			x_BYTE_E25ED_db_str[i].word_26, x_BYTE_E25ED_db_str[i].word_28);
		fwrite(buffer, strlen(buffer), 1, file);
	}
	sprintf(buffer, "};\n");
	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file); */
	/*
	typedef struct {//lenght 30
		int32_t dword_0;
		int32_t dword_4;
		int16_t word_8;
		int16_t word_10;
		int16_t word_12;
		int16_t word_14;
		int16_t word_16;
		int16_t word_18;
		int8_t byte_20;
		int8_t byte_21;
		int16_t word_22;
		int16_t word_24;
		int16_t word_26;
		//int8_t stuba[1];
		int16_t word_28;
		//int8_t stubb[1];
	} type_x_BYTE_E25ED_db_str;

	type_x_BYTE_E25ED_db_str x_BYTE_E25ED_db_str[16]=;
	uint8_t x_BYTE_E25ED_db[0x1E0] = {
	*/
	/*memcpy((uint8_t*)&str_D4C52ar[1], x_DWORD_D4C52ar+8, 0x132);
	str_D4C52ar[0].dword_0 = 0x2a5c44;
	char outname[512];
	char buffer[512];
	sprintf(outname, "c:\\prenos\\remc2\\outstr.txt");
	FILE* file = fopen(outname,"wt");
	sprintf(buffer,"type_D4C52ar str_D4C52ar[18]={\n");
	fwrite(buffer,strlen(buffer),1,file);
	for (int i = 0; i < 17; i++)
	{
		sprintf(buffer, "0x%08X,0x%08X,0x%04X,0x%08X,0x%08X,\n", str_D4C52ar[i].dword_0, str_D4C52ar[i].dword_4, str_D4C52ar[i].word_8, str_D4C52ar[i].dword_10, str_D4C52ar[i].dword_14);
		fwrite(buffer, strlen(buffer), 1, file);
	}
	sprintf(buffer, "};\n");
	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);*/
	/*
	memcpy(x_DWORD_D4C52ar_strtest, x_DWORD_D4C52ar + 0x002A7400 - 0x2a5c52, 14*500);
	char outname[512];
	char buffer[512];
	sprintf(outname, "c:\\prenos\\remc2\\outstr.txt");
	FILE* file = fopen(outname, "wt");
	sprintf(buffer, "type_D4C52ar2 x_DWORD_D4C52ar_strA0[32]={\n");
	fwrite(buffer, strlen(buffer), 1, file);
	for (int i = 0; i < 500; i++)
	{
		sprintf(buffer, "0x%08X,0x%04X,0x%08X,0x%08X,\n", x_DWORD_D4C52ar_strtest[i].dword_0, x_DWORD_D4C52ar_strtest[i].word_4, x_DWORD_D4C52ar_strtest[i].dword_6, x_DWORD_D4C52ar_strtest[i].dword_10);
		fwrite(buffer, strlen(buffer), 1, file);
	}
	sprintf(buffer, "};\n");
	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
	*/
	//memcpy((uint8_t*)str_D4D84ar, x_DWORD_D4C52arx+0x132, 0x67da);

	/*
	memcpy((uint8_t*)& SPELLS_BEGIN_BUFFER_str, SPELLS_BEGIN_BUFFER_DA818x, 26*80);
	char outname[512];
	char buffer[512];
	sprintf(outname, "c:\\prenos\\remc2\\outstr.txt");
	FILE* file = fopen(outname, "wt");
	sprintf(buffer, "type_SPELLS_BEGIN_BUFFER_str SPELLS_BEGIN_BUFFER_str[26] =\n");
	fwrite(buffer, strlen(buffer), 1, file);
	for (int i = 0; i < 26; i++)
	{
		sprintf(buffer, "{{0x%02X,0x%02X,{\n", SPELLS_BEGIN_BUFFER_str[i].byte_0, SPELLS_BEGIN_BUFFER_str[i].byte_1);
		fwrite(buffer, strlen(buffer), 1, file);
		sprintf(buffer, "{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%04X,0x%04X,0x%02X,0x%02X},\n", SPELLS_BEGIN_BUFFER_str[i].subspell[0].dword_2, SPELLS_BEGIN_BUFFER_str[i].subspell[0].dword_6, SPELLS_BEGIN_BUFFER_str[i].subspell[0].dword_A, SPELLS_BEGIN_BUFFER_str[i].subspell[0].dword_E, SPELLS_BEGIN_BUFFER_str[i].subspell[0].dword_0x12, SPELLS_BEGIN_BUFFER_str[i].subspell[0].word_0x16x, SPELLS_BEGIN_BUFFER_str[i].subspell[0].word_0x18, SPELLS_BEGIN_BUFFER_str[i].subspell[0].byte_0x1A, SPELLS_BEGIN_BUFFER_str[i].subspell[0].byte_0x1B);
		fwrite(buffer, strlen(buffer), 1, file);
		sprintf(buffer, "{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%04X,0x%04X,0x%02X,0x%02X},\n", SPELLS_BEGIN_BUFFER_str[i].subspell[1].dword_2, SPELLS_BEGIN_BUFFER_str[i].subspell[1].dword_6, SPELLS_BEGIN_BUFFER_str[i].subspell[1].dword_A, SPELLS_BEGIN_BUFFER_str[i].subspell[1].dword_E, SPELLS_BEGIN_BUFFER_str[i].subspell[1].dword_0x12, SPELLS_BEGIN_BUFFER_str[i].subspell[1].word_0x16x, SPELLS_BEGIN_BUFFER_str[i].subspell[1].word_0x18, SPELLS_BEGIN_BUFFER_str[i].subspell[1].byte_0x1A, SPELLS_BEGIN_BUFFER_str[i].subspell[1].byte_0x1B);
		fwrite(buffer, strlen(buffer), 1, file);
		sprintf(buffer, "{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%04X,0x%04X,0x%02X,0x%02X}}},\n", SPELLS_BEGIN_BUFFER_str[i].subspell[2].dword_2, SPELLS_BEGIN_BUFFER_str[i].subspell[2].dword_6, SPELLS_BEGIN_BUFFER_str[i].subspell[2].dword_A, SPELLS_BEGIN_BUFFER_str[i].subspell[2].dword_E, SPELLS_BEGIN_BUFFER_str[i].subspell[2].dword_0x12, SPELLS_BEGIN_BUFFER_str[i].subspell[2].word_0x16x, SPELLS_BEGIN_BUFFER_str[i].subspell[2].word_0x18, SPELLS_BEGIN_BUFFER_str[i].subspell[2].byte_0x1A, SPELLS_BEGIN_BUFFER_str[i].subspell[2].byte_0x1B);
		fwrite(buffer, strlen(buffer), 1, file);
	}
	sprintf(buffer, "};\n");
	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
	*/
	/*
	int32_t dword_2;
	int32_t dword_6;
	int32_t dword_A;
	int32_t dword_E;
	int32_t dword_0x12;
	int16_t word_0x16x;
	int16_t word_0x18;
	int8_t byte_0x1A;
	uint8_t byte_0x1B;
}
type_SPELLS_BEGIN_BUFFER_str_sub;

typedef struct {//lenght 80
	int8_t byte_0;
	uint8_t byte_1;
	type_SPELLS_BEGIN_BUFFER_str_sub subspell[3];
}
type_SPELLS_BEGIN_BUFFER_str;

	type_SPELLS_BEGIN_BUFFER_str SPELLS_BEGIN_BUFFER_str[26] =
	{{1,2,{
	{3,4,0,0,0,0,0,0,0},
	{5,6,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0}}}
	};
	*/
	/*type_SPELLS_BEGIN_BUFFER_str SPELLS_BEGIN_BUFFER_str[26];

	int8_t SPELLS_BEGIN_BUFFER_DA818x[0x820] = {*/
}

void begin_plugin() {
	//convert_x_WORD_E2970y_to_x_WORD_E2970();
	//x_D41A0_BYTEARRAY_4_struct.dwordindex_2388 = 10022;//fix sound param
}

void initposistruct() {//fix it
	posistruct1 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct2 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct3 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct4 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct5 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct6 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct7 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct8 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct9 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct10 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
	posistruct11 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);

	x_DWORD_EA3D4 = (bitmap_pos_struct_t*)malloc(sizeof(bitmap_pos_struct_t) * 1700);
};

//----- (00056730) --------------------------------------------------------
int sub_56730_clean_memory()//237730
{
	return sub_8B5A0();
}

//----- (0008B5A0) --------------------------------------------------------
int sub_8B5A0()//26c5a0
{
	if (x_BYTE_E36D1)
	{
		switch (unk_18058Cstr.x_WORD_1805C2_joystick)
		{
		case 1:
			sub_8B770();
			break;
		case 9:
			sub_75900();
			break;
		default:
			break;
		}
		x_BYTE_E36D1 = 0;
	}
	return 0;
}
// E36D1: using guessed type char x_BYTE_E36D1;
// 1805C2: using guessed type __int16 x_WORD_1805C2_joystick;

//----- (00053CF0) --------------------------------------------------------
int sub_53CF0_access(const char* a1)//234cf0
{
	return myaccess(a1, 0);
}
// 98CF2: using guessed type x_DWORD access(x_DWORD, x_DWORD);

//----- (000560D0) --------------------------------------------------------
void sub_560D0_create_sound_dir()//2370d0
{
	FILE* diginifile2; // eax
	FILE* diginifile; // ebx
	FILE* mdiini; // eax
	FILE* mdiini2; // ebx
	//printbuffer char v5; // [esp+0h] [ebp-2Ah]
	//printbuffer2 char v6; // [esp+80h] [ebp+56h]
	//char v7; // [esp+A8h] [ebp+7Eh]

	//v7 = 1;
	sprintf(printbuffer, "DEVICE\t\tNone\r\nDRIVER\t\tNone\r\nIO_ADDR\t\t-1\r\nIRQ\t\t-1\r\nDMA_8_BIT\t\t-1\r\nDMA_16_BIT\t\t-1\r\n");
	std::string digPath = GetSubDirectoryFile(gameFolder.c_str(), "SOUND", "DIG.INI");
	diginifile2 = DataFileIO::CreateOrOpenFile(digPath.c_str(), 512);
	if (diginifile2 == NULL)
	{
		diginifile = DataFileIO::CreateOrOpenFile(digPath.c_str(), 546);
		if (diginifile != NULL)
		{
			DataFileIO::WriteFile_98CAA(diginifile, (uint8_t*)printbuffer, strlen(printbuffer));
			DataFileIO::Close(diginifile);
		}
	}
	else
	{
		DataFileIO::Close(diginifile2);
	}
	std::string mdiPath = GetSubDirectoryFile(gameFolder.c_str(), "SOUND", "MDI.INI");
	mdiini = DataFileIO::CreateOrOpenFile(mdiPath.c_str(), 512);
	if (mdiini == NULL)
	{
		mdiini2 = DataFileIO::CreateOrOpenFile(mdiPath.c_str(), 546);
		if (mdiini2 != NULL)
		{
			DataFileIO::WriteFile_98CAA(mdiini2, (uint8_t*)printbuffer, strlen(printbuffer));
			DataFileIO::Close(mdiini2);
		}
	}
	else
	{
		DataFileIO::Close(mdiini);
	}
	//return v7;
}
// 8E3D5: using guessed type x_DWORD sprintf(x_DWORD, const char *, ...);
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (0008B770) --------------------------------------------------------
int sub_8B770()//26c770
{
	return sub_9B234((int*)unk_18058Cstr.unk_1805CE);
}

//----- (00053EF0) --------------------------------------------------------
bool sub_53EF0_fileexist(const char* path, const char* path2)//234ef0//fix a2
{
	FILE* testfile1; // eax
	FILE* testfile2; // eax
	bool result = false; // [esp+0h] [ebp-4h]
	testfile1 = DataFileIO::CreateOrOpenFile(path, 512);
	testfile2 = DataFileIO::CreateOrOpenFile(path2, 512);
	if (testfile1 == NULL || testfile2 == NULL)
		result = true;
	if (testfile1 != NULL)
		DataFileIO::Close(testfile1);
	if (testfile2 != NULL)
		DataFileIO::Close(testfile2);
	return result;
}

//----- (0009B234) --------------------------------------------------------
int sub_9B234(int* a1)//27C234
{
	int result; // eax

	if (*a1)
	{
		//fix  sub_9C58C(*a1);
		allert_error();
		//fix
		result = x_free((void*)*a1);
		*a1 = 0;
	}
	return result;
}
// A0E6B: using guessed type x_DWORD free(x_DWORD);

//----- (00075900) --------------------------------------------------------
void sub_75900()
{
	sub_75AB0();
}

//----- (00053F80) --------------------------------------------------------
char LoadFilesFromCDAndGameData(const char* cdPath, const char* gamePath, const char* fileName)//234f80
{
	FILE* file1; // ebx
	FILE* file2; // esi
	int v5; // eax
	int v6; // edi
	FILE* file3; // ebx
	FILE* file4; // edi
	signed int v10; // ebx
	int v11; // eax
	int v12; // esi
	int v13; // esi
	int v14; // eax

	char printbuffer[512] = { '\0' };//char v15; // [esp+0h] [ebp-194h]
	char printbuffer2[512] = { '\0' };//char v16; // [esp+90h] [ebp-104h]
	// pdwScreenBuffer_351628 - readbuffer
	//printbuffer - char v17; // [esp+120h] [ebp-74h]
	int v18; // [esp+184h] [ebp-10h]
	int v19; // [esp+188h] [ebp-Ch]
	int v20; // [esp+18Ch] [ebp-8h]
	//FILE* v21; // [esp+190h] [ebp-4h]

	if (!sub_53F60(cdPath))//234F90 - 234F60
		return 2;
	if (!sub_53F60(gamePath))//234FA5 - 234F60
		return 1;
	sprintf(printbuffer, "%s/%s.TAB", cdPath, fileName);//234FCA - 269F3D5
	sprintf(printbuffer2, "%s/%s.TAB", gamePath, fileName);//234FE3 - 269F3D5
	file1 = DataFileIO::CreateOrOpenFile(printbuffer, 0x200);//234FF7 - 279817
	if (file1 == NULL)//there will probably be inequality here
		return 3;
	file2 = DataFileIO::CreateOrOpenFile(printbuffer2, 0x222);//235012 - 279817
	if (file2 == NULL)
		return 3;
	v5 = DataFileIO::FileLengthBytes(file1);
	v6 = DataFileIO::Read(file1, readbuffer, v5);
	v19 = DataFileIO::WriteFile_98CAA(file2, readbuffer, v6);
	DataFileIO::Close(file1);
	DataFileIO::Close(file2);
	if (v6 != v19)
		return 3;
	sprintf(printbuffer, "%s/%s.DAT", cdPath, fileName);
	sprintf(printbuffer2, "%s/%s.DAT", gamePath, fileName);
	file3 = DataFileIO::CreateOrOpenFile(printbuffer, 512);
	//v21 = v8;
	if (file3 == NULL)
		return 3;
	file4 = DataFileIO::CreateOrOpenFile(printbuffer2, 546);
	if (file4 == NULL)
		return 3;
	v10 = DataFileIO::FileLengthBytes(file3);
	v18 = v10;
	v20 = 0;
	while (v10)
	{
		if (v10 <= 64000)
			v11 = v10;
		else
			v11 = 64000;
		v12 = DataFileIO::Read(file3, readbuffer, v11);
		v10 -= v12;
		if (DataFileIO::WriteFile_98CAA(file4, readbuffer, v12) != v12)
		{
			DataFileIO::Close(file3);
			DataFileIO::Close(file4);
			return 3;
		}
		v20 += v12;
		v13 = v20;
		settextposition(x_DWORD_F4720.x, x_DWORD_F4720.y);
		v14 = 100 * v13 / v18;
		if (v14 > 100)
			v14 = 100;
		sprintf(printbuffer, "%d%c", v14, 37);
		outtext(printbuffer);
	}
	DataFileIO::Close(file3);
	DataFileIO::Close(file4);
	memset(readbuffer, 0, 64000);
	return 0;
}

//----- (00053F60) --------------------------------------------------------
bool sub_53F60(const char* a1)//234f60
{
	return myaccess(a1, 0) != NULL;//234F69 - 279CF2
}
// 98CF2: using guessed type x_DWORD access(x_DWORD, x_DWORD);

//----- (0008C2CD) --------------------------------------------------------
void sub_8C2CD()//26d2cd
{
	;
}

//----- (0005C430) --------------------------------------------------------
void NetworkAllocation2_5C430()//23d430
{
	x_D41A0_BYTEARRAY_4_struct.isNetwork_216w = NetworkAllocation_74556();//255556 push ebp 355250
}

//----- (00055F70) --------------------------------------------------------
int sub_main(int argc, char** argv, char**  /*envp*/)//236F70
{
	std::function<void(Scene)> sceneChangeCallBack = SetCurrentScene;

	int exitCode = 0;
	if (!CommandLineParams.DoDebugafterload())IsAfterLoad = true;

	SetTimeStart();

#ifndef __ANDROID__
	try
#endif
	{
		begin_plugin();

		preconvert();//rewrite and remove it later

		*xadataclrd0dat.colorPalette_var28 = (uint8_t*)malloc(4096);//fix it

		unsigned __int16 actLevel = 0;

		//skip memset(&v6, 0, 28);//236F7F - 26D250
		//v7 = 0;
		//skip v6 = 0x3301;
	//removed  int386(0x21, (REGS*)&v6, (REGS*)&v6);//236F9D - 279D52 //INT 21,33 - Get/Set System Values (Ctl-Break/Boot Drive) AH = 33h AL = 01 to set Ctrl - Break checking flag
	//may be INT 33,1 Show Mouse Cursor see:https://www.equestionanswers.com/c/c-int33-mouse-service.php
		//skip signal(7, 1);//236FA9 - 279DC0
		//skip signal(4, 1);//236FB5 - 279DC0
		//skip signal(6, 1);//236FC1 - 279DC0

		std::cout << "Initializing logger...\n";

		spdlog::level::level_enum level = GetLoggingLevelFromString(CommandLineParams.GetLogLevelStr().c_str());

#ifdef _DEBUG
		level = GetLoggingLevelFromString("Debug");
#endif

		InitializeLogging(level);

		Logger->info("Reading config.json file");

#ifndef __ANDROID__
		try
#endif
		{
			SetConfig();
		}
#ifndef __ANDROID__
		catch (const std::exception& e)
		{
			Logger->critical("Error reading config.json file: {}", e.what());
			return -1;
		}
#endif

		EventDispatcher::I = new EventDispatcher();

		EventDispatcher::I->RegisterEvent(new Event<Scene>(EventType::E_SCENE_CHANGE, sceneChangeCallBack));
		EventDispatcher::I->DispatchEvent(EventType::E_GAME_STATE_CHANGE, GameState::STARTED);

		if (assignToSpecificCores)
		{
#ifdef _MSC_VER
			SetThreadIdealProcessor(GetCurrentThread(), 0);
			DWORD_PTR dw = SetThreadAffinityMask(GetCurrentThread(), DWORD_PTR(1) << 0);
#endif
		}

		if (CommandLineParams.DoDisableGraphicsEnhance()) {
			Logger->debug("Disabling enhanced graphics");
			bigSprites = false;
			bigTextures = false;
			texturepixels = 32;
		}

		//Set Paths for game data
		Logger->debug("Getting Game data paths");
		gameDataPath = GetSubDirectoryPath(gameFolder.c_str());
		cdDataPath = GetSubDirectoryPath(cdFolder.c_str());
		highResGraphicsPath = GetSubDirectoryPath(highResGraphicsFolder.c_str());
		fixedMenuGraphicsPath = GetSubDirectoryPath(fixedMenuGraphicsFolder.c_str());

		Logger->debug("Initializing graphics Width: {} Height: {}", windowResWidth, windowResHeight);
		VGA_Init(windowResWidth, windowResHeight, gameResWidth, gameResHeight, maintainAspectRatio, displayIndex);
		gamepad_init(gameResWidth, gameResHeight);

		Logger->info("Finding Game Data...");
		if (std::string mainfile = GetSubDirectoryFile(gameFolder.c_str(), "CDATA", "TMAPS0-0.DAT"); !file_exists(mainfile.c_str()))//test original file
		{
			if (std::filesystem::is_directory(gameDataPath))
			{
				Logger->info("Original game not found in {0} sub folder ", gameFolder);
				Logger->info("Installing game data from CD_Files...");
			}
			else
			{
				Logger->error("Sub folder {0} does not exist!", gameFolder);
				mydelay(5000);
				exit(1);//iso not found
			}
		}
		else
		{
			Logger->info("Original Game Data Found!");
		}

		//dos_setvect(9, null_vector, 0);

		initposistruct();

		sub_56210_process_command_line(argc, argv);//236FD4 - 237210
		if (CommandLineParams.ModeNetwork()) {
			if (Iam_server || Iam_client)
				InitNetworkInfo();
		}

		if (CommandLineParams.DoCopySkipConfig() || config_skip_screen) {
			x_BYTE_D41AD_skip_screen = config_skip_screen;
		}

		if (CommandLineParams.GetPlaybackPath().length() > 0 &&
			std::filesystem::exists(CommandLineParams.GetPlaybackPath().c_str()))
		{
			StartPlayback(CommandLineParams.GetPlaybackPath().c_str());
		}
		else if (CommandLineParams.GetRecordingPath().length() > 0)
		{
			StartRecording(CommandLineParams.GetRecordingPath().c_str());
		}

		Initialize();//236FDC - 23C8D0//rozdil 1E1000

		sub_46830_main_loop(actLevel);//227830

		if (CommandLineParams.GetPlaybackPath().length() > 0 &&
			std::filesystem::exists(CommandLineParams.GetPlaybackPath().c_str()))
		{
			StopPlayback();
		}
		else if (CommandLineParams.GetRecordingPath().length() > 0)
		{
			StopRecording();
		}

		sub_5BC20();//23CC20 //remove devices?
		sub_56730_clean_memory();//237730

		if (CommandLineParams.ModeNetwork()) {
			if (Iam_server || Iam_client)
			{
				EndMyNetLib();
				/*EndLibNetClient();
				if (Iam_server)
					EndLibNetServer();*/
			}
		}
		delete EventDispatcher::I;
		VGA_close();
	}
#ifndef __ANDROID__
	catch (const thread_exit_exception& e)
	{
		Logger->info("Immediate Exit called");
	}
	catch (const std::exception& e)
	{
		Logger->critical("Critical Error: {}", e.what());
		exitCode = -1;
	}
#endif
	Logger->info("Exited Game");
	return exitCode;
}

Scene GetCurrentScene()
{
	return m_CurrentScene;
}

void SetCurrentScene(const Scene scene)
{
	m_CurrentScene = scene;
}

void StartRecording(const char* outputFileName)
{
	if (m_InputRecorder != nullptr)
		delete m_InputRecorder;

	m_InputRecorder = new InputRecorder(outputFileName);
	m_InputRecorder->StartRecording();
}

void StopRecording()
{
	if (m_InputRecorder != nullptr && m_InputRecorder->m_IsRecording)
	{
		m_InputRecorder->StopRecording();
		delete m_InputRecorder;
		m_InputRecorder = nullptr;
	}
}

void StartPlayback(const char* inputFileName)
{
	if (m_InputRecorder != nullptr)
	{
		delete m_InputRecorder;
		m_InputRecorder = nullptr;
	}

	m_InputRecorder = new InputRecorder(inputFileName);
	m_InputRecorder->StartPlayback();
}

void StopPlayback()
{
	if (m_InputRecorder != nullptr && m_InputRecorder->m_IsPlaying)
	{
		m_InputRecorder->StopPlayback();
		delete m_InputRecorder;
		m_InputRecorder = nullptr;
	}
}

bool IsRecordingOrPlaying()
{
	return (m_InputRecorder != nullptr && (m_InputRecorder->m_IsPlaying || m_InputRecorder->m_IsRecording));
}

bool IsRecording()
{
	return (m_InputRecorder != nullptr && m_InputRecorder->m_IsRecording);
}