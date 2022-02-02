#pragma once

#ifndef RENDER_THREAD
#define RENDER_THREAD

#include "../utilities/SafeQueue.h"
#include <functional>
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

	uint8_t m_core;
	std::atomic_bool m_running;
	std::thread m_renderThread;
	SafeQueue<std::function<void()>> m_taskQueue;

public:

	RenderThread(uint8_t core);
	~RenderThread();

	void StartWorkerThread(uint8_t core);
	void StopWorkerThread();
	void Enqueue(std::function<void()> task);
	bool IsRunning();
};

#endif //RENDER_THREAD
