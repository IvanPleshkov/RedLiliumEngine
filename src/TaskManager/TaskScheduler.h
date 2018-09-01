#pragma once

#include "TaskManagerForward.h"

namespace RED_LILIUM_NAMESPACE
{

class TaskScheduler : public RedLiliumObject
{
public:
	TaskScheduler();
	~TaskScheduler() override;

	void PushTasks(const std::vector<sptr<Task>>& tasks);
	sptr<Task> PopTask();
	bool RemoveTasks(const std::vector<sptr<Task>>& tasks);

	void OnStartTask(const sptr<Task>& task);
	void OnFinishTask(const sptr<Task>& task);

private:
	bool CanPopTask(const sptr<Task>& task) const;

	std::unordered_set<ptr<RedLiliumObject>> m_writeResources;
	std::multiset<ptr<RedLiliumObject>> m_readResources;
	std::list<sptr<Task>> m_tasks;	
};

} // namespace RED_LILIUM_NAMESPACE
