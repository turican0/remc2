#include "DataFileIO.h"

#include "engine/Basic.h"
#include "engine/engine_support.h"

Pathstruct pstr[100] = {
xasearchd_2bac30 ,//00
xafonts0dat,//01
xafonts0tab,//02
xafonts1dat,//03
xafonts1tab,//04
xadatatmaps00tab,//05
xapaldata,//06
xapalmem,//07
xadatapointersdat,//08
xadatapointerstab,//09
xazero,//10-a
xadatabuild00dat,//11-b
xadatabuild00tab,//12-c
xazero,//13
xadataetextdat,//14
xazero,//15
xadataftextdat,//16
xazero,//17
xadatagtextdat,//18
xazero,//19
xadataitextdat,//20
xazero,//21
xadatablock16dat,//22
xadatablock32dat,//23
xazero,//24
xabscreen,//25
xadatapald0dat,//26
xazero,//27
xawscreen_351628,//28
xabscreen2,//29
xadatapald0dat2,//30
xazero,//31
xadatamsprd00dat,//32
xadatamsprd00tab,//33
xazero,//34
xadatahsprd00dat,//35
xadatahsprd00tab,//36
xadatahfont3dat,//37
xadatahfont3tab,//38
xazero,//39
xadataclrd0dat,//40
xazero,//41
xadataspellsdatx,//42
xazero };//43

void DataFileIO::SetCDFilePaths(const char* cdDataPath, Pathstruct pstr[])
{
	sprintf(pstr[psxafonts0dat].path, "%s/%s", cdDataPath, "DATA/FONT0.DAT\0");
	sprintf(pstr[psxafonts0tab].path, "%s/%s", cdDataPath, "DATA/FONT0.TAB\0");
	sprintf(pstr[psxafonts1dat].path, "%s/%s", cdDataPath, "DATA/FONT1.DAT\0");
	sprintf(pstr[psxafonts1tab].path, "%s/%s", cdDataPath, "DATA/FONT1.TAB\0");
	sprintf(pstr[psxadatatmaps00tab].path, "%s/%s", cdDataPath, "DATA/TMAPS0-0.TAB");
	sprintf(pstr[psxadatapointersdat].path, "%s/%s", cdDataPath, "DATA/POINTERS.DAT\0");
	sprintf(pstr[psxadatapointerstab].path, "%s/%s", cdDataPath, "DATA/POINTERS.TAB\0");
	sprintf(pstr[psxadatabuild00dat].path, "%s/%s", cdDataPath, "DATA/BUILD0-0.DAT\0");
	sprintf(pstr[psxadatabuild00tab].path, "%s/%s", cdDataPath, "DATA/BUILD0-0.TAB\0");
	sprintf(pstr[psxadataetextdat].path, "%s/%s", cdDataPath, "DATA/ETEXT.DAT\0");
	sprintf(pstr[psxadataftextdat].path, "%s/%s", cdDataPath, "DATA/FTEXT.DAT\0");
	sprintf(pstr[psxadatagtextdat].path, "%s/%s", cdDataPath, "DATA/GTEXT.DAT\0");
	sprintf(pstr[psxadataitextdat].path, "%s/%s", cdDataPath, "DATA/ITEXT.DAT\0");
	sprintf(pstr[psxadatablock16dat].path, "%s/%s", cdDataPath, "DATA/BLOCK16.DAT\0");
	sprintf(pstr[psxadatablock32dat].path, "%s/%s", cdDataPath, "DATA/BLOCK32.DAT\0");
	sprintf(pstr[psxadatapald0dat].path, "%s/%s", cdDataPath, "DATA/PALD-0.DAT\0");
	sprintf(pstr[psxadatapald0dat2].path, "%s/%s", cdDataPath, "DATA/PALD-0.DAT\0");
	sprintf(pstr[psxadatamsprd00dat].path, "%s/%s", cdDataPath, "DATA/MSPRD0-0.DAT\0");
	sprintf(pstr[psxadatamsprd00tab].path, "%s/%s", cdDataPath, "DATA/MSPRD0-0.TAB\0");
	sprintf(pstr[psxadatahsprd00dat].path, "%s/%s", cdDataPath, "DATA/HSPRD0-0.DAT\0");
	sprintf(pstr[psxadatahsprd00tab].path, "%s/%s", cdDataPath, "DATA/HSPRD0-0.TAB\0");
	sprintf(pstr[psxadatahfont3dat].path, "%s/%s", cdDataPath, "DATA/HFONT3.DAT\0");
	sprintf(pstr[psxadatahfont3tab].path, "%s/%s", cdDataPath, "DATA/HFONT3.TAB\0");
	sprintf(pstr[psxadataclrd0dat].path, "%s/%s", cdDataPath, "DATA/CLRD-0.DAT\0");
	sprintf(pstr[psxadataspellsdat].path, "%s/%s", cdDataPath, "DATA/SPELLS.DAT\0");
}

