#include "port_filesystem.h"
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
using namespace std;
/*
#ifndef DIR
#define DIR __dirstream_t *
#endif
*/

//char gamepath[512] = "c:\\prenos\\Magic2\\mc2-orig-copy";
char gameFolder[512] = "NETHERW";
char cdFolder[512] = "CD_Files";
char bigGraphicsFolder[512] = "bigGraphics";

#ifndef _MSC_VER
	#include <libgen.h>
	#include <iostream>
	#include <unistd.h>
	#include <stdarg.h>
	#include <sys/stat.h>
    #include <cstdio>
#endif

#ifdef _MSC_VER
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
#endif

bool firstrun = true;

std::string get_exe_path() {
#ifdef _MSC_VER
	LPWSTR buffer = new WCHAR[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string locstr = utf8_encode(buffer);
	delete[] buffer;
	std::string::size_type pos = std::string(locstr).find_last_of("\\/");
	std::string strpathx = std::string(locstr).substr(0, pos)/*+"\\system.exe"*/;
	return strpathx;
#else
	std::string strpathx;
	char result[ PATH_MAX ];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	if (count != -1) {
		result[count] = '\0';
		strpathx = dirname(result);
	}
	return strpathx;
#endif
};

long my_findfirst(char* path, _finddata_t* c_file){
	#ifdef DEBUG_START
		debug_printf("my_findfirst:%s\n", path);
	#endif //DEBUG_START
	#ifdef DEBUG_START
		debug_printf("my_findfirst:fixed:%s\n", path);
	#endif //DEBUG_START
	long result= _findfirst(path, c_file);
	#ifdef DEBUG_START
			debug_printf("my_findfirst:end:%d\n", result);
	#endif //DEBUG_START
	return result;
}

long my_findnext(long hFile, _finddata_t* c_file){
	long result = _findnext(hFile, c_file);
	#ifdef DEBUG_START
		debug_printf("my_findnext:end:%d\n", result);
	#endif //DEBUG_START
	return result;
}

void my_findclose(long hFile){
	_findclose(hFile);
};

bool file_exists(const char * filename) {
	/*if (FILE * file = fcaseopen(filename, "r")) {
		fclose(file);
		return true;
	}
	return false;*/
	FILE* file;
	if ((file = fcaseopen(filename, "r")) != NULL) {
		fclose(file);
		#ifdef DEBUG_START
				debug_printf("file_exists:true-%s\n", filename);
		#endif //DEBUG_START
		return true;
	}
	#ifdef DEBUG_START
		debug_printf("file_exists:false-%s\n", filename);
	#endif //DEBUG_START
	return false;
}

FILE* mycreate(const char* path, uint32_t  /*flags*/) {
	FILE *fp;
	fp = fcaseopen(path, "wb+");
	#ifdef DEBUG_START
		debug_printf("mycreate:%p\n",fp);
	#endif //DEBUG_START
	return fp;
};

FILE* debug_output;

bool debug_first = false;
const char* debug_filename = "../debug.txt";
std::string path = {};

void debug_printf(const char* format, ...) {
	char prbuffer[1024];
#ifndef FLATPAK
	va_list arg;
	//int done;
	va_start(arg, format);
	vsprintf(prbuffer, format, arg);
	va_end(arg);

	std::string exepath = get_exe_path();
	path = exepath + "/" + debug_filename;

	if (debug_first)
	{
		debug_output = fcaseopen(path.c_str(), "wt");
		debug_first = false;
	}
	else
		debug_output = fcaseopen(path.c_str(), "at");
	fprintf(debug_output, "%s", prbuffer);
	fclose(debug_output);
	#ifdef DEBUG_PRINT_DEBUG_TO_SCREEN
		printf(prbuffer);
	#endif
#endif
}

int32_t myaccess(const char* path, uint32_t  /*flags*/) {
	DIR *dir;
	//char path2[2048] = "\0";
	#ifdef DEBUG_FILEOPS
		debug_printf("myaccess:orig path:%s\n", path);
	#endif //DEBUG_FILEOPS
	//pathfix(path, path2);//only for DOSBOX version
	//#ifdef DEBUG_FILEOPS
	//	debug_printf("myaccess:fix path:%s\n", path2);
	//#endif //DEBUG_FILEOPS
	dir = opendir(path);
	#ifdef DEBUG_FILEOPS
		debug_printf("myaccess:exit:%p %d\n", dir, errno);
	#endif //DEBUG_FILEOPS
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

int32_t /*__cdecl*/ mymkdir(const char* path) {
	//char path2[512] = "\0";
	#ifdef DEBUG_FILEOPS
		debug_printf("mymkdir:path: %s\n", path);
	#endif //DEBUG_FILEOPS
	//pathfix(path, path2);//only for DOSBOX version

	//#ifdef DEBUG_FILEOPS
	//	debug_printf("mymkdir:path2: %s\n", path2);
	//#endif //DEBUG_FILEOPS

#ifdef WIN32
	const WCHAR *pwcsName;
	// required size
	int nChars = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, 0);
	// allocate it
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, path, -1, (LPWSTR)pwcsName, nChars);
	// use it....

#ifdef DEBUG_FILEOPS
	debug_printf("mymkdir:path3: %s\n", pwcsName);
#endif //DEBUG_FILEOPS
#endif



	int result;
#if defined (WIN32)						/* MS Visual C++ */
	result = _wmkdir(pwcsName);
	delete[] pwcsName;
#else
	result = mkdir(path, 0700);
#endif
	// delete it

#ifdef DEBUG_FILEOPS
	debug_printf("mymkdir:end: %d\n", result);
#endif //DEBUG_FILEOPS
	return result;
};

