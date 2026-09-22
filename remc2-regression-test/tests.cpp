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
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <cstdio>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#define REGTEST_POPEN _popen
#define REGTEST_PCLOSE _pclose
#else
#define REGTEST_POPEN popen
#define REGTEST_PCLOSE pclose
#endif
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
		if (!unitTestsProgress && !resave)//a test of its own: a bar instead of the lines for the runner
		{
			unitTestsProgress = true;
			unitTestsProgressFrames = test.steps;
		}
		if (run_regtest(test.level, test.type, test.index, test.save, test.record.c_str(), resave ? 1 : test.steps, test.intervalSave, test.folder.c_str()) != 0)
			numFailedTests++;
		if (unitTestsProgressFrames > 0)
		{
			printf("\n");
			unitTestsProgress = false;
			unitTestsProgressFrames = 0;
		}
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


// A test keeps its state in the engine globals, so every test needs its own process: the runner
// starts itself with the arguments of one test ("--level 5", "--record 1 --level 2", ...), reads
// what the child prints and draws the progress of the running ones.
struct type_running_test
{
	std::string arguments;
	std::string name;//as the test prints it
	int total = 0;//frames
	std::atomic<int> done{ 0 };
	std::atomic<bool> started{ false };
	std::atomic<bool> announced{ false };
	std::atomic<bool> printed{ false };
	std::atomic<bool> finished{ false };
	std::atomic<bool> failed{ false };
	std::chrono::steady_clock::time_point start;
	std::atomic<double> startup{ 0 };//seconds until the first frame: loading of the level
	std::atomic<double> duration{ 0 };
	std::string output;
};

std::string TestArguments(const type_regtest& test)
{
	if (!test.folder.empty())
		return "--record " + test.folder.substr(6) + " --level " + std::to_string(test.level);
	if (test.index > 0)
		return "--afterload " + std::to_string(test.index);
	return "--level " + std::to_string(test.level);
}

std::string TestName(const type_regtest& test)
{
	if (!test.folder.empty())
		return test.folder + " level " + std::to_string(test.level);
	if (test.index > 0)
		return "afterload " + std::to_string(test.index) + " level " + std::to_string(test.level);
	return "level " + std::to_string(test.level);
}

std::string TimeText(double seconds)
{
	if (seconds < 0 || seconds > 359999)
		seconds = 0;
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", (int)seconds / 3600, ((int)seconds / 60) % 60, (int)seconds % 60);
	return buffer;
}

double SecondsSince(std::chrono::steady_clock::time_point since)
{
	return std::chrono::duration<double>(std::chrono::steady_clock::now() - since).count();
}

// the child of one test: its output is kept, "@PROGRESS <step>" only moves its progress
void RunTestProcess(const std::string& exePath, type_running_test& test)
{
	test.start = std::chrono::steady_clock::now();
	test.started = true;
	const std::string command = "\"\"" + exePath + "\" " + test.arguments + " --progress 2>&1\"";
	FILE* pipe = REGTEST_POPEN(command.c_str(), "r");
	if (pipe == nullptr)
	{
		test.failed = true;
		test.finished = true;
		return;
	}
	char line[4096];
	while (fgets(line, sizeof(line), pipe) != nullptr)
	{
		if (strncmp(line, "@PROGRESS ", 10) == 0)
		{
			if (test.done == 0)
				test.startup = SecondsSince(test.start);
			test.done = atoi(line + 10);
		}
		else
			test.output += line;
	}
	const int exitCode = REGTEST_PCLOSE(pipe);
	test.failed = exitCode != 0 || test.output.find("- OK") == std::string::npos;
	test.duration = SecondsSince(test.start);
	test.done = test.total;
	test.finished = true;
}