int DataFileIO::ReadFileAndDecompress(const char* path, uint8_t** data)
{
	int result = 0; // eax // length of file or 0 on error
	FILE* file; // ebx
	uint32_t length; // esi
	file = CreateOrOpenFile((char*)path, 0x200);

	if (file)
	{
		length = FileLengthBytes(file);
		Read(file, *data, length);
		Close(file);
		result = (int)DataFileRNC::Decompress(*data, *data);
		if (result >= 0)
		{
			if (!result)
				result = length;
		}
		else
		{
			Logger->error("ERROR decompressing");
			result = -2;
		}
	}
	return result;
}

FILE* DataFileIO::CreateOrOpenFile(const char* pathname, int __pmode)
{
	FILE* file; // ST10_4


	if (__pmode == 0x222)
	{
#ifdef _MSC_VER
		file = CreateFile(pathname, GENERIC_ALL);
#else
		file = CreateFile(pathname, 0x1c0);
#endif
		//x_setmode(v2, 0x200);
		if (file) {
			Close(file);
		}
	}
	return Open(pathname, __pmode, 0x40);
}

FILE* DataFileIO::CreateFile(const char* path, uint32_t flags)
{
	return mycreate(path, flags);
}

FILE* DataFileIO::Open(const char* path, int pmode, uint32_t flags) {
	return myopen(path, pmode, flags);
}

int32_t DataFileIO::Close(FILE* file) {
	return myclose(file);
}

int32_t DataFileIO::Seek(FILE* file, x_DWORD position, char type) {
	return mylseek(file, position, type);
}

size_t DataFileIO::Read(FILE* file, uint8_t* data, uint32_t length) {
	size_t result = fread(data, 1, length, file);
	Logger->trace("Read fread length {} result {}", length, result);
	return result;
};

long DataFileIO::FileLengthBytes(FILE* file) 
{
	long size;
	myfseek(file, 0, SEEK_END); // seek to end of file
	size = myftell(file); // get current file pointer
	myfseek(file, 0, SEEK_SET); // seek back to beginning of file
	return size;
}

//----- (00055C00) --------------------------------------------------------
bool DataFileIO::sub_55C00_TestSaveFile2(int16_t a1)//236c00
{
	return sub_55750_TestExistingSaveFile(0, a1);
}

//----- (00055750) --------------------------------------------------------
bool DataFileIO::sub_55750_TestExistingSaveFile(uint8_t fileindex, int levelindex, bool loadRegressionTest)//236750 //load in game
{
	FILE* saveslevfile;
	int32_t readState;
	bool correctReadState;
	uint32_t filesize2;
	FILE* savesmapfile;
	uint32_t filesize;
	bool result = false;
	char path[512];
	sprintf(path, "%s/%s", gameDataPath.c_str(), "SAVE");
	if (loadRegressionTest)
	{
		sprintf(path, "%sregressions", CommandLineParams.GetMemimagesPath().c_str());
		if (unitTests)
		{
			sprintf(path, "%s/SAVE", unitTestsPath.c_str());
		}
	}

	sprintf(printbuffer, "%s/%s%d.DAT", path, "SVER", fileindex + 1);
	if (DataFileIO::ReadFileAndDecompress(printbuffer, &readbuffer) == 8 && *(uint32_t*)&readbuffer[4] == levelindex && *(uint32_t*)&readbuffer[0] == 0xf)
	{
		sprintf(printbuffer, "%s/%s%d.DAT", path, "SLEV", fileindex + 1);
		saveslevfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
		if (saveslevfile)
		{
			readState = DataFileIO::Read(saveslevfile, readbuffer, 8);
			correctReadState = readState != 8;
			if (readState == 8 && *(uint32_t*)&readbuffer[4] != D41A0_0.dword_0x4)
				correctReadState = true;
			filesize2 = DataFileIO::FileLengthBytes(saveslevfile);
			DataFileIO::Close(saveslevfile);
			if (filesize2 == sizeof(type_shadow_D41A0_BYTESTR_0) && !correctReadState)
			{
				sprintf(printbuffer, "%s/%s%d.DAT", path, "SMAP", fileindex + 1);
				savesmapfile = DataFileIO::CreateOrOpenFile(printbuffer, 512);
				if (savesmapfile)
				{
					filesize = DataFileIO::FileLengthBytes(savesmapfile);
					DataFileIO::Close(savesmapfile);
					if (filesize == 0x712C2)
						result = true;
				}
			}
		}
	}
	return result;
}

