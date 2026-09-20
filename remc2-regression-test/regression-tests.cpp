#include "regression-tests.h"
#include "../remc2/engine/EventsFunctions.h"
#include "../remc2/engine/PlayerInput.h"
#include "../remc2/engine/Level.h"
#include "../remc2/engine/read_config.h"
#include <filesystem>

// Per-run state of the engine that the tests depend on.  All tests run in one process, and without
// this each one starts where the previous one left off: IsAfterLoad stays true after a level test,
// so an afterload test compared frame 0 against a memimage taken after the load (which only
// happens on frame 5) and failed on step 0.
extern int debug_first_run;
extern int count_begin;
extern int countcompindexes;
extern type_compstr lastcompstr;
extern int debugcounter_1fb7a0;

static void ResetRegressionRunState()
{
	IsAfterLoad = false;
	debug_first_run = 0;
	count_begin = 1;
	debugcounter_47560 = 0;
	debugcounter_1fb7a0 = 0;
	save_debugcounter = 0;
	countcompindexes = 0;
	lastcompstr.adress = 0;
	lastcompstr.index = 0;
}

bool resaveRecordings = false;

// memimages/regressions next to the exe, else the one of the sources
std::string RegressionsPath()
{
	const std::string nextToExe = get_exe_path() + "/memimages/regressions";
	if (std::filesystem::exists(nextToExe))
		return nextToExe;
	return (std::filesystem::path(__FILE__).parent_path() / "memimages" / "regressions").string();
}

int run_regtest(int level, int testType, int index, int saveIndex, const char* recordName, int maxSteps, bool turnOnIntervalSave, const char* recordFolder)//236F70
{
	int exitCode = 0;
	const std::string testName = strlen(recordFolder) > 0 ? std::string(recordFolder) : "aftreload " + std::to_string(index);
	Logger->info("Testing {} for Level {}", testName, level);

	unitTests = true;
	menuFps = 0;//no fps limit in the tests, maxGameFps is 0 in regression-config.json
	std::string locUnitTestsPath;
	std::string recordPath = "";
	if (testType>0)
	{
		locUnitTestsPath = RegressionsPath() + "/afterloadtest" + std::to_string(index);
		if (strlen(recordFolder) > 0)//<folder>/<recording>, <folder>/level<N>/sequence-*
			locUnitTestsPath = RegressionsPath() + "/" + recordFolder + "/level" + std::to_string(level);
		if (strlen(recordName) > 0)//in memimages/regressions
			recordPath = RegressionsPath() + "/" + (strlen(recordFolder) > 0 ? std::string(recordFolder) + "/" : "") + recordName;
	}
	else
		locUnitTestsPath = RegressionsPath() + "/level" + std::to_string(level);
	unitTestsPath = locUnitTestsPath;
	int locEndTestsCode = 0;
	endTestsCode = &locEndTestsCode;

	std::vector<std::string> args;
	args.reserve(20);

	std::string path = get_exe_path() + "/regression-config.json";

	args.emplace_back("remc2");

	if (testType>0)
	{
		//args.emplace_back("--mode_debug_afterload");
		args.emplace_back("--mode_test_regressions");
		args.emplace_back(std::to_string(testType));
		args.emplace_back("--mode_test_save_index");
		args.emplace_back(std::to_string(saveIndex));
		args.emplace_back("--text_output_to_console");
		args.emplace_back("--set_level");
		args.emplace_back(std::to_string(level - 1));
		args.emplace_back("--config_file_path");
		args.emplace_back(path);
		if ((testType == 1) || (testType == 3))
			args.emplace_back("--debugafterload");
		//args.emplace_back("--is_recorded_regtest");
		if (recordPath != "")
		{
			args.emplace_back("--play_file");
			args.emplace_back(recordPath);
			if (resaveRecordings)
			{
				args.emplace_back("--record_file");//rewritten with level saves
				args.emplace_back(recordPath);
			}
		}
		args.emplace_back("--set_max_regressions_steps");
		args.emplace_back(std::to_string(maxSteps));
		if(turnOnIntervalSave)
			args.emplace_back("--interval_save");
	}
	else
	{
		//args.emplace_back("--mode_test_regressions_game");
		args.emplace_back("--mode_test_regressions");
		args.emplace_back(std::to_string(testType));
		args.emplace_back("--text_output_to_console");
		args.emplace_back("--set_level");
		args.emplace_back(std::to_string(level - 1));
		args.emplace_back("--config_file_path");
		args.emplace_back(path);
		args.emplace_back("--set_max_regressions_steps");
		args.emplace_back(std::to_string(maxSteps));
		if (turnOnIntervalSave)
			args.emplace_back("--interval_save");
	}

	std::vector<char*> argv;
	argv.reserve(args.size());
	for (auto& s : args)
		argv.push_back(s.data());   // C++17+, null-terminated

	int argc = static_cast<int>(argv.size());

	char* envp[] = { nullptr };

	for (int i = 0; i < 100; i++)
	{
		compstr[i].adress = 0;
		compstr[i].index = 0;
	}
	ResetRegressionRunState();

	CommandLineParams.Init(argc, argv.data());
	support_begin();
	x_BYTE_D4B80 = 0;
	CleanF5538_716A0();
	for (int i = 0; i < 504; i++)
		str_DWORD_F66F0x[i] = nullptr;
	x_DWORD_E9C28_str = nullptr;

	try
	{
		sub_main(argc, argv.data(), envp);
	}
	catch (const thread_exit_exception& e) {}
	catch (const std::exception& e)
	{
		Logger->error("Exception running main thread: {}", e.what());
		exitCode = -1;
	}

	support_end();
	if (locEndTestsCode == 20)
		if (testType > 0)
			Logger->info("Test {} for Level {} - OK\n\n", testName, level);
		else
			Logger->info("Test Level {} - OK\n\n", level);
	else
	{
		if (testType > 0)
			Logger->info("Test {} for Level {} - FAILED\n\n", testName, level);
		else
			Logger->error("Test Level {} - FAILED\n\n", level);
		exitCode = -1;
	}
	return exitCode;
}