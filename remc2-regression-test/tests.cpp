// Unit-tests.cpp : This file contains the main function. This is where the program execution starts and ends.
//
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "regression-tests.h"
//TEMPDIAG begin
#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#include <crtdbg.h>
#pragma comment(lib, "dbghelp.lib")
int TempAssertHook(int, char* message, int*)
{
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, nullptr, TRUE);
	void* frames[64];
	const USHORT count = CaptureStackBackTrace(0, 64, frames, nullptr);
	std::cout << "TEMPDIAG assert: " << message << std::endl;
	for (USHORT i = 0; i < count; i++)
	{
		char buffer[sizeof(SYMBOL_INFO) + 256] = {};
		SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = 255;
		IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) };
		DWORD displacement = 0;
		const bool hasSymbol = SymFromAddr(process, (DWORD64)frames[i], nullptr, symbol);
		const bool hasLine = SymGetLineFromAddr64(process, (DWORD64)frames[i], &displacement, &line);
		std::cout << "TEMPDIAG " << (hasSymbol ? symbol->Name : "?") << " " << (hasLine ? line.FileName : "") << ":" << (hasLine ? line.LineNumber : 0) << std::endl;
	}
	std::cout.flush();
	ExitProcess(3);
	return TRUE;
}
#define TEMPDIAG_INSTALL_HOOK() _CrtSetReportHook2(_CRT_RPTHOOK_INSTALL, TempAssertHook)
#else
// Linux/glibc has no CRT report-hook mechanism: assert() just prints to
// stderr and calls abort(), which raises SIGABRT. We hook that instead and
// print a backtrace with glibc's <execinfo.h>, which is the closest
// equivalent to the Windows dbghelp-based stack walk above.
#include <execinfo.h>
#include <csignal>
#include <cstdlib>
void TempAssertHook(int)
{
	void* frames[64];
	const int count = backtrace(frames, 64);
	std::cout << "TEMPDIAG assert: SIGABRT" << std::endl;
	std::cout.flush();
	backtrace_symbols_fd(frames, count, STDOUT_FILENO);
	std::cout.flush();
	_exit(3);
}
#define TEMPDIAG_INSTALL_HOOK() std::signal(SIGABRT, TempAssertHook)
#endif
//TEMPDIAG end


enum TestType {
	BeginLevelNoActions = 0,
	AfterloadNoActions = 1,
	BeginLevelWithActions = 2,
	AfterloadWithActions = 3
};

struct type_regtest
{
	std::string name;
	int level = 0;
	int type = BeginLevelNoActions;
	int index = 0;
	int save = 0;
	std::string record;//in memimages/regressions, or in folder
	int steps = 20;
	bool intervalSave = false;
	std::string folder;//record<N>: <folder>/level<L>/sequence-*
};

// N of <prefix>N, -1 for other names
int FolderNumber(const std::string& name, const std::string& prefix)
{
	if (name.rfind(prefix, 0) != 0 || name.size() == prefix.size() || name.find_first_not_of("0123456789", prefix.size()) != std::string::npos)
		return -1;
	return std::stoi(name.substr(prefix.size()));
}

// frames of sequence-002285FF-00356038 (.bin or .binz)
int SequenceFrames(const std::string& folder)
{
	const std::string name = folder + "/sequence-002285FF-00356038";
	if (std::filesystem::exists(name + ".bin"))
		return (int)(std::filesystem::file_size(name + ".bin") / 224790);
	std::ifstream file(name + ".binz", std::ios::binary);
	file.seekg(12);
	int frames = 0;
	uint32_t length = 0;
	while (file.read((char*)&length, 4) && file.seekg(length, std::ios::cur))
		frames++;
	return frames;
}

