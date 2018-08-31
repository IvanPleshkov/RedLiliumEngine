#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskScheduler.h"

using namespace RED_LILIUM_NAMESPACE;

TaskManagerThread::TaskManagerThread(ptr<TaskManager> taskManager)
	: m_taskManager(taskManager)
{
}

TaskManagerThread::~TaskManagerThread()
{
}

void TaskManagerThread::Start(const sptr<Task>& startTask)
{
	m_currentTask = startTask;
	m_state = State::Running;
	// todo: free barier
}

void TaskManagerThread::StartLooping()
{
	m_state = State::Running;

	while (m_state != State::Stopped)
	{
		if (m_currentTask == nullptr)
		{
			// todo: barrier
		}

		StartLoopIteration();

		m_state = State::Waiting;
	}
}

void TaskManagerThread::StartLoopIteration()
{
	const ptr<TaskScheduler> scheduler = m_taskManager->m_taskScheduler.get();

	{ // lock_guard scope
		std::lock_guard<std::mutex> lock(m_taskManager->m_mutex);

		if (m_currentTask)
		{
			scheduler->OnStartTask(m_currentTask);
		}
	}

	while (m_currentTask)
	{
		m_currentTask->Run();

		{ // lock_guard scope
			std::lock_guard<std::mutex> lock(m_taskManager->m_mutex);

			scheduler->OnFinishTask(m_currentTask);
			scheduler->PushTasks(m_taskManager->g_tasksPool);
			m_currentTask = scheduler->PopTask();
			if (m_currentTask)
			{
				scheduler->OnStartTask(m_currentTask);
			}
		}

		m_taskManager->g_tasksPool.clear();
	}
}
