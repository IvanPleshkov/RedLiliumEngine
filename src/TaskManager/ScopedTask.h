#pragma once

#include "TaskManagerForward.h"
#include "Task.h"

namespace RED_LILIUM_NAMESPACE
{

class ScopedTask : public Task
{
public:
	using LambdaType = std::function<bool(const sptr<ScopedTask>& task)>;

	static sptr<Task> Add(
		const std::string& name,
		std::initializer_list<ptr<const RedLiliumObject>> readResources,
		std::initializer_list<ptr<const RedLiliumObject>> writeResources,
		std::initializer_list<sptr<Task>> depends,
		LambdaType lambda);

public:
	~ScopedTask() override {}

	bool Run() override;

private:
	 ScopedTask(const std::string& name, LambdaType lambda);

private:
	LambdaType m_lambda;
	wptr<ScopedTask> m_thisTask;
};

} // namespace RED_LILIUM_NAMESPACE
