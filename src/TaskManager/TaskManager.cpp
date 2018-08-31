#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskSheduler.h"

using namespace RED_LILIUM_NAMESPACE;

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
	throw std::exception();
}

bool TaskManager::RemoveTask(const sptr<Task>& task)
{
	throw std::exception();
}

bool TaskManager::RemoveTasks(const std::vector<sptr<Task>>& tasks)
{
	throw std::exception();
}