void EnableAnsiColours()
{
#ifdef _WIN32
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	if (GetConsoleMode(console, &mode))
		SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

std::string ProgressBar(double part, int width)
{
	const int full = (int)(part * width + 0.5);
	return "[" + std::string(full, '=') + std::string(width - full > 0 ? width - full : 0, '.') + "]";
}

// time left of the whole run: the frames per second of one job and the start of a level, spread over the jobs
double SecondsLeft(const std::vector<std::unique_ptr<type_running_test>>& tests, int jobs)
{
	double jobSeconds = 0, startupSeconds = 0;
	int frames = 0, startups = 0;
	for (const auto& test : tests)
		if (test->started)
		{
			const double elapsed = test->finished ? test->duration.load() : SecondsSince(test->start);
			const double startup = test->startup.load();
			jobSeconds += elapsed - startup;
			frames += test->done;
			if (startup > 0)
			{
				startupSeconds += startup;
				startups++;
			}
		}
	if (frames <= 0 || jobSeconds <= 0)
		return 0;
	const double rate = frames / jobSeconds;//frames per second of one job
	const double startupTime = startups > 0 ? startupSeconds / startups : 0;

	std::vector<double> freeAt;
	for (const auto& test : tests)
		if (test->started && !test->finished)
		{
			const double elapsed = SecondsSince(test->start);
			const double left = (test->total - test->done) / rate + (test->done > 0 ? 0 : std::max(0.0, startupTime - elapsed));
			freeAt.push_back(left);
		}
	while ((int)freeAt.size() < jobs)
		freeAt.push_back(0);
	std::vector<int> pending;
	for (const auto& test : tests)
		if (!test->started)
			pending.push_back(test->total);
	std::sort(pending.begin(), pending.end(), std::greater<int>());
	for (const int total : pending)
	{
		auto first = std::min_element(freeAt.begin(), freeAt.end());
		*first += startupTime + total / rate;
	}
	return *std::max_element(freeAt.begin(), freeAt.end());
}

// the lines under the finished tests: one for every running test and one for all of them
std::vector<std::string> StatusLines(const std::vector<std::unique_ptr<type_running_test>>& tests,
	std::chrono::steady_clock::time_point start, int spin, int jobs)
{
	static const char spinner[] = "|/-\\";
	std::vector<std::string> lines;
	int framesDone = 0, finished = 0;
	std::string marks;
	for (const auto& test : tests)
	{
		framesDone += test->done;
		if (test->finished)
		{
			finished++;
			marks += test->failed ? 'X' : '#';
		}
		else
			marks += test->started ? spinner[spin % 4] : '.';
		if (test->started && !test->finished)
		{
			const double elapsed = SecondsSince(test->start);
			const int done = test->done;
			const double startup = test->startup.load();
			const double left = done > 0 ? (elapsed - startup) * (test->total - done) / done : 0;
			char buffer[256];
			snprintf(buffer, sizeof(buffer), "  %-22s %s %5d/%-5d frames, %5d left, %s elapsed, %s left",
				test->name.c_str(), ProgressBar(test->total > 0 ? (double)done / test->total : 0, 20).c_str(),
				done, test->total, test->total - done, TimeText(elapsed).c_str(), TimeText(left).c_str());
			lines.push_back(buffer);
		}
	}
	const double elapsed = SecondsSince(start);
	const double left = SecondsLeft(tests, jobs);
	char buffer[512];
	snprintf(buffer, sizeof(buffer), "%s %d/%d tests, %s elapsed, %s left",
		marks.c_str(), finished, (int)tests.size(), TimeText(elapsed).c_str(), TimeText(left).c_str());
	lines.push_back(buffer);
	return lines;
}

// tests in parallel; their outputs are printed in the order of the tests, as a single run prints them
int RunTestsInParallel(const std::vector<type_regtest>& list, int jobs, const std::string& exePath)
{
	EnableAnsiColours();
	std::vector<std::unique_ptr<type_running_test>> tests;
	for (const auto& test : list)
	{
		auto running = std::make_unique<type_running_test>();
		running->arguments = TestArguments(test);
		running->name = TestName(test);
		running->total = test.steps;
		tests.push_back(std::move(running));
	}

	std::vector<size_t> order(tests.size());//the longest tests first, they decide the whole time
	for (size_t i = 0; i < order.size(); i++)
		order[i] = i;
	std::stable_sort(order.begin(), order.end(), [&](size_t a, size_t b) { return tests[a]->total > tests[b]->total; });

	const auto start = std::chrono::steady_clock::now();
	std::atomic<size_t> next{ 0 };
	std::vector<std::thread> workers;
	for (int i = 0; i < jobs; i++)
		workers.emplace_back([&]()
			{
				for (size_t index = next++; index < order.size(); index = next++)
					RunTestProcess(exePath, *tests[order[index]]);
			});

	size_t printed = 0;
	int spin = 0, statusLines = 0;
	while (printed < tests.size())
	{
		for (int i = 0; i < statusLines; i++)
			printf("\033[1A\033[2K");
		statusLines = 0;
		for (const auto& test : tests)
			if (test->started && !test->announced.exchange(true))
				printf("  started: %s (%d frames)\n", test->name.c_str(), test->total);
		for (const auto& test : tests)//a finished test right away, the tests do not finish in their order
			if (test->finished && !test->printed.exchange(true))
			{
				printf("%s", test->output.c_str());
				printf("  %-22s %s %d frames in %s\n\n", test->name.c_str(), ProgressBar(1, 20).c_str(),
					test->total, TimeText(test->duration.load()).c_str());
				printed++;
			}
		const std::vector<std::string> lines = StatusLines(tests, start, spin++, jobs);
		for (const std::string& line : lines)
			printf("%s\n", line.c_str());
		statusLines = (int)lines.size();
		fflush(stdout);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	for (std::thread& worker : workers)
		worker.join();

	printf("--- summary ---\n");
	for (const auto& test : tests)
		printf("  %-22s %-6s %6d frames in %s\n", test->name.c_str(), test->failed ? "FAILED" : "OK",
			test->total, TimeText(test->duration.load()).c_str());

	int failed = 0;
	for (const auto& test : tests)
		if (test->failed)
			failed++;
	return failed;
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
	int jobs = (int)std::thread::hardware_concurrency() / 2;
	for (int a = 1; a < argc; a++)
	{
		if (std::string(argv[a]) == "--resave") resave = true;
		if (std::string(argv[a]) == "--progress") unitTestsProgress = true;
		if (std::string(argv[a]) == "--compare_from" && a + 1 < argc) unitTestsCompareFrom = atoi(argv[a + 1]);
		if (a + 1 >= argc) continue;
		if (std::string(argv[a]) == "--level") onlyLevel = atoi(argv[a + 1]);
		if (std::string(argv[a]) == "--afterload") onlyAfterload = atoi(argv[a + 1]);
		if (std::string(argv[a]) == "--record") onlyRecord = atoi(argv[a + 1]);
		if (std::string(argv[a]) == "--jobs") jobs = atoi(argv[a + 1]);
	}
	if (jobs < 1)
		jobs = 1;
	// without a selection: every test in its own process, "--jobs 1" one after another
	if (onlyLevel < 0 && onlyAfterload < 0 && onlyRecord < 0 && !resave && jobs > 1)
		numFailedTests += RunTestsInParallel(FindRegressionTests(-1, -1, -1), jobs, argv[0]);
	else
		numFailedTests += CountFailedRegressionTests(onlyLevel, onlyAfterload, onlyRecord, resave);

	if (numFailedTests == 0)
	{
		Logger->info("All tests passed");
	}
	else
	{
		Logger->error("{} tests failed!", numFailedTests);
	}	

	Logger->flush();
	return numFailedTests;
}