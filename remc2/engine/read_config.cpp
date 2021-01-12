#include "read_config.h"
int config_skip_screen;
int texturepixels = 32;
int speedGame = 35;
int speedAnim = 100;
bool res640x480 = false;
int gameResWidth = 640;
int gameResHeight = 480;
bool maintainAspectRatio = false;
bool bigTextures = false;
bool bigSprites = false;

void readini(const std::string& filename) {

	INIReader reader(filename);

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'test.ini'\n";
		return;
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

	std::string readstrgd = reader.GetString("graphics", "defaultresolution", "");
	if (!strcmp("640x480", (char*)readstrgd.c_str()))
		res640x480 = true;

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

	gameResWidth = reader.GetInteger("graphics", "gameResWidth", 640);
	gameResHeight = reader.GetInteger("graphics", "gameResHeight", 480);

	if (gameResWidth < 640 || gameResHeight < 480)
	{
		gameResWidth = 640;
		gameResHeight = 480;
	}
	
	maintainAspectRatio = reader.GetBoolean("graphics", "maintainAspectRatio", true);

	std::string readstr2 = reader.GetString("main", "gameFolder", "");
	strcpy((char*)gameFolder, (char*)readstr2.c_str());
	std::string readstr4 = reader.GetString("main", "cdFolder", "");
	strcpy((char*)cdFolder, (char*)readstr4.c_str());

	speedGame = reader.GetInteger("game", "speed", 35);
	speedAnim = reader.GetInteger("game", "animspeed", 100);
};