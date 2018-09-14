#pragma once

#include "Task.h"

namespace RED_LILIUM_NAMESPACE
{

template <class T, class Tuple>
struct TupleHasType;

template <class T>
struct TupleHasType<T, std::tuple<>> 
{
	using Result = std::false_type;
};

template <class T, class U, class... Ts>
struct TupleHasType<T, std::tuple<U, Ts...>> : TupleHasType<T, std::tuple<Ts...>> {};

template <class T, class... Ts>
struct TupleHasType<T, std::tuple<T, Ts...>>
{
	using Result = std::true_type;
};

class EmptyClass {};

template<class... T>
class TaskClosureImpl : private std::tuple<ptr<T>...>
{
public:
	template<class I>
	using HasType = TupleHasType<I, std::tuple<T...>>;

    explicit TaskClosureImpl(ptr<T>... args) : std::tuple<ptr<T>...>(args...)
    {}

	template<class I>
	ptr<I>& Get()
	{
		return std::get<ptr<I>>(*static_cast<std::tuple<ptr<T>...>*>(this));
	}
};

template<class T, class ReadClosure, class WriteClosure, class IsRead>
struct ReturnByType
{
	using Type = ptr<T>;
};

template<class T, class ReadClosure, class WriteClosure>
struct ReturnByType<T, ReadClosure, WriteClosure, std::true_type>
{
	using Type = ptr<const T>;
};

template<class... T>
class TaskReadClosure : public TaskClosureImpl<const T...>
{
public:
    explicit TaskReadClosure(EmptyClass) : TaskClosureImpl<const T...>(ptr<const T>(nullptr)...)
	{}

    TaskReadClosure(ptr<const T>... args) : TaskClosureImpl<const T...>(args...)
    {}
};

template<class... T>
class TaskWriteClosure : public TaskClosureImpl<T...>
{
public:
    explicit TaskWriteClosure(EmptyClass) : TaskClosureImpl<T...>(ptr<T>(nullptr)...)
	{}

    TaskWriteClosure(ptr<T>... args) : TaskClosureImpl<T...>(args...)
    {}
};

template<class ReadClosure = TaskReadClosure<>, class WriteClosure = TaskWriteClosure<>>
class CapturedTask : public Task
{
public:
	using Read = ReadClosure;
	using Write = WriteClosure;
	using CapturedTaskType = CapturedTask<ReadClosure, WriteClosure>;
	
	template <class T>
	using IsReadType = typename ReadClosure::template HasType<const T>::Result;
	template <class T>
	using IsWriteType = typename WriteClosure::template HasType<T>::Result;

	template <class T>
	using ResourceType = typename ReturnByType<T, ReadClosure, WriteClosure, IsReadType<T>>::Type;

public:
    CapturedTask(const std::string& name)
		: Task(name)
        , m_readClosure(EmptyClass())
        , m_writeClosure(EmptyClass())
	{ }

    CapturedTask(const std::string& name, ReadClosure readClosure, WriteClosure writeClosure)
        : Task(name)
        , m_readClosure(readClosure)
        , m_writeClosure(writeClosure)
    { }

	~CapturedTask() override {}

	template <class T>
	ptr<const T> GetRead()
	{
        return m_readClosure.template Get<const T>();
	}

	template <class T>
	ptr<T> GetWrite()
	{
        return m_writeClosure.template Get<T>();
	}

	template <class T>
	ResourceType<T> Get()
	{
		return Get<T>(IsReadType<T>());
	}

	template <class T>
	void SetRead(ptr<const T> resource)
	{
		RED_LILIUM_ASSERT(GetRead<T>() == nullptr);
        m_readClosure.template Get<const T>() = resource;
		AddReadResource(resource);
	}

	template <class T>
	void SetWrite(ptr<const T> resource)
	{
		RED_LILIUM_ASSERT(GetWrite<T>() == nullptr);
        m_writeClosure.template Get<T>() = ConstCast<T>(resource);
		AddWriteResource(resource);
	}

	template <class T>
	void Set(ptr<const T> resource)
	{
		return Set<T>(resource, IsReadType<T>());
	}

#if RED_LILIUM_POINTER_COUNTERS

	template <class T>
	void SetRead(const T* resource)
	{
		SetRead(ptr<const T>(resource));
	}

	template <class T>
	void SetRead(T* resource)
	{
		SetRead(ptr<const T>(resource));
	}

	template <class T>
	void SetWrite(const T* resource)
	{
		SetWrite(ptr<const T>(resource));
	}

	template <class T>
	void SetWrite(T* resource)
	{
		SetWrite(ptr<const T>(resource));
	}

	template <class T>
	void Set(const T* resource)
	{
		return Set<T>(ptr<const T>(resource), IsReadType<T>());
	}

	template <class T>
	void Set(T* resource)
	{
		return Set<T>(ptr<const T>(resource), IsReadType<T>());
	}

#endif // RED_LILIUM_POINTER_COUNTERS

private:

	template <class T>
	ResourceType<T> Get(std::true_type)
	{
		return GetRead<T>();
	}

	template <class T>
	ResourceType<T> Get(std::false_type)
	{
		return GetWrite<T>();
	}

	template <class T>
	void Set(ptr<const T> resource, std::true_type)
	{
		return SetRead<T>(resource);
	}

	template <class T>
	void Set(ptr<const T> resource, std::false_type)
	{
		return SetWrite<T>(resource);
	}

	ReadClosure m_readClosure;
	WriteClosure m_writeClosure;
};


template<class ReadClosure = TaskReadClosure<>, class WriteClosure = TaskWriteClosure<>>
void AddAsyncTask(
        const std::string& name,
        ReadClosure readClosure,
        WriteClosure writeClosure,
        std::function<void()> lambda)
{
    class CapturedTaskImpl : public CapturedTask<ReadClosure, WriteClosure>
    {
    public:
        CapturedTaskImpl(
                    const std::string& name,
                    ReadClosure readClosure,
                    WriteClosure writeClosure,
                    std::function<void()> lambda)
            : CapturedTask<ReadClosure, WriteClosure>(name, readClosure, writeClosure)
            , m_lambda(lambda)
        {}
        ~CapturedTaskImpl() override {}

        bool Run() override
        {
            m_lambda();
            return true;
        }

    private:
        std::function<void()> m_lambda;
    };
}

} // namespace RED_LILIUM_NAMESPACE
