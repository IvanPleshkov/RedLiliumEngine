#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskSheduler.h"

using namespace RED_LILIUM_NAMESPACE;

TaskSheduler::TaskSheduler()
{
}

TaskSheduler::~TaskSheduler()
{
}

void TaskSheduler::PushTasks(const std::vector<sptr<Task>>& tasks)
{
	throw std::exception();
}

bool TaskSheduler::RemoveTask(const sptr<Task>& tasks)
{
	throw std::exception();
}

bool TaskSheduler::RemoveTasks(const std::vector<sptr<Task>>& tasks)
{
	throw std::exception();
}

const sptr<Task>& TaskSheduler::PopTask(ptr<TaskManagerThread> thread)
{
	throw std::exception();
}

void TaskSheduler::OnStartTask(const sptr<Task>& task, ptr<TaskManagerThread> thread)
{
	throw std::exception();
}

void TaskSheduler::OnFinishTask(const sptr<Task>& task, ptr<TaskManagerThread> thread)
{
	throw std::exception();
}
