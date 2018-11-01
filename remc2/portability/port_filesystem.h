#ifndef PORT_FILESYSTEM
#define PORT_FILESYSTEM
#include <io.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <windows.h>
#include <direct.h> 
#include "mctypes.h"
#include "dirent.h"
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>

extern char gamepath[512];

long my_findfirst(char* path, _finddata_t* c_file);
long my_findnext(long hFile, _finddata_t* c_file);
void my_findclose(long hFile);
bool file_exists(const char * filename);
FILE* mycreate(char* path, Bit32u flags);
Bit32s myaccess(char* path, Bit32u flags);
Bit32s mymkdir(char* path);

FILE* myopen(char* path, int pmode, Bit32u flags);
int myclose(FILE* descriptor);
Bit32s mylseek(FILE* filedesc, x_DWORD position, char type);
Bit32s myfseek(FILE* filedesc, x_DWORD position, char type);

long myftell(FILE* decriptor);

int x_chdir(const char* path);
char* x_getcwd(x_DWORD a, x_DWORD b);

FILE* myopent(char* path, char* type);

typedef struct {
	int number;
	char dir[256][512];
} dirsstruct;

dirsstruct getListDir(char* dirname);

int dos_getdrive(int* a);

#endif //PORT_FILESYSTEM
