#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _MSC_VER
    #include <direct.h>  
    #include <io.h>  
    #include <windows.h>
    #include "dirent-x.h"
#else
    #include "dirent.h"
#endif

#include "port_time.h"
#include "port_filesystem.h"
#include "port_sdl_sound.h"
#include "defs.h"

#include "INIReader.h"
#include "ini.h"



#ifndef READ_CONFIG
#define READ_CONFIG

bool readini();

typedef struct
{
	int config_skip_screen;
	int texturepixels;
	int speedGame;
	int speedAnim;
	int windowResWidth;
	int windowResHeight;
	int gameResWidth;
	int gameResHeight;
	bool maintainAspectRatio;
	bool bigTextures;
	bool bigSprites;
	bool sky;
	bool reflections;
	bool dynamicLighting;
	bool openGLRender;
	bool multiThreadedRender;
	int numberOfRenderThreads;
	bool assignToSpecificCores;
	bool hqsound;
	bool oggmusic;
	bool oggmusicalternative;
	bool fixspeedsound;
	std::filesystem::path oggmusicFolder;
	std::filesystem::path gameFolder;
	std::filesystem::path cdFolder;        
	std::filesystem::path bigGraphicsFolder;
	std::filesystem::path dataDir;
	std::filesystem::path gameDataPath;
	std::filesystem::path cdDataPath;
	std::filesystem::path bigGraphicsPath;
} config_t;

extern config_t settings;

#endif //READ_CONFIG
