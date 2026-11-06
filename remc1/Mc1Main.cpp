#include "Mc1Main.h"

#include <cstdio>
#include <exception>
#include <iostream>

#include "engine/Mc1Game.h"
#include "engine/Select.h"
#include "../remc2/engine/CommandLineParser.h"
#include "../remc2/engine/read_config.h"
#include "../remc2/portability/port_filesystem.h"
#include "../remc2/portability/port_time.h"
#include "../remc2/shared/EventDispatcher.h"

int Mc1Main(int /*argc*/, char** /*argv*/)
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
		// the batch file starts CARPET.EXE or HIDDEN.EXE without arguments
		mc1Variant = game == SELECTED_HIDDEN_WORLDS ? MC1_VARIANT_HW : MC1_VARIANT_MC1;
		char exeName[16];
		snprintf(exeName, sizeof(exeName), "%s", mc1Variant == MC1_VARIANT_HW ? "HIDDEN.EXE" : "CARPET.EXE");
		char* gameArgv[] = { exeName, nullptr };
		Main_404F0_40830(1, gameArgv);
	}

	delete EventDispatcher::I;
	EventDispatcher::I = nullptr;
	return 0;
}
