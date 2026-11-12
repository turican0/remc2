#include "Mc1Main.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "engine/Mc1Files.h"
#include "engine/Mc1Game.h"
#include "engine/Mc1Screen.h"
#include "engine/Mc1System.h"
#include "engine/Select.h"
#include "../remc2/engine/CommandLineParser.h"
#include "../remc2/engine/read_config.h"
#include "../remc2/portability/port_filesystem.h"
#include "../remc2/portability/port_sdl_vga_mouse.h"
#include "../remc2/portability/port_time.h"
#include "../remc2/shared/EventDispatcher.h"

int Mc1Main(int argc, char** argv)
{
	SetTimeStart();
	std::cout << "Initializing logger...\n";
	InitializeLogging(GetLoggingLevelFromString(CommandLineParams.GetLogLevelStr().c_str()));
	try
	{
		SetConfig();
	}
	catch (const std::exception& e)
	{
		Logger->critical("Error reading config.json file: {}", e.what());
		return -1;
	}
	EventDispatcher::I = new EventDispatcher();

	// -game select shows the Magic Carpet Plus selector, -game mc1 / mc1hw start the game directly
	int16_t game = SELECTED_MAGIC_CARPET;
	if (CommandLineParams.GetGameType() == GameType::SELECT)
	{
		game = RunSelect_10010(GetSubDirectoryPath("MC1"));
		Logger->info("MC1 selector result: {}", game);
	}
	else if (CommandLineParams.GetGameType() == GameType::MC1HW)
		game = SELECTED_HIDDEN_WORLDS;
	if (game != SELECTED_NONE)
	{
		// the game gets the parameters remc2 does not know (e.g. -level 5 -cheat 1), as on the DOS command line
		mc1Variant = game == SELECTED_HIDDEN_WORLDS ? MC1_VARIANT_HW : MC1_VARIANT_MC1;
		mc1DataPath = GetSubDirectoryPath("MC1");
		std::vector<std::string> gameArgs;
		gameArgs.push_back(argc > 0 ? argv[0] : "remc2");
		for (const std::string& param : CommandLineParams.GetUnknownParams())
			gameArgs.push_back(param);
		std::vector<char*> gameArgv;
		for (std::string& arg : gameArgs)
			gameArgv.push_back(&arg[0]);
		gameArgv.push_back(nullptr);
		Main_404F0_40830((int)gameArgs.size(), gameArgv.data());
		// the original left its memory to DOS
		FreeMc1Memory();
		FreeCursorBuffers();
		*workScreen_12EFF4_12EFE4 = nullptr;
		mc1State_AE400_AE3F0 = nullptr;
		mc1Settings_AE408_AE3F8 = nullptr;
		vesaInfo_12F080_12F070 = nullptr;
		*blurBuffer_AE404_AE3F4 = nullptr;
	}

	// one window for the selector and the game, closed once at the end
	VGA_close();
	delete EventDispatcher::I;
	EventDispatcher::I = nullptr;
	return 0;
}
