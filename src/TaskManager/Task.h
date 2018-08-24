#pragma once

#include <core/common.h>

namespace RED_LILIUM_NAMESPACE
{

enum class TaskState : u8
{
	NotRegistered		= 1 << 0,
	Waiting				= 1 << 1,
	Running				= 1 << 2,
	Finished			= 1 << 3
};

class Task : public RedLiliumObject
{
public:
	Task(const std::string_view& name);
	~Task() override;

	virtual bool Run() = 0;

	void AddDependTask(const sptr<Task>& task);
	void AddReadResource(const ptr<RedLiliumObject>& resource);
	void AddWriteResource(const ptr<RedLiliumObject>& resource);

private:
	std::string m_name;
	std::atomic<TaskState> m_taskState;

	std::unordered_set<ptr<RedLiliumObject>> m_readResources;
	std::unordered_set<ptr<RedLiliumObject>> m_writeResources;
};


} // namespace RED_LILIUM_NAMESPACE
