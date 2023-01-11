#include "read_config.h"
#include "../engine/CommandLineParser.h"

#include <vector>
#include <filesystem>
#include <cstdlib>

int config_skip_screen;
int texturepixels = 32;
int maxGameFps = 30;
int fmvFps = 20;
int menuFps = 30;
std::string loggingLevel = "Info";
int displayIndex = 0;
int windowResWidth = 640;
int windowResHeight = 480;
int gameResWidth = 640;
int gameResHeight = 480;
bool maintainAspectRatio = false;
bool forceWindow = false;
bool bigTextures = false;
bool bigSprites = false;
bool sky = true;
bool reflections = false;
bool dynamicLighting = false;
bool multiThreadedRender = false;
int numberOfRenderThreads = 0;
bool assignToSpecificCores = false;
bool openGLRender = false;

std::string findIniFile() {
	// find location of inifile and read it
	std::vector<std::string> inifile_locations;
	if (CommandLineParams.GetConfigFilePath().length() > 0) {
		inifile_locations.push_back(CommandLineParams.GetConfigFilePath());
	}
	else {
#ifdef __linux__
		auto env_home_dir = std::getenv("HOME");
		auto env_xdg_config_home_dir = std::getenv("XDG_CONFIG_HOME");
		std::filesystem::path home_dir;
		std::filesystem::path xdg_config_home_dir;
		if (env_home_dir) home_dir = env_home_dir;
		if (env_xdg_config_home_dir) xdg_config_home_dir = env_xdg_config_home_dir;

		if (std::filesystem::exists(xdg_config_home_dir)) {
			inifile_locations.emplace_back(xdg_config_home_dir / "remc2" / "config.ini");
		}
		if (std::filesystem::exists(home_dir)) {
			inifile_locations.emplace_back(home_dir / ".config" / "remc2" / "config.ini");
		}
#else //__linux__
		auto home_drive = std::getenv("HOMEDRIVE");
		auto home_path = std::getenv("HOMEPATH");
		if (home_drive && home_path) {
			std::string home_dir = std::string(home_drive) + "/" + std::string(home_path);
			inifile_locations.push_back(home_dir + "/remc2/config.ini");
		}
#endif //__linux__
		inifile_locations.push_back(get_exe_path() + "/config.ini");
	}
	std::string inifile;
	// first location at which an inifile can be found is chosen
	for (auto inifile_location: inifile_locations) {
		if (std::filesystem::exists(inifile_location)) {
			inifile = inifile_location;
			break;
		}
	}


	return inifile;
}

bool readini() {
	std::string inifile = findIniFile();
	if (std::filesystem::exists(inifile)) {
		if (CommandLineParams.DoShowDebugMessages1())
			std::cout << "Using inifile: " << inifile << "\n";
	}
	else {
		if (CommandLineParams.DoShowDebugMessages1())
			std::cout << "Inifile cannot be found... Exiting\n";
		return false;
	}

	INIReader reader(inifile);

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'test.ini'\n";
		return false;
	}
	if (reader.GetBoolean("skips", "skipintro", true))
		config_skip_screen = 1;
	else
		config_skip_screen = 0;

	if (reader.GetBoolean("sound", "hqsound", true))
		hqsound = true;
	else
		hqsound = false;

	if (reader.GetBoolean("sound", "fixspeedsound", true))
		fixspeedsound = true;
	else
		fixspeedsound = false;

	if (reader.GetBoolean("sound", "oggmusic", true))
	{
		oggmusic = true;
		hqsound = true;//for mp3 music must be activate hqsound
	}
	else
		oggmusic = false;
	if (reader.GetBoolean("sound", "oggmusicalternative", true))
	{
		oggmusicalternative = true;
	}
	else
	{
		oggmusicalternative = false;
	}

	std::string readstr = reader.GetString("sound", "oggmusicFolder", "");
	strcpy(oggmusicFolder, (char*)readstr.c_str());

	std::string readstr3 = reader.GetString("graphics", "bigGraphicsFolder", "");
	strcpy(bigGraphicsFolder, (char*)readstr3.c_str());

	if (reader.GetBoolean("graphics", "useEnhancedGraphics", false) && strlen(bigGraphicsFolder) > 0)
	{
		bigSprites = true;
		bigTextures = true;
		texturepixels = 128;
	}
	else
	{
		texturepixels = 32;
	}

	displayIndex = reader.GetInteger("graphics", "displayIndex", 0);
	windowResWidth = reader.GetInteger("graphics", "windowResWidth", 640);
	windowResHeight = reader.GetInteger("graphics", "windowResHeight", 480);

	if (windowResWidth < 640 || windowResHeight < 480)
	{
		windowResWidth = 640;
		windowResHeight = 480;
	}

	gameResWidth = reader.GetInteger("graphics", "gameResWidth", 640);
	gameResHeight = reader.GetInteger("graphics", "gameResHeight", 480);

	if (gameResWidth < 640 || gameResHeight < 480)
	{
		gameResWidth = 640;
		gameResHeight = 480;
	}

	maintainAspectRatio = reader.GetBoolean("graphics", "maintainAspectRatio", true);
	forceWindow = reader.GetBoolean("graphics", "forceWindow", false);
	sky = reader.GetBoolean("graphics", "sky", true);
	reflections = reader.GetBoolean("graphics", "reflections", false);
	dynamicLighting = reader.GetBoolean("graphics", "dynamicLighting", false);

	std::string readstr2 = reader.GetString("main", "gameFolder", "");
	strcpy((char*)gameFolder, (char*)readstr2.c_str());
	std::string readstr4 = reader.GetString("main", "cdFolder", "");
	strcpy((char*)cdFolder, (char*)readstr4.c_str());

	openGLRender = reader.GetBoolean("graphics", "openGLRender", false);

	if (!openGLRender)
	{
		multiThreadedRender = reader.GetBoolean("graphics", "multiThreadedRender", false);
		numberOfRenderThreads = reader.GetInteger("graphics", "numberOfRenderThreads", 0);

		if (multiThreadedRender)
		{
			assignToSpecificCores = reader.GetBoolean("graphics", "assignToSpecificCores", false);

			if (numberOfRenderThreads < 1)
			{
				numberOfRenderThreads = 1;
			}
		}
		else
		{
			numberOfRenderThreads = 0;
		}
	}

	maxGameFps = reader.GetInteger("game", "maxGameFps", 0);
	fmvFps = reader.GetInteger("game", "fmvFps", 20);
	loggingLevel = reader.GetString("game", "loggingLevel", "Info");

	return true;
};