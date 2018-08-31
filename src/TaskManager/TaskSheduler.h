#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Task;
class TaskManagerThread;

class TaskSheduler : public RedLiliumObject
{
public:
	TaskSheduler();
	~TaskSheduler() override;

	void PushTasks(const std::vector<sptr<Task>>& tasks);
	bool RemoveTask(const sptr<Task>& tasks);
	bool RemoveTasks(const std::vector<sptr<Task>>& tasks);

	const sptr<Task>& PopTask(const ptr<TaskManagerThread>& thread);
	void OnStartTask(const sptr<Task>& task, const ptr<TaskManagerThread>& thread);
	void OnFinishTask(const sptr<Task>& task, const ptr<TaskManagerThread>& thread);

};

} // namespace RED_LILIUM_NAMESPACE