FILE* myopen(char* path, int pmode, uint32_t flags) {
	#ifdef DEBUG_START
		debug_printf("myopen:open file:%s\n", path);
	#endif //DEBUG_START
	//bool localDrive::FileOpen(DOS_File * * file, const char * name, uint32_t flags) {
	const char * type;
	if ((pmode == 0x222) && (flags == 0x40))type = "rb+";
	else if ((pmode == 0x200) && (flags == 0x40))type = "rb+";
	else
		exit(1);//error - DOSSetError(DOSERR_ACCESS_CODE_INVALID);
	FILE *fp = NULL;
	//char path2[512] = "\0";
	//pathfix(path, path2);//only for DOSBOX version
	//#ifdef DEBUG_START
	//	debug_printf("myopen:open file:fixed:%s\n", path2);
	//#endif //DEBUG_START
	//if(file_exists(path2))

	fp= fcaseopen(path, type);
	#ifdef DEBUG_START
		debug_printf("myopen:open end %p\n", fp);
	#endif //DEBUG_START
	return fp;
};
int myclose(FILE* descriptor) {
	return fclose(descriptor);
};
int32_t mylseek(FILE* filedesc, x_DWORD position, char type) {
	return fseek(filedesc, position, type);
};

int32_t myfseek(FILE* filedesc, x_DWORD position, char type) {
	return fseek(filedesc, position, type);
};

long myftell(FILE* decriptor) {
	return ftell(decriptor);
};

int DirExists(const char* path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return -1;
}

FILE* myopent(char* path, char* type) {
	FILE *fp;
	fp= fcaseopen(path, type);
	#ifdef DEBUG_FILEOPS
		debug_printf("myopent:end: %p\n", fp);
	#endif //DEBUG_FILEOPS
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
	std::string pathexe = get_exe_path();
	sprintf(outdirname, "%s/%s", pathexe.c_str(), indirname);
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
#ifdef WIN32
	*a = _getdrive();
#else
	*a = 0;
	std::cerr << "STUB: dos_getdrive on Linux" << std::endl;
#endif
	return *a;
};

#ifdef _MSC_VER
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

	for (uint32_t i = 0; i < strlen(path); i++)
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

