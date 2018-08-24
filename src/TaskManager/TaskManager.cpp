#include "pch.h"
#include "TaskManager.h"

using namespace RED_LILIUM_NAMESPACE;

class TaskGraph
{
public:
	void PushTask(
		const sptr<Task>& task,
		std::unordered_set<sptr<Task>>&& depends,
		std::unordered_set<ptr<RedLiliumObject>>&& readResources,
		std::unordered_set<ptr<RedLiliumObject>>&& writeResources);
	const sptr<Task>& PopTask();
	bool PopTask(const sptr<Task>& task);

private:
	std::mutex m_accessMutex;

	std::unordered_map<
		sptr<Task>,
		std::unordered_set<ptr<RedLiliumObject>>> m_tasksReadResources;
	std::unordered_map<
		sptr<Task>,
		std::unordered_set<ptr<RedLiliumObject>>> m_tasksWriteResources;
};

class TasksThread
{
public:
	TasksThread();



private:
	std::atomic<bool> m_isBusy;
	
};

static thread_local TasksThread s_taskThreads;
static thread_local std::vector<sptr<Task>> s_tasksGatherer;
static TaskGraph s_taskGraph;

TaskManager::TaskManager(u32 threadsCount)
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::SetThreadsCount(u32 threadsCount)
{
}

void TaskManager::AddTask(const sptr<Task>& task)
{
	RED_LILIUM_GUARD();

	s_tasksGatherer.push_back(task);
}

bool TaskManager::RemoveTask(const sptr<Task>& task)
{
	return false;
}
