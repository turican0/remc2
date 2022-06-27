#include "RenderThread.h"
#include <mutex>

RenderThread::RenderThread()
{
	m_core = 0;
	m_running = false;
	m_isTaskRunning = false;
	m_task = 0;
	StartWorkerThread();
}

RenderThread::RenderThread(uint8_t core)
{
	m_core = 0;
	m_running = false;
	m_isTaskRunning = false;
	m_task = 0;
	StartWorkerThread(core);
}

RenderThread::~RenderThread()
{
	StopWorkerThread();
}

void RenderThread::StartWorkerThread(int8_t core)
{
	int8_t numCores = std::thread::hardware_concurrency();

	if (numCores < core)
		return;

	m_core = core;
	m_renderThread = std::thread([this, core] {

#ifdef _MSC_VER
		if (core >= 0) {
			SetThreadIdealProcessor(GetCurrentThread(), core);
			DWORD_PTR dw = SetThreadAffinityMask(GetCurrentThread(), DWORD_PTR(1) << ((uint8_t)core));
		}
#endif

#ifndef _MSC_VER
		std::unique_lock<std::mutex> lock(m_taskMutex, std::defer_lock);
#endif
		do
		{

#ifndef _MSC_VER
			lock.lock();
			// only wake up if we need to render or if the thread is stopped
			m_nextTaskCondition.wait(lock, [this]{ return m_isTaskRunning || !m_running; });
#endif
			if (m_task) {
				m_task();
				m_task = 0;
				m_isTaskRunning = false;
			}
#ifndef _MSC_VER
			lock.unlock();
#endif
		} while (m_running);
	});

	m_running = true;
}

void RenderThread::StopWorkerThread()
{
	if (m_running)
	{
		m_running = false;
		m_nextTaskCondition.notify_all();
		if (m_renderThread.joinable()) {
			m_renderThread.join();
		}
	}
}

void RenderThread::Run(std::function<void()> task)
{
	std::lock_guard<std::mutex> guard(m_taskMutex);
	m_isTaskRunning = true;
	m_task = task;
	m_nextTaskCondition.notify_all();
}

bool RenderThread::IsRunning()
{
	return m_running;
}

bool RenderThread::GetIsTaskRunning()
{
	return m_isTaskRunning;
}