//----- (00090D3F) --------------------------------------------------------
char* DataFileIO::sub_90D3F_unload_file_array(int psindex)//271d3f
{
	char* result; // eax

	while (1)
	{
		result = pstr[psindex].path;
		if (!pstr[psindex].colorPalette_var28)
			break;
		ClearMemoryForPath(pstr[psindex]);
		//pstr[++psindex];
		psindex++;
	}
	return result;
}

//----- (00084250) --------------------------------------------------------
bool DataFileIO::LoadFileArray_84250(int psindex)//265250
{
	int oldpsindex = psindex;
	if (pstr[psindex].colorPalette_var28)
	{
		do
		{
			ClearMemoryForPath(pstr[psindex++]);
		} while (pstr[psindex].colorPalette_var28);
	}
	psindex = oldpsindex;
	int index = 0;
	while (pstr[psindex].colorPalette_var28)
	{
		int unpackResult = UnpackAndLoadMemoryFromPath(pstr[psindex]);//27B32d
		if (unpackResult >= 0)
		{
			if (unpackResult)
			{
				psindex++;
				continue;
			}
			sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);//install Palette for text mode(show error)
			myprintf("ERROR: File %s.\n","palette");
		}
		else
		{
			sub_41A90_VGA_Palette_install((TColor*)*xadatapald0dat2.colorPalette_var28);//install Palette for text mode(show error)
			myprintf("ERROR: Allocation %s.\n","palette");
		}
		myprintf("Press return to continue\n");
		index++;
		getc(stdin);
		psindex++;
	}
	return index;
}

//----- (0009A2F5) --------------------------------------------------------
uint8_t* DataFileIO::ClearMemoryForPath(Pathstruct path)//27B2f5
{
	uint8_t* result; // eax
	//2bac30
	result = *path.colorPalette_var28;//fix it 2bac30
	//result = 0;//fix it
	if (result)
	{
		FreeMem_83E80(*path.colorPalette_var28);
		result = *path.colorPalette_var28;
		*result = 0;
	}
	return result;
}

//----- (0009A32D) --------------------------------------------------------
signed int DataFileIO::UnpackAndLoadMemoryFromPath(Pathstruct path)//27B32d
{
	//int v1; // edx
	//int *v2; // eax
	//void* v3; // edx
	//int *v4; // eax
	//void* (*v6)(int); // [esp+0h] [ebp-8h]

	//sub_85070();
	/*if (path.var40_alloc_type & 1 )
	  v6 = sub_83D70_malloc1;
	else
	  v6 = sub_83CD0_malloc2;*/
	ClearMemoryForPath(path);
	if (path.path[0] == 0x2A)//fix
	{
		if (path.var40_alloc_type & 1)
		{
			*(path.colorPalette_var28) = (uint8_t*)Malloc_83D70(path.var36_size_buffer);
			memset(*(path.colorPalette_var28), 0, path.var36_size_buffer);
		}
		else
		{
			*(path.colorPalette_var28) = (uint8_t*)Malloc_83CD0(path.var36_size_buffer);
			memset(*(path.colorPalette_var28), 0, path.var36_size_buffer);
		}
		//v2 = *(int *)(path.colorPalette_var28);
		//*v2 = v1;
		if (!(*(path.colorPalette_var28)))
			return 0;
	}
	else
	{
		path.var36_size_buffer = (signed int)sub_AB9E1_get_file_unpack_size(path.path);
		if (path.var36_size_buffer <= 0)
			return 0;
		if (path.var40_alloc_type & 1)
		{
			*(path.colorPalette_var28) = (uint8_t*)Malloc_83D70(path.var36_size_buffer);//asi init a malloc bufferu
			memset(*(path.colorPalette_var28), 0, path.var36_size_buffer);
		}
		else
		{
			*(path.colorPalette_var28) = (uint8_t*)Malloc_83CD0(path.var36_size_buffer);//asi init a malloc bufferu
			memset(*(path.colorPalette_var28), 0, path.var36_size_buffer);
		}
		//v4 = *(int **)path.colorPalette_var28;
		//*v4 = v3;
		if (!(*(path.colorPalette_var28)))
			return -1;

		if (DataFileIO::ReadFileAndDecompress(path.path, path.colorPalette_var28) != path.var36_size_buffer)
		{
			path.colorPalette_var28 = 0;
			path.var32_end_buffer = 0;
			path.var36_size_buffer = 0;
			return 0;
		}
	}
	if (path.var32_end_buffer)
		(*path.var32_end_buffer) = (*path.colorPalette_var28) + path.var36_size_buffer;//fix ma byt 1
	return 1;
}

