#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskScheduler.h"

using namespace RED_LILIUM_NAMESPACE;

Task::Task(const std::string& name)
	: m_taskState(TaskState::NotRegistered)
{
}

Task::~Task()
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::Finished || m_taskState == TaskState::Canceled);
}

void Task::AddDependTask(const sptr<Task>& task)
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::NotRegistered);

	m_dependTasks.push_back(task);
}

void Task::AddReadResource(ptr<RedLiliumObject> resource)
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::NotRegistered);

	m_readResources.push_back(resource);
}

void Task::AddWriteResource(ptr<RedLiliumObject> resource)
{
	RED_LILIUM_ASSERT(m_taskState == TaskState::NotRegistered);

	m_writeResources.push_back(resource);
}
