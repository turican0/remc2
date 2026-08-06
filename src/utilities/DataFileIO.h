#pragma once

#include <cstdio>
#include <cstdint>
#include "portability/port_filesystem.h"
#include "portability/port_outputs.h"
#include "engine/global_types.h"
#include "DataFileRNC.h"

extern Pathstruct pstr[];

class DataFileIO
{
private:	

public:
	static int ReadFileAndDecompress(const char* path, uint8_t** data); //Reads the file path passed in into "data" array of 1 byte elements
	static FILE* CreateOrOpenFile(const char* pathname, int __pmode);
	static FILE* CreateFile(const char* path, uint32_t flags);
	static FILE* Open(const char* path, int pmode, uint32_t flags);
	static int32_t Close(FILE* file);
	static int32_t Seek(FILE* file, x_DWORD position, char type);
	static size_t Read(FILE* file, uint8_t* data, uint32_t length);
	static long FileLengthBytes(FILE* file);
	static bool sub_55C00_TestSaveFile2(int16_t a1);
	static bool sub_55750_TestExistingSaveFile(uint8_t fileindex, int levelindex, bool loadRegressionTest = false);
	static char* sub_90D3F_unload_file_array(int a1);
	static bool LoadFileArray_84250(int psindex);
	static void SetCDFilePaths(const char* cdDataPath, Pathstruct pstr[]);
	static uint8_t* ClearMemoryForPath(Pathstruct path);
	static signed int UnpackAndLoadMemoryFromPath(Pathstruct path);
	static signed int sub_AB9E1_get_file_unpack_size(const char* a1);
	static signed int sub_98C48_open_nwrite_close(const char* file, uint8_t* buffer, uint32_t count);
	static size_t WriteFile_98CAA(FILE* a1, uint8_t* a2, uint32_t a3);
	static size_t x_write2(FILE* descriptor, uint8_t* buffer, uint32_t size);


private:
	// Disallow creating an instance of this object
	DataFileIO();
};
