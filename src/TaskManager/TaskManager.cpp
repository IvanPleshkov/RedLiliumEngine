#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskScheduler.h"

using namespace RED_LILIUM_NAMESPACE;

thread_local std::vector<sptr<Task>> TaskManager::g_tasksPool;

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::Start(const sptr<Task>& firstTask, std::optional<u32> threadsCount)
{
	throw std::exception();
}

void TaskManager::Wait()
{
	throw std::exception();
}

void TaskManager::SetThreadsCount(std::optional<u32> threadsCount)
{
	throw std::exception();
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
}
