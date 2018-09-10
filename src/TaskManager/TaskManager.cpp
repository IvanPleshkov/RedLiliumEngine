#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "TaskManagerThread.h"

using namespace RED_LILIUM_NAMESPACE;

thread_local std::vector<sptr<Task>> TaskManager::g_tasksPool;

#if RED_LILIUM_SINGLE_THREAD_MODE

TaskManager::TaskManager()
{
	m_taskScheduler = umake<TaskScheduler>();
}

TaskManager::~TaskManager()
{
}

void TaskManager::Start(const sptr<Task>& firstTask, std::optional<u32> threadsCount)
{
	sptr<Task> currentTask;
	m_taskScheduler->PushTasks({ firstTask });
	do
	{
		currentTask = m_taskScheduler->PopTask();

		if (currentTask)
		{
			m_taskScheduler->OnStartTask(currentTask);
			currentTask->Run();
			m_taskScheduler->OnFinishTask(currentTask);
			m_taskScheduler->PushTasks(g_tasksPool);
			g_tasksPool.clear();
		}
	} while (currentTask != nullptr);
}

void TaskManager::Wait()
{
}

void TaskManager::SetThreadsCount(std::optional<u32> threadsCount)
{
	if (threadsCount && threadsCount.value() != 1)
	{
		throw std::exception();
	}
}

void TaskManager::AddTask(const sptr<Task>& task)
{
	g_tasksPool.push_back(task);
}

void TaskManager::AddTask(sptr<Task>&& task)
{
	g_tasksPool.push_back(std::move(task));
}

bool TaskManager::RemoveTask(const sptr<Task>& task)
{
	return RemoveTasks({ task });
}

bool TaskManager::RemoveTasks(const std::vector<sptr<Task>>& tasks)
{
	throw std::exception();
	// return m_taskScheduler->RemoveTasks(tasks);
}

#endif // RED_LILIUM_SINGLE_THREAD_MODE
