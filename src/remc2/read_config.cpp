#include "read_config.h"
#include "port_filesystem.h"

config_t settings =
{
	.config_skip_screen = 0,
	.texturepixels = 32,
	.speedGame = 35,
	.speedAnim = 100,
	.windowResWidth = 640,
	.windowResHeight = 480,
	.gameResWidth = 640,
	.gameResHeight = 480,
	.maintainAspectRatio = false,
	.bigTextures = false,
	.bigSprites = false,
	.sky = true,
	.reflections = false,
	.dynamicLighting = false,
	.openGLRender = false,
	.multiThreadedRender = false,
	.numberOfRenderThreads = 0,
	.assignToSpecificCores = false,
	.hqsound = true,
	.oggmusic = true,
	.oggmusicalternative = true,
	.fixspeedsound = false,
//char gamepath[512] = "c:\\prenos\\Magic2\\mc2-orig-copy";
	.gameFolder        = "NETHERW",
	.cdFolder          = "gamedisc",
	.bigGraphicsFolder = "graphics/128x128",
#ifdef _WIN32
	.dataDir = get_exe_path(),
#else
	.dataDir = DATADIR ? DATADIR : "/usr/share/remc2",
#endif
};

std::filesystem::path findIniFile() {
	// find location of inifile and read it
	std::vector<std::filesystem::path> inifile_locations;
#ifdef _WIN32
	auto home_drive = std::getenv("HOMEDRIVE");
	auto home_path =  std::getenv("HOMEPATH");
	if (home_drive && home_path) {
		std::string home_dir = std::string(home_drive) + "/" + std::string(home_path);
		inifile_locations.push_back(home_dir + "/remc2/config.ini");
	}
#else
	auto env_home_dir = std::getenv("HOME");
	auto env_xdg_config_home_dir = std::getenv("XDG_CONFIG_HOME");
	std::filesystem::path home_dir;
	std::filesystem::path xdg_config_home_dir;
	std::filesystem::path data_dir;
	if (env_home_dir) home_dir = env_home_dir;
	if (env_xdg_config_home_dir) xdg_config_home_dir = env_xdg_config_home_dir;
	if (DATADIR) data_dir = DATADIR;

	if (std::filesystem::exists(xdg_config_home_dir)) {
		inifile_locations.emplace_back(xdg_config_home_dir / "remc2" / "config.ini");
	}

	if (std::filesystem::exists(home_dir)) {
		inifile_locations.emplace_back(home_dir / ".config" / "remc2" / "config.ini");
	}

	if (std::filesystem::exists(data_dir)) {
		inifile_locations.emplace_back(data_dir / "config.ini");
	}
#endif
	inifile_locations.push_back(get_exe_path() / "config.ini");
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
		std::cout << "Using inifile: " << inifile << "\n";
	}
	else {
		std::cout << "Inifile cannot be found... Exiting\n";
		return false;
	}

	INIReader reader(inifile);

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'test.ini'\n";
		return false;
	}
	if (reader.GetBoolean("skips", "skipintro", true))
		settings.config_skip_screen = 1;
	else
		settings.config_skip_screen = 0;

	if (reader.GetBoolean("sound", "hqsound", true))
		settings.hqsound = true;
	else
		settings.hqsound = false;

	if (reader.GetBoolean("sound", "fixspeedsound", true))
		settings.fixspeedsound = true;
	else
		settings.fixspeedsound = false;

	if (reader.GetBoolean("sound", "oggmusic", true))
	{
		settings.oggmusic = true;
		settings.hqsound = true;//for mp3 music must be activate hqsound
	}
	else
		settings.oggmusic = false;
	if (reader.GetBoolean("sound", "oggmusicalternative", true))
	{
		settings.oggmusicalternative = true;
	}
	else
	{
		settings.oggmusicalternative = false;
	}

	settings.oggmusicFolder = reader.GetString("sound", "oggmusicFolder", "");
	settings.bigGraphicsFolder = reader.GetString("graphics", "bigGraphicsFolder", "");

	if (reader.GetBoolean("graphics", "useEnhancedGraphics", false) && settings.bigGraphicsFolder.native().size())
	{
		settings.bigSprites = true;
		settings.bigTextures = true;
		settings.texturepixels = 128;
	}
	else
	{
		settings.texturepixels = 32;
	}

	settings.gameResWidth = reader.GetInteger("graphics", "gameResWidth", 640);
	settings.gameResHeight = reader.GetInteger("graphics", "gameResHeight", 480);

	if (settings.gameResWidth < 640 || settings.gameResHeight < 480)
	{
		settings.gameResWidth = 640;
		settings.gameResHeight = 480;
	}

	settings.windowResWidth = reader.GetInteger("graphics", "windowResWidth", 640);
	settings.windowResHeight = reader.GetInteger("graphics", "windowResHeight", 480);

	if (settings.windowResWidth < 640 || settings.windowResHeight < 480)
	{
		settings.windowResWidth = 640;
		settings.windowResHeight = 480;
	}

	if (settings.windowResWidth < settings.gameResWidth) 
	{
		settings.windowResWidth = settings.gameResWidth;
	}

	if (settings.windowResHeight < settings.gameResHeight)
	{
		settings.windowResHeight = settings.gameResHeight;
	}

	settings.maintainAspectRatio = reader.GetBoolean("graphics", "maintainAspectRatio", true);
	settings.sky = reader.GetBoolean("graphics", "sky", true);
	settings.reflections = reader.GetBoolean("graphics", "reflections", false);
	settings.dynamicLighting = reader.GetBoolean("graphics", "dynamicLighting", false);

	settings.gameFolder = reader.GetString("main", "gameFolder", "");
	settings.cdFolder = reader.GetString("main", "cdFolder", "");

	settings.speedGame = reader.GetInteger("game", "speed", 30);
	settings.speedAnim = reader.GetInteger("game", "animspeed", 100);

	settings.openGLRender = reader.GetBoolean("graphics", "openGLRender", false);

	if (!settings.openGLRender)
	{
		settings.multiThreadedRender = reader.GetBoolean("graphics", "multiThreadedRender", false);
		settings.numberOfRenderThreads = reader.GetInteger("graphics", "numberOfRenderThreads", 0);

		if (settings.multiThreadedRender)
		{
			settings.assignToSpecificCores = reader.GetBoolean("graphics", "assignToSpecificCores", false);

			if (settings.numberOfRenderThreads < 1)
			{
				settings.numberOfRenderThreads = 1;
			}
		}
		else
		{
			settings.numberOfRenderThreads = 0;
		}
	}

	return true;
};
