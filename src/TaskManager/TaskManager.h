#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Task;
class TaskManagerThread;
class TaskScheduler;

class TaskManager : public RedLiliumObject
{
public:
	TaskManager();
	~TaskManager() override;

	void Start(const sptr<Task>& firstTask, std::optional<u32> threadsCount = std::nullopt);
	void Wait();

	static void SetThreadsCount(std::optional<u32> threadsCount);
	static void AddTask(const sptr<Task>& task);
	static void AddTask(sptr<Task>&& task);
	static bool RemoveTask(const sptr<Task>& task);
	static bool RemoveTasks(const std::vector<sptr<Task>>& tasks);

private:
	friend class TaskManagerThread;

	static thread_local std::vector<sptr<Task>> g_tasksPool;

	std::vector<uptr<TaskManagerThread>> m_threads;
	uptr<TaskScheduler> m_taskScheduler;
	std::mutex m_mutex;
};

} // namespace RED_LILIUM_NAMESPACE
