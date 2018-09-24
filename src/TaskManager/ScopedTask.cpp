#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskScheduler.h"
#include "ScopedTask.h"

using namespace RED_LILIUM_NAMESPACE;

sptr<Task> ScopedTask::Add(
	const std::string& name,
	std::initializer_list<ptr<const RedLiliumObject>> readResources,
	std::initializer_list<ptr<const RedLiliumObject>> writeResources,
	std::initializer_list<sptr<Task>> depends,
	LambdaType lambda)
{
	sptr<ScopedTask> task = smake<ScopedTask>(name, lambda);
	task->m_thisTask = task;
	return task;
}

ScopedTask::ScopedTask(const std::string& name, LambdaType lambda)
	: Task(name)
	, m_lambda(lambda)
{
}

bool ScopedTask::Run()
{
	return m_lambda(m_thisTask.lock());
}
