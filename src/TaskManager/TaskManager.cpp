#include "pch.h"
#include "TaskManager.h"
#include "Task.h"

using namespace RED_LILIUM_NAMESPACE;

class TaskGraph
{
public:
	void PushTasks(const std::vector<sptr<Task>>& tasks);
	const sptr<Task>& PopTask();
	bool RemoveTasks(const std::vector<sptr<Task>>& tasks);

	
};

void TaskGraph::PushTasks(const std::vector<sptr<Task>>& tasks)
{
	RED_LILIUM_GUARD();
}

const sptr<Task>& TaskGraph::PopTask()
{
	RED_LILIUM_GUARD();
	return nullptr;
}

bool TaskGraph::RemoveTasks(const std::vector<sptr<Task>>& tasks)
{
	RED_LILIUM_GUARD();

	throw std::exception();
}


class TasksThread : public RedLiliumObject
{
public:
	TasksThread();
	~TasksThread() override;

	void RunThread(const sptr<Task>& startTask);

private:
	std::atomic<bool> m_isBusy;
	
};

static thread_local TasksThread s_taskThreads;
static thread_local std::vector<sptr<Task>> s_tasksGatherer;
static TaskGraph s_taskGraph;
static std::mutex s_taskGraphAccessMutex;


TasksThread::TasksThread()
{}

TasksThread::~TasksThread()
{
}

void TasksThread::RunThread(const sptr<Task>& startTask)
{
	m_isBusy = true;

	sptr<Task> currentTask = startTask;

	{
		std::lock_guard<std::mutex> lock(s_taskGraphAccessMutex);
	}

	while (currentTask)
	{
		{
			RED_LILIUM_SCOPE();
			currentTask->Run();
		}

		{
			RED_LILIUM_SCOPE();

			std::lock_guard<std::mutex> lock(s_taskGraphAccessMutex);

			s_taskGraph.PushTasks(s_tasksGatherer);
			s_tasksGatherer.clear();

			currentTask = s_taskGraph.PopTask();
		}
	}

	m_isBusy = false;
}

TaskManager::TaskManager(const sptr<Task>& firstTask, std::optional<u32> threadsCount)
{
	s_taskGraph.PushTasks({ firstTask });
}

TaskManager::~TaskManager()
{
}

void TaskManager::Wait()
{
}

void TaskManager::SetThreadsCount(std::optional<u32> threadsCount)
{
}

void TaskManager::AddTask(const sptr<Task>& task)
{
	RED_LILIUM_GUARD();

	s_tasksGatherer.push_back(task);
}

bool TaskManager::RemoveTask(const sptr<Task>& task)
{
	return RemoveTasks({ task });
}

bool TaskManager::RemoveTasks(const std::vector<sptr<Task>>& tasks)
{
	return s_taskGraph.RemoveTasks(tasks);
}
