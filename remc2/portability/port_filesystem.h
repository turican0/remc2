#ifndef PORT_FILESYSTEM
#define PORT_FILESYSTEM
#include <io.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <windows.h>
//#ifdef _MSC_VER
#include <direct.h>
//#endif
#include "mctypes.h"
#ifdef _MSC_VER
#include "dirent-x.h"
#else
#include "dirent.h"
#endif
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>

#define DEBUG_MKDIR
#define DEBUG_START
#define DEBUG_FILEOPS
#define DEBUG_LOADSAVE

//#define DEBUG_PRINT_DEBUG_TO_SCREEN

extern char gameFolder[512];
extern char cdFolder[512];
extern char bigGraphicsFolder[512];


long my_findfirst(char* path, _finddata_t* c_file);
long my_findnext(long hFile, _finddata_t* c_file);
void my_findclose(long hFile);
bool file_exists(const char * filename);
bool fix_file_exists(const char* filename);
FILE* mycreate(char* path, Bit32u flags);
Bit32s myaccess(char* path, Bit32u flags);
Bit32s mymkdir(char* path);

FILE* myopen(char* path, int pmode, Bit32u flags);
int myclose(FILE* descriptor);
Bit32s mylseek(FILE* filedesc, x_DWORD position, char type);
Bit32s myfseek(FILE* filedesc, x_DWORD position, char type);

void AdvReadfile(const char* path, Bit8u* buffer);
void ReadGraphicsfile(const char* path, Bit8u* buffer,long size=-1);
bool ExistGraphicsfile(const char* path);

long myftell(FILE* decriptor);

int x_chdir(const char* path);
char* x_getcwd(x_DWORD a, x_DWORD b);

FILE* myopent(char* path, char* type);

typedef struct {
	int number;
	char dir[256][512];
} dirsstruct;

dirsstruct getListDir(char* dirname);
//dirsstruct getListDirFix(char* dirname);
void FixDir(char* outdirname, char* indirname);

int dos_getdrive(int* a);

void get_exe_path(char*);

unsigned __int64 dos_getdiskfree(__int16 a1, __int16 a2, Bit8u a, short* b);

void debug_printf(const char* format, ...);

void GetSubDirectoryPath(char* buffer, char* subDirectory);

void GetSubDirectoryPath(char* buffer, char* gamepath, char* subDirectory);

void GetSubDirectoryFile(char* buffer, char* gamepath, char* subDirectory, char* fileName);

void GetSaveGameFile(char* buffer, char* gamepath, __int16 index);

#endif //PORT_FILESYSTEM
