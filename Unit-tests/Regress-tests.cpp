#include "Regress-tests.h"

void sub_main_thread(int argc, char** argv, char**  envp)
{
	try
	{
		sub_main(argc, argv, envp);
	}
	catch (const thread_exit_exception&)
	{
	}
}

int run_regtest(int level)//236F70
{
	unitTests = true;
	std::string locUnitTestsPath = "./memimages/regressions/level" + std::to_string(level) + "/";
	unitTestsPath = locUnitTestsPath;
	int locEndTestsCode;
	endTestsCode = &locEndTestsCode;

	int argc = 4;
	char* argv[4];
	char* arg1 = "remc2";
	char* arg2 = "--mode_test_regressions_game";
	char* arg3 = "reglevel";
	char* envp[] = { nullptr };
	char arg4[4];
	sprintf(arg4, "%d", level - 1);
	argv[0] = arg1;
	argv[1] = arg2;
	argv[2] = arg3;
	argv[3] = arg4;
	CommandLineParams.Init(argc, argv);
	support_begin();
	x_BYTE_D4B80 = 0;
	CleanF5538_716A0();
	for (int i = 0; i < 504; i++)
		str_DWORD_F66F0x[i] = nullptr;
	x_DWORD_E9C28_str = nullptr;
	std::thread mainThread(sub_main_thread, argc, argv, envp);
	mainThread.join();
	support_end();
	if (locEndTestsCode == 20)
		printf("Test Level%d - OK\n", level);
	else
	{
		printf("Test Level%d - FAILED\n", level);
		return -1;
	}
	return 0;
}