unsigned __int64 dos_getdiskfree(__int16 a1, __int16 a2, uint8_t a, short* b) {
	unsigned long wanted_size = 0;//fix it
	char drivename[10];
	sprintf(drivename, "%c:", (uint8_t)(a + 64));
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
#endif

void AdvReadfile(const char* path, uint8_t* buffer) {
	std::string pathexe = get_exe_path();
	std::string path2 = pathexe + "/" + std::string(path);
	/*
	FILE* file0;
	fopen_s(&file0, path2, (char*)"wb");
	char x = 1;
	fwrite(&x, 1, 1, file0);
	myclose(file0);
	*/
	FILE* file;
	//fopen_s(&file, (char*)"c:\\prenos\\remc2\\biggraphics\\out_rlt-n-out.data", (char*)"rb");
	file= fcaseopen(path2.c_str(), (char*)"rb");
	fseek(file, 0L, SEEK_END);
	long szdata = ftell(file);
	fseek(file, 0L, SEEK_SET);
	fread(buffer, szdata, 1, file);
	myclose(file);
};

bool ExistGraphicsfile(const char* path) {

	FILE* file;

	if ((file = fopen(path, "r")) != NULL) {
		fclose(file);
#ifdef DEBUG_START
		debug_printf("ffile_exists:true-%s\n", path);
#endif //DEBUG_START
		return true;
	}
#ifdef DEBUG_START
	debug_printf("ffile_exists:false-%s\n", path);
#endif //DEBUG_START
	return false;
}

void ReadGraphicsfile(const char* path, uint8_t* buffer, long size) 
{
	FILE* file;
	file = fcaseopen(path, (char*)"rb");
	if (file != NULL)
	{
		if (size == -1)
		{
			fseek(file, 0L, SEEK_END);
			size = ftell(file);
			fseek(file, 0L, SEEK_SET);
		}
		fread(buffer, size, 1, file);
		myclose(file);
	}
};

std::string getExistingDataPath(std::filesystem::path path) 
{
	std::vector<std::string> file_locations;
#ifdef __linux__
	auto env_home_dir = std::getenv("HOME");
	auto env_xdg_data_home_dir = std::getenv("XDG_DATA_HOME");
	std::filesystem::path home_dir;
	std::filesystem::path xdg_data_home_dir;
	if (env_home_dir) home_dir = env_home_dir;
	if (env_xdg_data_home_dir) xdg_data_home_dir = env_xdg_data_home_dir;

	if (std::filesystem::exists(xdg_data_home_dir)) {
		file_locations.emplace_back(xdg_data_home_dir / "remc2" / path);
	}
	if (std::filesystem::exists(home_dir)) {
		file_locations.emplace_back(home_dir / ".local" / "share" / "remc2" / path);
	}
#else //__linux__
	auto home_drive = std::getenv("HOMEDRIVE");
	auto home_path =  std::getenv("HOMEPATH");
	if (home_drive && home_path) {
		std::filesystem::path home_dir(std::string(home_drive) + "/" + std::string(home_path));
		file_locations.push_back((home_dir / "remc2" / path).string());
	}
#endif //__linux__
	file_locations.push_back(get_exe_path() + "/" + path.string());

	std::string file_found;

	// first location at which the file can be found is chosen
	for (std::string file_location : file_locations) {
#if !defined(_WIN32)
		char caseInsensitivePath[250];
		casepath(file_location.c_str(), caseInsensitivePath);
		if (std::filesystem::exists(caseInsensitivePath)) {
			file_found = std::string(caseInsensitivePath);
			break;
		}
#else
		if (std::filesystem::exists(file_location)) {
			file_found = file_location;
			break;
		}
#endif
	}

	std::cout << "Data file found: " << file_found << "\n";
	return file_found;
}

void GetSubDirectoryPath(char* buffer, const char* subDirectory)
{
	std::string path = getExistingDataPath(subDirectory);
	sprintf(buffer, "%s", path.c_str());
}

void GetSubDirectoryPath(char* buffer, const char* gamepath, const char* subDirectory)
{
	std::string path = getExistingDataPath(
		std::filesystem::path(gamepath) / std::filesystem::path(subDirectory)
	);
	sprintf(buffer, "%s", path.c_str());
}

void GetSubDirectoryFile(char* buffer, const char* gamepath, const char* subDirectory, const char* fileName)
{
	char subDirPath[MAX_PATH]; 
	GetSubDirectoryPath(subDirPath, gamepath, subDirectory);
	sprintf(buffer, "%s/%s", subDirPath, fileName);
}

void GetSaveGameFile(char* buffer, const char* gamepath, int16_t index)
{
	char subDirPath[MAX_PATH];
	GetSubDirectoryPath(subDirPath, gamepath, "SAVE");
	sprintf(buffer, "%s/SAVE%d.GAM", subDirPath, index);
}

int GetDirectory(char* directory, const char* filePath)
{
	string str = string(filePath);
	size_t found = str.find_last_of("/\\");

	if (found)
	{
		directory = strcpy(directory, str.substr(0, found).c_str());
		return 0;
	}
	return - 1;
}
