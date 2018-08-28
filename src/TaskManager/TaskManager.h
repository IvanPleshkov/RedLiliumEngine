#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Task;

class TaskManager : public RedLiliumObject
{
public:
	TaskManager(const sptr<Task>& firstTask, std::optional<u32> threadsCount = std::nullopt);
	~TaskManager() override;
	void Wait();

	static void SetThreadsCount(std::optional<u32> threadsCount);
	static void AddTask(const sptr<Task>& task);
	static bool RemoveTask(const sptr<Task>& task);
	static bool RemoveTasks(const std::vector<sptr<Task>>& tasks);
};

} // namespace RED_LILIUM_NAMESPACE
