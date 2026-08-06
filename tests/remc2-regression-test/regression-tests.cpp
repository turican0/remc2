#include "regression-tests.h"

int run_regtest(int level, int testType, int index, int saveIndex, const char* recordName, int maxSteps, bool turnOnIntervalSave)//236F70
{
	int exitCode = 0;
	Logger->info("Testing aftreload {} for Level {}", index, level);

	unitTests = true;
	std::string locUnitTestsPath;
	std::string recordPath = "";
	if (testType>0)
	{
		locUnitTestsPath = get_exe_path() + "/memimages/regressions/afterloadtest" + std::to_string(index);
		if(strlen(recordName) > 0)
			recordPath = get_exe_path() + "/memimages/regressions/afterloadtest" + std::to_string(index) + "/" + recordName;
	}
	else
		locUnitTestsPath = get_exe_path() + "/memimages/regressions/level" + std::to_string(level);
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
			Logger->info("Test aftreload {} for Level {} - OK\n\n", index, level);
		else
			Logger->info("Test Level {} - OK\n\n", level);
	else
	{
		if (testType > 0)
			Logger->info("Test aftreload {} for Level {} - FAILED\n\n", index, level);
		else
			Logger->error("Test Level {} - FAILED\n\n", level);
		exitCode = -1;
	}
	return exitCode;
}