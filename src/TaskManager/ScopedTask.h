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
	ScopedTask(const std::string& name, LambdaType lambda);
	~ScopedTask() override {}

	bool Run() override;

	template<class T>
	ptr<const T> GetRead() const
	{
		for (auto resource : m_readResources)
		{
			ptr<const T> castedResource = Cast<const T>(resource);
			if (castedResource != nullptr)
			{
				return castedResource;
			}
		}
		return nullptr;
	}

	template<class T>
	ptr<T> GetWrite() const
	{
		for (auto resource : m_writeResources)
		{
			ptr<const T> castedResource = Cast<const T>(resource);
			if (castedResource != nullptr)
			{
				return ConstCast<T>(castedResource);
			}
		}
		return nullptr;
	}

private:
	LambdaType m_lambda;
	wptr<ScopedTask> m_thisTask;
};

} // namespace RED_LILIUM_NAMESPACE
