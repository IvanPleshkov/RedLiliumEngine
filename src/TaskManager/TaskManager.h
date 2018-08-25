#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Task;

class TaskManager : public RedLiliumObject
{
public:
	TaskManager(u32 threadsCount = 0);
	~TaskManager() override;

	static void SetThreadsCount(u32 threadsCount);
	static void AddTask(const sptr<Task>& task);
	static bool RemoveTask(const sptr<Task>& task);
};

} // namespace RED_LILIUM_NAMESPACE
