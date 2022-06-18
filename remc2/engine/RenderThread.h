#pragma once

#ifndef RENDER_THREAD
#define RENDER_THREAD

#include "../utilities/SafeQueue.h"
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#ifdef WIN32
#include <windows.h>
#elif _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif // win32

class RenderThread
{
private:

	int8_t m_core;
	bool m_running;
	std::thread m_renderThread;
	std::function<void()> m_task;
	std::mutex m_taskMutex;
	std::condition_variable m_nextTaskCondition;
	bool m_isTaskRunning;

std::mutex mtx;

public:

	RenderThread();
	RenderThread(uint8_t core);
	~RenderThread();

	void StartWorkerThread(int8_t core = -1);
	void StopWorkerThread();
	void Run(std::function<void()> task);
	bool IsRunning();
	bool GetIsTaskRunning();
};

#endif //RENDER_THREAD
