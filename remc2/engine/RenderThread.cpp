#include "RenderThread.h"

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

void RenderThread::StartWorkerThread()
{
	m_renderThread = std::thread([this] {
		do
		{
			if (m_task)
			{
				m_task();
				m_task = 0;
				m_isTaskRunning = false;
			}

		} while (m_running);
		});

	m_running = true;
}

void RenderThread::StartWorkerThread(uint8_t core)
{
	unsigned numCores = 1;

	numCores = std::thread::hardware_concurrency();

	if (numCores >= core)
	{
		m_core = core;
		m_renderThread = std::thread([this, core] {

#ifdef _MSC_VER
			SetThreadIdealProcessor(GetCurrentThread(), core);
			DWORD_PTR dw = SetThreadAffinityMask(GetCurrentThread(), DWORD_PTR(1) << core);
#endif
			do
			{
				if (m_task)
				{
					m_task();
					m_task = 0;
					m_isTaskRunning = false;
				}

			} while (m_running);
		});

		m_running = true;
	}
}

void RenderThread::StopWorkerThread()
{
	if (m_running)
	{
		m_running = false;
		if (m_renderThread.joinable()) {
			m_renderThread.join();
		}
	}
}

void RenderThread::Run(std::function<void()> task)
{
	m_isTaskRunning = true;
	m_task = task;
}

bool RenderThread::IsRunning()
{
	return m_running;
}

bool RenderThread::GetIsTaskRunning()
{
	return m_isTaskRunning;
}