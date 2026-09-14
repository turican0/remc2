// Unit-tests.cpp : This file contains the main function. This is where the program execution starts and ends.
//
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include "regression-tests.h"


// onlyLevel / onlyAfterload select a single test (-1 = no selection).  Running one test per
// process keeps the engine's globals from carrying over between tests - run_regtest resets only
// a handful of them.
int CountFailedRegressionTests(int onlyLevel = -1, int onlyAfterload = -1) {
	int numFailedTests = 0;
	//run_regtest(level,testType,indexOfRegression,indexOfSavePosition(-1 - no load),isRecorded)
	enum TestType {
		BeginLevelNoActions = 0,
		AfterloadNoActions = 1,
		BeginLevelWithActions = 2,
		AfterloadWithActions = 3
	};
	Logger->info("\n--- Level regressions tests ---");
	const bool all = (onlyLevel < 0 && onlyAfterload < 0);
	for (int i = 1; i <= 25; i++)
		if (i != 22 && i != 25 && (all || onlyLevel == i))
			if (run_regtest(i) != 0)
			{
				numFailedTests++;
			}
	Logger->info("--- Afterload regressions tests ---");

	if ((all || onlyAfterload == 1) && run_regtest(2, TestType::AfterloadNoActions, 1, 2) != 0) numFailedTests++;
	if ((all || onlyAfterload == 2) && run_regtest(2, TestType::BeginLevelWithActions, 2, 1, "Levels-1-5-Recording.bin", 25) != 0) numFailedTests++;
	// Level 5 played to the end of the recording (5402 turns) against the original game replaying the same
	// recording in DOSBox (dosbox-x-remc2, mc2replay/run_replay.ps1 -Seq): memimages afterloadtest9
	if ((all || onlyAfterload == 9) && run_regtest(5, TestType::BeginLevelWithActions, 9, 0, "Level5-mine.dem", 5420) != 0) numFailedTests++;
	//if (run_regtest(1, TestType::BeginLevelWithActions, 3, -1, "Levels-1-5-Recording.bin", 3000) != 0) numFailedTests++;
	//if (run_regtest(1, true, 2, -1, "c:/prenos/remc2-dev2/remc2/x64/Debug/memimages/regressions/afterloadtest2/Levels-1-5-Recording.bin",25) != 0) numFailedTests++;


	// diff in level 22:
	//   the first frame with a diff has it at 0x7dba and the following bytes:
	//      buffer[32186] = 53   adress[32186] = 150
	//      buffer[32187] = 120   adress[32187] = 189
	//      buffer[32194] = 18   adress[32194] = 0
	//      buffer[32195] = 4   adress[32195] = 0
	//   in type_entity_0x6E8E struct_0x6E8E[1000];
	//   size of struct_0x6E8E[1000] is 0x29040 = 168000
	//   168 byte per element
	//   -> diff in the 23rd element struct_0x6E8E[22] at position 140
	//       -> maxMana_0x8C_140 and playerEntityIndex_0x94_148

	// diff in level 25
	// byte_counter_current_objective_box_0x36E04 = 0 instead of 200

	return numFailedTests;
}

int main(int argc, char** argv)
{
	//if (CommandLineParams.DoShowDebugMessages1()) -- for suppress messages
	int numFailedTests = 0;

	InitializeLogging(spdlog::level::info);
	// "--level N" or "--afterload N" runs just that test
	int onlyLevel = -1;
	int onlyAfterload = -1;
	for (int a = 1; a + 1 < argc; a++)
	{
		if (std::string(argv[a]) == "--level") onlyLevel = atoi(argv[a + 1]);
		if (std::string(argv[a]) == "--afterload") onlyAfterload = atoi(argv[a + 1]);
	}
	numFailedTests += CountFailedRegressionTests(onlyLevel, onlyAfterload);

	if (numFailedTests == 0)
	{
		Logger->info("All tests passed");
	}
	else
	{
		Logger->error("{} tests failed!", numFailedTests);
	}	

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	return numFailedTests;
}
