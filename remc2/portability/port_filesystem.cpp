#include "port_filesystem.h"
#include <string>


//char gamepath[512] = "c:\\prenos\\Magic2\\mc2-orig-copy";
char gamepath[512] = "..\\..\\Magic2\\mc2-orig-copy";
char biggraphicspath[512] = "biggraphics/";
char gamepathout[512];
char fixsound[512] = "fix-sound\\";
char fixsoundout[512];


std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

void pathfix(char* path, char* path2)
{
	LPWSTR buffer = new WCHAR[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string locstr = utf8_encode(buffer);
	std::string::size_type pos = std::string(locstr).find_last_of("\\/");
	std::string strpathx = std::string(locstr).substr(0, pos)/*+"\\system.exe"*/;
	char* pathx = (char*)strpathx.c_str();

	sprintf(fixsoundout, "%s\\%s", pathx, fixsound);
	sprintf(gamepathout, "%s\\%s", pathx, gamepath);
	

	if ((path[0] == 'c') || (path[0] == 'C'))
	{
		long len = strlen(path);
		char* fixstring = (char*)gamepathout;//(char*)"c:/prenos/Magic2/mc2-orig-copy";
		long fixlen = strlen(fixstring);
		for (int i = len;i > 1;i--)
			path2[i + fixlen - 2] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}
	else
	{
		long len = strlen(path);
		char* fixstring = (char*)gamepathout;//(char*)"c:/prenos/Magic2/mc2-orig-copy/";
		long fixlen = strlen(fixstring);
		for (int i = len;i > -1;i--)
			path2[i + fixlen+1] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
		path2[fixlen] = '\\';
	}
}

void get_exe_path(char* retpath) {
	LPWSTR buffer = new WCHAR[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string locstr = utf8_encode(buffer);
	std::string::size_type pos = std::string(locstr).find_last_of("\\/");
	std::string strpathx = std::string(locstr).substr(0, pos)/*+"\\system.exe"*/;
	sprintf(retpath,"%s", (char*)strpathx.c_str());
	//retpath = (char*)strpathx.c_str();
	//return pathx;
};


void pathfix2(char* path, char* path2)
{
	LPWSTR buffer= new WCHAR[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string locstr= utf8_encode(buffer);
	std::string::size_type pos = std::string(locstr).find_last_of("\\/");
	std::string strpathx = std::string(locstr).substr(0, pos)/*+"\\system.exe"*/;
	char* pathx = (char*)strpathx.c_str();
	sprintf(fixsoundout,"%s\\%s", pathx,fixsound);
	sprintf(gamepathout, "%s\\%s", pathx, gamepath);

	if ((path[0] == 'c') || (path[0] == 'C'))
	{
		long len = strlen(path);
		char* fixstring = (char*)gamepath;//(char*)"c:/prenos/Magic2/mc2-orig-copy";
		long fixlen = strlen(fixstring);
		for (int i = len;i > 1;i--)
			path2[i + fixlen - 2] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}
	else
	{
		long len = strlen(path);
		char* fixstring = (char*)fixsoundout;//(char*)"c:/prenos/Magic2/mc2-orig-copy/NETHERW/sound/";
		//char* fixstring = (char*)"c:/prenos/Magic2/mc2-orig-copy/NETHERW/sound/";
		long fixlen = strlen(fixstring);
		for (int i = len;i > -1;i--)
			path2[i + fixlen] = path[i];
		for (int i = 0;i < fixlen;i++)
			path2[i] = fixstring[i];
	}
	free(buffer);
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
	strcpy(path2, "c:/");//fix this
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
	if (fopen_s(&file, filename, "r")==NULL) {
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

Bit32s myfseek(FILE* filedesc, x_DWORD position, char type) {
	return fseek(filedesc, position, type);
};

long myftell(FILE* decriptor) {
	return ftell(decriptor);
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

FILE* myopent(char* path, char* type) {
	FILE *fp;
	char path2[512] = "\0";
	pathfix2(path, path2);//only for DOSBOX version
	//if(file_exists(path2))

	fopen_s(&fp, path2, type);
	return fp;
};

dirsstruct getListDir(char* dirname)
{
	struct dirent *de;  // Pointer for directory entry 
	dirsstruct directories;
	directories.number = 0;
	// opendir() returns a pointer of DIR type.  
	DIR *dr = opendir(dirname);
	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{
		printf("Could not open current directory1 %s\n", dirname);
		return directories;
	}
	// Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
	// for readdir() 
	while ((de = readdir(dr)) != NULL)
	{
		if(de->d_name[0]!='.')
			sprintf(directories.dir[directories.number++],"%s", de->d_name);
		//printf("%s\n", de->d_name);		
	}
	closedir(dr);
	return directories;
}

void FixDir(char* outdirname, char* indirname) {
	//char outdirname[512] = "\0";
	char pathexe[512] = "\0";
	get_exe_path(pathexe);
	sprintf(outdirname, "%s/%s", pathexe, indirname);
};
/*

dirsstruct getListDirFix(char* indirname)
{
	struct dirent *de;  // Pointer for directory entry 
	dirsstruct directories;
	directories.number = 0;
	// opendir() returns a pointer of DIR type.  
	char path2[512] = "\0";
	char pathexe[512] = "\0";
	get_exe_path(pathexe);
	sprintf(path2,"%s/%s", pathexe,indirname);

	DIR *dr = opendir(path2);
	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{
		printf("Could not open current directory2 %s\n", path2);
		return directories;
	}
	// Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
	// for readdir() 
	while ((de = readdir(dr)) != NULL)
	{
		if (de->d_name[0] != '.')
			sprintf(directories.dir[directories.number++], "%s", de->d_name);
		//printf("%s\n", de->d_name);		
	}
	closedir(dr);
	return directories;
}
*/
int dos_getdrive(int* a) {
	*a = _getdrive();
	return *a;
};

struct space_info
{
	// all values are byte counts
	unsigned long capacity;
	unsigned long free;      // <= capacity
	unsigned long available; // <= free
};
//BOOST_FILESYSTEM_DECL
space_info space(char* path, int* ec)
{
#   ifdef BOOST_POSIX_API
	struct BOOST_STATVFS vfs;
	space_info info;
	if (!error(::BOOST_STATVFS(path, &vfs) ? BOOST_ERRNO : 0,
		p, ec, "boost::filesystem::space"))
	{
		info.capacity
			= static_cast<boost::uintmax_t>(vfs.f_blocks)* BOOST_STATVFS_F_FRSIZE;
		info.free
			= static_cast<boost::uintmax_t>(vfs.f_bfree)* BOOST_STATVFS_F_FRSIZE;
		info.available
			= static_cast<boost::uintmax_t>(vfs.f_bavail)* BOOST_STATVFS_F_FRSIZE;
	}

#   else
	ULARGE_INTEGER avail, total, free;
	space_info info;

	//std::string charstring = "hello, world";

	std::wstring widestring;

	for (Bit32u i = 0; i < strlen(path); i++)
		widestring += (wchar_t)path[i];

	LPCWSTR lpcwpath = widestring.c_str();


	if (GetDiskFreeSpaceExW(lpcwpath, &avail, &total, &free) != 0)
	{
		info.capacity
			= ((total.HighPart) << 32)
			+ total.LowPart;
		info.free
			= ((free.HighPart) << 32)
			+ free.LowPart;
		info.available
			= ((avail.HighPart) << 32)
			+ avail.LowPart;
	}

#   endif

	else
	{
		info.capacity = info.free = info.available = 0;
	}
	return info;
}

unsigned __int64 dos_getdiskfree(__int16 a1, __int16 a2, Bit8u a, short* b) {
	unsigned long wanted_size = 0;//fix it
	char drivename[10];
	sprintf(drivename, "%c:", (Bit8u)(a + 64));
	int ec;
	space_info myspaceinfo = space(drivename, &ec);
	if (ec)
		if (myspaceinfo.free > wanted_size)return 0;
		else return 1;
	else return 1;
	/*
	if ( (_WORD)b == -1 )
	return _set_EINVAL(b, a3);
	a4[0] = a;
	a4[1] = a2;
	a4[2] = b;
	a4[3] = a1;
	*/
};

void AdvReadfile(const char* path, Bit8u* buffer) {

	
	char pathexe[512] = "\0";
	get_exe_path(pathexe);
	char path2[512];
	sprintf(path2, "%s/%s", pathexe, path);
	/*
	FILE* file0;
	fopen_s(&file0, path2, (char*)"wb");
	char x = 1;
	fwrite(&x, 1, 1, file0);
	myclose(file0);
	*/
	FILE* file;
	//fopen_s(&file, (char*)"c:\\prenos\\remc2\\biggraphics\\out_rlt-n-out.data", (char*)"rb");
	fopen_s(&file, path2, (char*)"rb");
	fseek(file, 0L, SEEK_END);
	long szdata = ftell(file);
	fseek(file, 0L, SEEK_SET);
	fread(buffer, szdata, 1, file);
	myclose(file);
};

void ReadGraphicsfile(const char* path, Bit8u* buffer) {


	char pathexe[512] = "\0";
	get_exe_path(pathexe);
	char path2[512];
	sprintf(path2, "%s/%s%s", pathexe, biggraphicspath, path);
	
	/*FILE* file0;
	fopen_s(&file0, path2, (char*)"wb");
	char x = 1;
	fwrite(&x, 1, 1, file0);
	myclose(file0);*/
	
	FILE* file;
	//fopen_s(&file, (char*)"c:\\prenos\\remc2\\biggraphics\\out_rlt-n-out.data", (char*)"rb");
	fopen_s(&file, path2, (char*)"rb");
	fseek(file, 0L, SEEK_END);
	long szdata = ftell(file);
	fseek(file, 0L, SEEK_SET);
	fread(buffer, szdata, 1, file);
	myclose(file);
};