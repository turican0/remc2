#include "Regression-tests.h"

int run_regtest(int level)//236F70
{
	Logger->info("Testing Level{}", level);

	unitTests = true;
	std::string locUnitTestsPath = get_exe_path() + "/memimages/regressions/level" + std::to_string(level);
	unitTestsPath = locUnitTestsPath;
	int locEndTestsCode;
	endTestsCode = &locEndTestsCode;

	int argc = 7;
	char* argv[7];
	char arg1[] = "remc2";
	char arg2[] = "--mode_test_regressions_game";
	char arg3[] = "--text_output_to_console";
	char arg4[] = "reglevel";
	char arg5[4];
	sprintf(arg5, "%d", level - 1);
	char arg6[] = "--config_file_path";

	std::string path = get_exe_path() + "/regression-config.ini";
	char* arg7 = &path[0];
	char* envp[] = { nullptr };
	argv[0] = arg1;
	argv[1] = arg2;
	argv[2] = arg3;
	argv[3] = arg4;
	argv[4] = arg5;
	argv[5] = arg6;
	argv[6] = arg7;

	for (int i = 0; i < 100; i++)
	{
		compstr[i].adress = 0;
		compstr[i].index = 0;
	}

	CommandLineParams.Init(argc, argv);
	support_begin();
	x_BYTE_D4B80 = 0;
	CleanF5538_716A0();
	for (int i = 0; i < 504; i++)
		str_DWORD_F66F0x[i] = nullptr;
	x_DWORD_E9C28_str = nullptr;

	try
	{
		sub_main(argc, argv, envp);
	}
	catch (const thread_exit_exception& e){}
	catch (const std::exception& e)
	{
		Logger->error("Exception running main thread: {}", e.what());
	}

	support_end();
	if (locEndTestsCode == 20)
		Logger->info("Test Level{} - OK", level);
	else
	{
		Logger->error("Test Level{} - FAILED", level);
		return -1;
	}
	return 0;
}