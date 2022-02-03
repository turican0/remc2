#include "RenderThread.h"

RenderThread::RenderThread(uint8_t core)
{
	m_core = 0;
	m_running = false;
	m_runningTasks = 0;
	StartWorkerThread(core);
}

RenderThread::~RenderThread()
{
	StopWorkerThread();
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
				std::function<void()> task = m_taskQueue.dequeue();
				if (task)
				{
					task();
					m_runningTasks--;
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

void RenderThread::Enqueue(std::function<void()> task)
{
	m_runningTasks++;
	m_taskQueue.enqueue(task);
}

bool RenderThread::IsRunning()
{
	return m_running;
}

int RenderThread::GetNumberRunningTasks()
{
	return m_runningTasks;
}