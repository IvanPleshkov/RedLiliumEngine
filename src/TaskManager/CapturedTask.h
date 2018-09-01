#pragma once

#include "Task.h"

namespace RED_LILIUM_NAMESPACE
{

template<class... T>
class TaskClosureImpl : private std::tuple<T...>
{
public:
	TaskClosureImpl(const T&... args);

	template<class I>
	const I& Get();
};

template<class... T>
class TaskReadClosure : public TaskClosureImpl<ptr<const T>...>
{};

template<class... T>
class TaskWriteClosure : public TaskClosureImpl<ptr<T>...>
{};

template<class ReadClosure = TaskReadClosure<>, class WriteClosure = TaskWriteClosure<>>
class CapturedTask : public Task
{
public:
    CapturedTask(const std::string& name, const ReadClosure& readClosure, const WriteClosure& writeClosure);
	~CapturedTask() override;

	template <class T>
	ptr<const T>& GetRead();

	template <class T>
	ptr<T>& GetWrite();

private:
	ReadClosure m_readClosure;
	WriteClosure m_writeClosure;
};

//====================== TaskClosureImpl
template<class ...T>
inline TaskClosureImpl<T...>::TaskClosureImpl(const T& ...args)
{
}

template<class I, class ...T>
inline const I& TaskClosureImpl<T...>::Get()
{
	// TODO: insert return statement here
}

//====================== CapturedTask
template<class ReadClosure, class WriteClosure>
inline CapturedTask<ReadClosure, WriteClosure>::CapturedTask(const std::string& name, const ReadClosure& readClosure, const WriteClosure& writeClosure)
	: Task(name)
	, m_readClosure(readClosure)
	, m_writeClosure(writeClosure)
{

}

template<class ReadClosure, class WriteClosure>
inline CapturedTask<ReadClosure, WriteClosure>::~CapturedTask()
{}

template<class ReadClosure, class WriteClosure, class T>
inline ptr<const T>& CapturedTask<ReadClosure, WriteClosure>::GetRead()
{
	// TODO: insert return statement here
}

template<class ReadClosure, class WriteClosure, class T>
inline ptr<T>& CapturedTask<ReadClosure, WriteClosure>::GetWrite()
{
	// TODO: insert return statement here
}

} // namespace RED_LILIUM_NAMESPACE
