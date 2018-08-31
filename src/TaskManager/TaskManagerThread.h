#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Task;

class TaskManagerThread : public RedLiliumObject
{
public:
	TaskManagerThread();
	~TaskManagerThread() override;

	void RunThread(const sptr<Task>& startTask);
};

} // namespace RED_LILIUM_NAMESPACE