// every test of memimages/regressions: level<N>, afterloadtest<N> (its regtest.txt), record<N>/level<L>
std::vector<type_regtest> FindRegressionTests(int onlyLevel, int onlyAfterload, int onlyRecord)
{
	const bool all = (onlyLevel < 0 && onlyAfterload < 0 && onlyRecord < 0);
	std::vector<std::pair<std::tuple<int, int, int>, type_regtest>> tests;
	for (const auto& entry : std::filesystem::directory_iterator(RegressionsPath()))
	{
		const std::string name = entry.path().filename().string();
		type_regtest test;
		test.name = name;
		if ((test.level = FolderNumber(name, "level")) > 0 && (all || (onlyRecord < 0 && onlyLevel == test.level)))
			tests.push_back({ { 0, test.level, 0 }, test });
		else if ((test.index = FolderNumber(name, "afterloadtest")) > 0 && (all || onlyAfterload == test.index))
		{
			std::ifstream file(entry.path() / "regtest.txt");
			for (std::string token; file >> token;)
			{
				const std::string key = token.substr(0, token.find('='));
				const std::string value = token.substr(token.find('=') + 1);
				if (key == "level") test.level = std::stoi(value);
				if (key == "type") test.type = std::stoi(value);
				if (key == "save") test.save = std::stoi(value);
				if (key == "record") test.record = value;
				if (key == "steps") test.steps = std::stoi(value);
				if (key == "intervalsave") test.intervalSave = value == "1";
			}
			tests.push_back({ { 1, test.index, 0 }, test });
		}
		else if (const int record = FolderNumber(name, "record"); record > 0 && (all || onlyRecord == record))
		{
			test.type = BeginLevelWithActions;
			test.folder = name;
			for (const auto& file : std::filesystem::directory_iterator(entry.path()))
				if (file.is_regular_file())
					test.record = file.path().filename().string();
			for (const auto& levelFolder : std::filesystem::directory_iterator(entry.path()))
			{
				test.level = FolderNumber(levelFolder.path().filename().string(), "level");
				if (test.level <= 0 || (onlyLevel > 0 && onlyRecord > 0 && onlyLevel != test.level))
					continue;
				test.steps = SequenceFrames(levelFolder.path().string());
				tests.push_back({ { 2, record, test.level }, test });
			}
		}
	}
	std::sort(tests.begin(), tests.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
	std::vector<type_regtest> result;
	for (const auto& test : tests)
		result.push_back(test.second);
	return result;
}

extern bool resaveRecordings;

// onlyLevel / onlyAfterload / onlyRecord select tests (-1 = no selection).  Running one test per
// process keeps the engine's globals from carrying over between tests - run_regtest resets only
// a handful of them.  resave: 1 step of the tests with a recording, which gets the level start saves.
int CountFailedRegressionTests(int onlyLevel = -1, int onlyAfterload = -1, int onlyRecord = -1, bool resave = false) {
	int numFailedTests = 0;
	for (const auto& test : FindRegressionTests(onlyLevel, onlyAfterload, onlyRecord))
	{
		if (resave && test.record.empty())
			continue;
		resaveRecordings = resave;
		if (run_regtest(test.level, test.type, test.index, test.save, test.record.c_str(), resave ? 1 : test.steps, test.intervalSave, test.folder.c_str()) != 0)
			numFailedTests++;
		resaveRecordings = false;
	}

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
	TEMPDIAG_INSTALL_HOOK();//TEMPDIAG
	// "--level N" or "--afterload N" runs just that test, "--record N [--level L]" the levels of recording N,
	// "--resave" with them rewrites the level start saves of their recordings
	int onlyLevel = -1;
	int onlyAfterload = -1;
	int onlyRecord = -1;
	bool resave = false;
	for (int a = 1; a < argc; a++)
	{
		if (std::string(argv[a]) == "--resave") resave = true;
		if (a + 1 >= argc) continue;
		if (std::string(argv[a]) == "--level") onlyLevel = atoi(argv[a + 1]);
		if (std::string(argv[a]) == "--afterload") onlyAfterload = atoi(argv[a + 1]);
		if (std::string(argv[a]) == "--record") onlyRecord = atoi(argv[a + 1]);
	}
	numFailedTests += CountFailedRegressionTests(onlyLevel, onlyAfterload, onlyRecord, resave);

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