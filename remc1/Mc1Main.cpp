#include "Mc1Main.h"

#include <exception>
#include <iostream>

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

	// the original batch file starts CARPET.EXE or HIDDEN.EXE by the choice, the games are not ported yet
	int16_t game = RunSelect_10010(GetSubDirectoryPath("MC1"));
	Logger->info("MC1 selector result: {}", game);

	delete EventDispatcher::I;
	EventDispatcher::I = nullptr;
	return 0;
}
