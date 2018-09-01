#pragma once

#if !RED_LILIUM_SINGLE_THREAD_MODE

#include "TaskManagerForward.h"

namespace RED_LILIUM_NAMESPACE
{

class TaskManagerThread : public RedLiliumObject
{
public:
	TaskManagerThread(ptr<TaskManager> taskManager);
	~TaskManagerThread() override;

	void Start(const sptr<Task>& startTask);

private:
	enum class State : u8
	{
		Waiting		= 1 << 0,
		Running		= 1 << 1,
		Stopped		= 1 << 2
	};

	void StartLooping();
	void StartLoopIteration();

	const ptr<TaskManager> m_taskManager;
	sptr<Task> m_currentTask;
	std::atomic<State> m_state;
};

} // namespace RED_LILIUM_NAMESPACE

#endif
