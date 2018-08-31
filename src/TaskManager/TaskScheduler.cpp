#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskScheduler.h"

using namespace RED_LILIUM_NAMESPACE;

TaskScheduler::TaskScheduler()
{
}

TaskScheduler::~TaskScheduler()
{
}

void TaskScheduler::PushTasks(const std::vector<sptr<Task>>& tasks)
{
	for (const auto& task : tasks)
	{
		RED_LILIUM_ASSERT(task->m_taskState == TaskState::NotRegistered);
		task->m_taskState = TaskState::Waiting;
		m_tasks.push_back(task);
	}
}

bool TaskScheduler::RemoveTasks(const std::vector<sptr<Task>>& tasks)
{
	throw std::exception();
}

sptr<Task> TaskScheduler::PopTask()
{
	for (auto it = m_tasks.begin(); it != m_tasks.end(); it++)
	{
		if (CanPopTask(*it))
		{
			sptr<Task> task = *it;
			m_tasks.erase(it);
			return std::move(task);
		}
	}

	return nullptr;
}

bool TaskScheduler::CanPopTask(const sptr<Task>& task) const
{
	for (auto dependTask : task->m_dependTasks)
	{
		if (!dependTask.expired())
		{
			auto lockedDependTask = dependTask.lock();
			if (lockedDependTask->m_taskState != TaskState::Finished)
			{
				return false;
			}
		}
	}

	for (auto writeResource : task->m_writeResources)
	{
		if (m_writeResources.count(writeResource) || m_readResources.count(writeResource))
		{
			return false;
		}
	}

	for (auto readResource : task->m_readResources)
	{
		if (m_writeResources.count(readResource))
		{
			return false;
		}
	}

	return true;
}

void TaskScheduler::OnStartTask(const sptr<Task>& task)
{
	RED_LILIUM_ASSERT(task->m_taskState == TaskState::Waiting);

	for (auto writeResource : task->m_writeResources)
	{
		RED_LILIUM_ASSERT(m_writeResources.count(writeResource) == 0);
		RED_LILIUM_ASSERT(m_readResources.count(writeResource) == 0);
		m_writeResources.insert(writeResource);
	}

	for (auto readResource : task->m_readResources)
	{
		RED_LILIUM_ASSERT(m_writeResources.count(readResource) == 0);
		m_readResources.insert(readResource);
	}

	task->m_taskState = TaskState::Running;
}

void TaskScheduler::OnFinishTask(const sptr<Task>& task)
{
	RED_LILIUM_ASSERT(task->m_taskState == TaskState::Running);

	for (auto writeResource : task->m_writeResources)
	{
		RED_LILIUM_ASSERT(m_writeResources.count(writeResource) > 0);
		m_writeResources.erase(writeResource);
	}

	for (auto readResource : task->m_readResources)
	{
		auto it = m_readResources.find(readResource);
		RED_LILIUM_ASSERT(it != m_readResources.end());
		m_readResources.erase(it);
	}

	task->m_taskState = TaskState::Finished;
}
