#pragma once

#include "Task.h"

namespace RED_LILIUM_NAMESPACE
{

template<class... T>
class TaskClosureImpl : private std::tuple<ptr<T>...>
{
public:
	TaskClosureImpl() : std::tuple<ptr<T>...>(ptr<T>(nullptr)...)
	{}

	template<class I>
	ptr<I>& Get()
	{
		return std::get<ptr<I>>(*static_cast<std::tuple<ptr<T>...>*>(this));
	}
};

template<class... T>
class TaskReadClosure : public TaskClosureImpl<const T...>
{
public:
    TaskReadClosure() : TaskClosureImpl<const T...>()
	{}
};

template<class... T>
class TaskWriteClosure : public TaskClosureImpl<T...>
{
public:
    TaskWriteClosure() : TaskClosureImpl<T...>()
	{}
};

template<class ReadClosure = TaskReadClosure<>, class WriteClosure = TaskWriteClosure<>>
class CapturedTask : public Task
{
public:
	using Read = ReadClosure;
	using Write = WriteClosure;
	using CapturedTaskType = CapturedTask<ReadClosure, WriteClosure>;

    CapturedTask(const std::string& name)
		: Task(name)
		, m_readClosure()
		, m_writeClosure()
	{ }

	~CapturedTask() override {}

	template <class T>
	ptr<const T>& GetRead()
	{
		return m_readClosure.Get<const T>();
	}

	template <class T>
	ptr<T>& GetWrite()
	{
		return m_writeClosure.Get<T>();
	}

	template <class T>
	void SetRead(ptr<const T> resource)
	{
		RED_LILIUM_ASSERT(GetRead<T>() == nullptr);
		m_readClosure.Get<const T>() = resource;
		AddReadResource(resource);
	}

	template <class T>
	void SetRead(const uptr<T>& resource)
	{
		RED_LILIUM_ASSERT(GetRead<T>() == nullptr);
		m_readClosure.Get<const T>() = resource.get();
		AddReadResource(resource.get());
	}

	template <class T>
	void SetWrite(ptr<T> resource)
	{
		RED_LILIUM_ASSERT(GetWrite<T>() == nullptr);
		m_writeClosure.Get<T>() = resource;
		AddWriteResource(resource);
	}

	template <class T>
	void SetWrite(const uptr<T>& resource)
	{
		RED_LILIUM_ASSERT(GetWrite<T>() == nullptr);
		m_writeClosure.Get<T>() = resource.get();
		AddWriteResource(resource.get());
	}


private:
	ReadClosure m_readClosure;
	WriteClosure m_writeClosure;
};

} // namespace RED_LILIUM_NAMESPACE
