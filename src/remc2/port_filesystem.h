#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>  
#include <cstring>
#include <string>
#include <filesystem>
#include <regex>
#include <vector>
#include <iostream>
#include "mctypes.h"
#include "fcaseopen.h"
#include "strutils.h"

#ifdef _MSC_VER
	#include <windows.h>
	#include <direct.h>
	#include <io.h>
	#include "dirent-x.h"
#else
	#include "dirent.h"
	extern "C" {
    	#include "findfirst.h"
	}
	#include <limits.h>

	#define MAX_PATH PATH_MAX
	#define _chdir chdir
#endif

//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>

#define DEBUG_MKDIR
#define DEBUG_START
#define DEBUG_FILEOPS
#define DEBUG_LOADSAVE

//#define DEBUG_PRINT_DEBUG_TO_SCREEN

extern std::filesystem::path gameFolder;
extern std::filesystem::path cdFolder;
extern std::filesystem::path bigGraphicsFolder;

long my_findfirst(const std::filesystem::path& path, _finddata_t* c_file);
long my_findnext(long hFile, _finddata_t* c_file);
void my_findclose(long hFile);
#define file_exists std::filesystem::exists
FILE* mycreate(const std::filesystem::path& path, uint32_t flags);
int32_t myaccess(const char* path, uint32_t flags);
int32_t mymkdir(const char* path);

FILE* myopen(const std::filesystem::path& path, int pmode, uint32_t flags);
int myclose(FILE* descriptor);
int32_t mylseek(FILE* filedesc, x_DWORD position, char type);
int32_t myfseek(FILE* filedesc, x_DWORD position, char type);

void AdvReadfile(const char* path, uint8_t* buffer);
void ReadGraphicsfile(const std::filesystem::path& path, uint8_t* buffer,long size=-1);
bool ExistGraphicsfile(const char* path);

long myftell(FILE* decriptor);

bool DirExists(const std::filesystem::path& path);

FILE* myopent(char* path, char* type);

typedef struct {
	int number;
	char dir[256][512];
} dirsstruct;

dirsstruct getListDir(const std::filesystem::path& dirname);
//dirsstruct getListDirFix(char* dirname);
std::filesystem::path FixDir(const std::filesystem::path& indirname);

int dos_getdrive(int* a);

std::filesystem::path get_exe_path();

#ifdef _MSC_VER
uint64_t dos_getdiskfree(int16_t a1, int16_t a2, uint8_t a, short* b);
#endif

void debug_printf(const char* format, ...);

std::filesystem::path GetSubDirectoryPath(const std::filesystem::path& subDirectory);

std::filesystem::path GetSubDirectoryPath(const std::filesystem::path& gamepath, const std::filesystem::path& subDirectory);

std::filesystem::path GetSubDirectoryFilePath(const std::filesystem::path& subDirectory, const std::filesystem::path& fileName);

std::filesystem::path GetSubDirectoryFile(const std::filesystem::path& gamepath, const std::filesystem::path& subDirectory, const std::filesystem::path& fileName);

std::filesystem::path GetSaveGameFile(const std::filesystem::path& gamepath, int16_t index);

std::filesystem::path GetDirectory(const std::filesystem::path& filePath);

