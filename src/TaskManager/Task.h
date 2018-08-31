#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

enum class TaskState : u8
{
	NotRegistered		= 1 << 0,
	Waiting				= 1 << 1,
	Running				= 1 << 2,
	Finished			= 1 << 3,
	Canceled			= 1 << 4
};

class Task : public RedLiliumObject
{
public:
    Task(const std::string& name);
	~Task() override;

	virtual bool Run() = 0;

	void AddDependTask(const sptr<Task>& task);
	void AddReadResource(ptr<RedLiliumObject> resource);
	void AddWriteResource(ptr<RedLiliumObject> resource);

private:
	friend class TaskScheduler;

	std::string m_name;
	std::atomic<TaskState> m_taskState;

	std::vector<wptr<Task>> m_dependTasks;
	std::vector<ptr<RedLiliumObject>> m_readResources;
	std::vector<ptr<RedLiliumObject>> m_writeResources;
};

} // namespace RED_LILIUM_NAMESPACE
