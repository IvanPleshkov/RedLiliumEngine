#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskSheduler.h"

using namespace RED_LILIUM_NAMESPACE;

Task::Task(const std::string& name)
{
}

Task::~Task()
{
}

bool Task::Run()
{
	return true;
}

void Task::AddDependTask(const sptr<Task>& task)
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::NotRegistered);
}

void Task::AddReadResource(const ptr<RedLiliumObject>& resource)
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::NotRegistered);
}

void Task::AddWriteResource(const ptr<RedLiliumObject>& resource)
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::NotRegistered);
}
