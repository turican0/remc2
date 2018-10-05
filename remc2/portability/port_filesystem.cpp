#include "port_filesystem.h"

void pathfix(char* path, char* path2)
{
	if ((path[0] == 'c') || (path[0] == 'C'))
	{
		long len = strlen(path);
		char* fixstring = (char*)"c:/prenos/Magic2/mc2-orig-copy";
		long fixlen = strlen(fixstring);
		for (int i = len;i > 1;i--)
			path2[i + fixlen - 2] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}
	else
	{
		long len = strlen(path);
		char* fixstring = (char*)"c:/prenos/Magic2/mc2-orig-copy/";
		long fixlen = strlen(fixstring);
		for (int i = len;i > -1;i--)
			path2[i + fixlen] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}
}

void unpathfix(char* path, char* path2)
{
	/*if ((path[0] == 'c') || (path[0] == 'C'))
	{
		long len = strlen(path);
		char* fixstring = (char*)"c:/prenos/Magic2/mc2-orig-copy";
		long fixlen = strlen(fixstring);
		for (int i = len;i > 1;i--)
			path2[i + fixlen - 2] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}
	else
	{
		long len = strlen(path);
		char* fixstring = (char*)"c:/prenos/Magic2/mc2-orig-copy/";
		long fixlen = strlen(fixstring);
		for (int i = len;i > -1;i--)
			path2[i + fixlen] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}*/
	strcpy(path2, "c:/prenos/Magic2/mc2-orig-copy/");//fix this
}

long my_findfirst(char* path, _finddata_t* c_file){
	char path2[2048] = "\0";
	pathfix(path, path2);//only for DOSBOX version
	return _findfirst(path2, c_file);
}

long my_findnext(long hFile, _finddata_t* c_file){
return _findnext(hFile, c_file);
}

void my_findclose(long hFile){
	_findclose(hFile);
};

bool file_exists(const char * filename) {
	/*if (FILE * file = fopen(filename, "r")) {
		fclose(file);
		return true;
	}
	return false;*/
	FILE* file;
	if (fopen_s(&file, filename, "r")) {
		fclose(file);
		return true;
	}
	return false;
}

FILE* mycreate(char* path, Bit32u flags) {
	FILE *fp;
	char path2[512] = "\0";
	pathfix(path, path2);//only for DOSBOX version
	fp = fopen(path2, "wb+");
	return fp;
};

Bit32s myaccess(char* path, Bit32u flags) {
	DIR *dir;
	char path2[2048] = "\0";
	pathfix(path, path2);//only for DOSBOX version
	dir = opendir(path2);
	if (dir)
	{
		/* Directory exists. */
		closedir(dir);
		return 1;
	}
	else if (ENOENT == errno)
	{
		return -1;
	}
	return -1;
};

Bit32s /*__cdecl*/ mymkdir(char* path) {
	char path2[512] = "\0";
	pathfix(path, path2);//only for DOSBOX version


	const WCHAR *pwcsName;
	// required size
	int nChars = MultiByteToWideChar(CP_ACP, 0, path2, -1, NULL, 0);
	// allocate it
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, path2, -1, (LPWSTR)pwcsName, nChars);
	// use it....





	int result;
#if defined (WIN32)						/* MS Visual C++ */
	result = _wmkdir(pwcsName);
#else
	result = mkdir(pwcsName, 0700);
#endif
	// delete it
	delete[] pwcsName;
	return result;
};

FILE* myopen(char* path, int pmode, Bit32u flags) {
	//bool localDrive::FileOpen(DOS_File * * file, const char * name, Bit32u flags) {
	const char * type;
	if ((pmode == 0x222) && (flags == 0x40))type = "rb+";
	else if ((pmode == 0x200) && (flags == 0x40))type = "rb+";
	else
		exit(1);//error - DOSSetError(DOSERR_ACCESS_CODE_INVALID);
	FILE *fp;
	char path2[512] = "\0";
	pathfix(path, path2);//only for DOSBOX version
	//if(file_exists(path2))

	fopen_s(&fp, path2, type);
	return fp;
};
int myclose(FILE* descriptor) {
	return fclose(descriptor);
};
Bit32s mylseek(FILE* filedesc, x_DWORD position, char type) {
	return fseek(filedesc, position, type);
};

int x_chdir(const char* path) {
	char path2[2048] = "\0";
	pathfix((char*)path, path2);
	int result = _chdir(path2);
	return result;
};// weak
char* x_getcwd(x_DWORD a, x_DWORD b) {
	char cwd[512] = "\0";	
	if (getcwd(cwd, 512) == NULL)
		perror("getcwd() error");
	char* path2 = (char*)malloc(512);
	unpathfix(cwd, path2);	
	return path2;
};