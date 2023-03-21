#include "port_filesystem.h"
#include "../engine/CommandLineParser.h"
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
char forceRender[512] = "";
spdlog::logger* Logger = nullptr;

#ifndef _MSC_VER
	#include <libgen.h>
	#include <iostream>
	#include <unistd.h>
	#include <stdarg.h>
	#include <sys/stat.h>
    #include <cstdio>
#endif

const char* GetStringFromLoggingLevel(spdlog::level::level_enum level)
{
	const char* level_enum_str[] = { "trace", "debug", "info", "warn", "err", "critical" };
	return level_enum_str[level];
}

spdlog::level::level_enum GetLoggingLevelFromString(const char* levelStr)
{
	spdlog::level::level_enum level = spdlog::level::info;

	if (strcmp(levelStr, "Info") == 0)
		level = level = spdlog::level::info;
	else if (strcmp(levelStr, "Warn") == 0)
		level = spdlog::level::warn;
	else if (strcmp(levelStr, "Debug") == 0)
		level = spdlog::level::debug;
	else if (strcmp(levelStr, "Trace") == 0)
		level = spdlog::level::trace;
	else if (strcmp(levelStr, "Error") == 0)
		level = spdlog::level::err;
	else if (strcmp(levelStr, "Critcal") == 0)
		level = spdlog::level::critical;

	return level;
}
void InitializeLogging(spdlog::level::level_enum level)
{
	try
	{
		if (Logger == nullptr)
		{
			auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console_sink->set_level(level);
			console_sink->set_pattern("[%H:%M:%S %z] [%^%-8l%$] %v");

			auto max_size = 1048576 * 5;
			auto max_files = 3;
			auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log.txt", max_size, max_files);
			file_sink->set_level(level);
			file_sink->set_pattern("[%H:%M:%S:%f %z] [%^%-8l%$] %v");

			Logger = new spdlog::logger("multi_sink", { console_sink, file_sink });
			Logger->set_level(level);
			auto levelStr = GetStringFromLoggingLevel(level);
			Logger->info("Logging Initialized with Level: {}", levelStr);
		}
		else
		{
			Logger->warn("Logging already Initialized");
		}
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log init failed: " << ex.what() << std::endl;
	}
}

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
	Logger->debug("my_findfirst:fixed:{}", path);
	long result= _findfirst(path, c_file);
	Logger->debug("my_findfirst:end:{}", result);
	return result;
}

long my_findnext(long hFile, _finddata_t* c_file){
	long result = _findnext(hFile, c_file);
	Logger->debug("my_findnext:end:{}", result);
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
		Logger->debug("file_exists:true-{}", filename);
		return true;
	}
	Logger->debug("file_exists:false-{}", filename);
	return false;
}

FILE* mycreate(const char* path, uint32_t  /*flags*/) {
	FILE *fp = fcaseopen(path, "wb+");
	Logger->debug("mycreate:{}", fmt::ptr(fp));
	return fp;
};

int32_t myaccess(const char* path, uint32_t  /*flags*/) {
	DIR *dir;
	//char path2[2048] = "\0";
	Logger->debug("myaccess:orig path:{}", path);
	//pathfix(path, path2);//only for DOSBOX version
	//	Logger->debug("myaccess:fix path:%s\n", path2);
	dir = opendir(path);
	Logger->debug("myaccess:exit:{} {}", fmt::ptr(dir), errno);
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

	Logger->debug("mymkdir:path: {}", path);

	//pathfix(path, path2);//only for DOSBOX version
	//Logger->debug("mymkdir:path2: %s\n", path2);

#ifdef WIN32
	const WCHAR *pwcsName;
	// required size
	int nChars = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, 0);
	// allocate it
	pwcsName = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, path, -1, (LPWSTR)pwcsName, nChars);
	// use it....
	Logger->debug("mymkdir:path3: {}", fmt::ptr(pwcsName));
#endif

	int result;
#if defined (WIN32)						/* MS Visual C++ */
	result = _wmkdir(pwcsName);
	delete[] pwcsName;
#else
	result = mkdir(path, 0700);
#endif
	// delete it
	Logger->debug("mymkdir:end: {}", result);
	return result;
};

FILE* myopen(const char* path, int pmode, uint32_t flags) {

	Logger->debug("myopen:open file: {}", path);
	//bool localDrive::FileOpen(DOS_File * * file, const char * name, uint32_t flags) {
	const char * type;
	if ((pmode == 0x222) && (flags == 0x40))type = "rb+";
	else if ((pmode == 0x200) && (flags == 0x40))type = "rb+";
	else
		exit(1);//error - DOSSetError(DOSERR_ACCESS_CODE_INVALID);
	FILE* fp = nullptr;
	//char path2[512] = "\0";
	//pathfix(path, path2);//only for DOSBOX version
	//	Logger->debug("myopen:open file:{}", path2);
	//if(file_exists(path2))

	fp= fcaseopen(path, type);
	Logger->debug("myopen:open end {}", fmt::ptr(fp));
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
	Logger->debug("myopent:end: {}", fmt::ptr(fp));
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
		Logger->error("Could not open current directory1 {}", dirname);
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

		Logger->debug("ffile_exists:true-{}", path);

		return true;
	}

	Logger->debug("ffile_exists:false-{}", path);

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
	for (const std::string &file_location: file_locations) {
#ifdef __linux__
		std::string caseInsensitivePath = casepath(file_location);
		if (std::filesystem::exists(caseInsensitivePath)) {
			file_found = std::string(caseInsensitivePath);
			break;
		}
#else //__linux__
		if (std::filesystem::exists(file_location)) {
			file_found = file_location;
			break;
		}
#endif //__linux__
	}
	if (CommandLineParams.DoShowDebugMessages1())
		std::cout << "Data file found: " << file_found << "\n";
	return file_found;
}

std::string GetSubDirectoryPath(const char* subDirectory)
{
	std::string path = getExistingDataPath(subDirectory);
	return path.c_str();
}

std::string GetSubDirectoryPath(const char* gamepath, const char* subDirectory)
{
	std::string path = getExistingDataPath(
		std::filesystem::path(gamepath) / std::filesystem::path(subDirectory)
	);
	return path.c_str();
}

std::string GetSubDirectoryFilePath(const char* subDirectory, const char* fileName)
{
	std::string subDirPath = GetSubDirectoryPath(subDirectory);
	return subDirPath + "/" + std::string(fileName);
}

std::string GetSubDirectoryFile(const char* gamepath, const char* subDirectory, const char* fileName)
{
	std::string subDirPath = GetSubDirectoryPath(gamepath, subDirectory);
	return subDirPath + "/" + std::string(fileName);
}

std::string GetSaveGameFile(const char* gamepath, int16_t index)
{
	std::string subDirPath = GetSubDirectoryPath(gamepath, "SAVE");
	char buffer[MAX_PATH];
	sprintf(buffer, "%s/SAVE%d.GAM", subDirPath.c_str(), index);
	return std::string(buffer);
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