//----- (000AB9E1) --------------------------------------------------------
signed int DataFileIO::sub_AB9E1_get_file_unpack_size(const char* path)//28c9e1
{
	uint8_t v2[10]; // [esp+0h] [ebp-1Ch]
	//unsigned __int8 v3; // [esp+4h] [ebp-18h]
	//unsigned __int8 v4; // [esp+5h] [ebp-17h]
	//unsigned __int8 v5; // [esp+6h] [ebp-16h]
	//unsigned __int8 v6; // [esp+7h] [ebp-15h]
	//char v7; // [esp+8h] [ebp-14h]
	//char v8; // [esp+9h] [ebp-13h]
	//char v9; // [esp+Ah] [ebp-12h]
	//char v10; // [esp+Bh] [ebp-11h]
	//char v11; // [esp+Ch] [ebp-10h]
	int ret_size; // [esp+14h] [ebp-8h]
	FILE* file; // [esp+18h] [ebp-4h]

	//fix it
	//v3 = 0;
	//v4 = 0;
	//v5 = 0;
	//v6 = 0;
	//v9 = 0;
	//v10 = 0;
	//fix it

	ret_size = -1;
	//v7 = 'R';
	//v8 = 'N';
	//v9 = 'C';
	//v10 = 1;
	//v11 = 0;
	file = DataFileIO::CreateOrOpenFile(path, 512);
	if (file == NULL)
		return 0;
	DataFileIO::Read(file, v2, 8);
	if (!strncmp((const char*)v2, (const char*)"RNC\1", 4))
	{
		ret_size = v2[4] << 8;//reverse size in rnc format
		ret_size += v2[5];
		ret_size <<= 8;
		ret_size += v2[6];
		ret_size <<= 8;
		ret_size += v2[7];
	}
	else
	{
		ret_size = DataFileIO::FileLengthBytes(file);
	}
	DataFileIO::Close(file);
	return ret_size;
}
// 988DA: using guessed type x_DWORD filelength(x_DWORD);
// 99682: using guessed type x_DWORD strncmp(x_DWORD, x_DWORD, x_DWORD);

//----- (00098C48) --------------------------------------------------------
signed int DataFileIO::sub_98C48_open_nwrite_close(const char* filename, uint8_t* buffer, uint32_t count)//279c48
{
	int result; // ST14_4
	FILE* file; // [esp+4h] [ebp-8h]

	file = DataFileIO::CreateOrOpenFile(filename, 546);
	if (file == NULL)
		return -1;//mozna null
	result = WriteFile_98CAA(file, buffer, count);
	DataFileIO::Close(file);
	return result;
}

size_t DataFileIO::x_write2(FILE* descriptor, uint8_t* buffer, uint32_t size) {
	return fwrite(buffer, 1, size, descriptor);
};// weak

//----- (00098CAA) --------------------------------------------------------
size_t DataFileIO::WriteFile_98CAA(FILE* filename, uint8_t* buffer, uint32_t num_bytes)//279caa
{
	return x_write2(filename, buffer, num_bytes);
}
