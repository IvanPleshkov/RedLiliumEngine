#include "pch.h"

#include <TaskManager/TaskScheduler.h>
#include <TaskManager/Task.h>
#include <TaskManager/CapturedTask.h>
#include "Tests.h"

using namespace RED_LILIUM_NAMESPACE;

namespace
{
	// Some helping types

	class ReadResourceSample0 : public RedLiliumObject
	{
	public:
		ReadResourceSample0() {}
		~ReadResourceSample0() override {}
	};

	class ReadResourceSample1 : public RedLiliumObject
	{
	public:
		ReadResourceSample1() {}
		~ReadResourceSample1() override {}
	};

	class WriteResourceSample0 : public RedLiliumObject
	{
	public:
		WriteResourceSample0() {}
		~WriteResourceSample0() override {}
	};
}

TEST(CapturedTask, ToolsCompilation)
{
	static_assert(std::is_same<
		TupleHasType<double, std::tuple<int, float, char>>::Result, 
		std::false_type>::value);
	static_assert(std::is_same<
		TupleHasType<float, std::tuple<int, float, char>>::Result,
		std::true_type>::value);
	static_assert(std::is_same<
		TupleHasType<double, std::tuple<>>::Result,
		std::false_type>::value);

	class CapturedTaskSample : public CapturedTask<
		TaskReadClosure<ReadResourceSample0, ReadResourceSample1>,
		TaskWriteClosure<WriteResourceSample0>>
	{};

	static_assert(std::is_same<
		CapturedTaskSample::IsReadType<ReadResourceSample0>,
		std::true_type>::value);
	static_assert(std::is_same<
		CapturedTaskSample::IsReadType<ReadResourceSample1>,
		std::true_type>::value);
	static_assert(std::is_same<
		CapturedTaskSample::IsReadType<WriteResourceSample0>,
		std::false_type>::value);

	static_assert(std::is_same<
		CapturedTaskSample::IsWriteType<WriteResourceSample0>,
		std::true_type>::value);
	static_assert(std::is_same<
		CapturedTaskSample::IsWriteType<ReadResourceSample0>,
		std::false_type>::value);
	static_assert(std::is_same<
		CapturedTaskSample::IsWriteType<ReadResourceSample1>,
		std::false_type>::value);

	static_assert(std::is_same<
		CapturedTaskSample::ResourceType<ReadResourceSample0>,
		ptr<const ReadResourceSample0>> ::value);
	static_assert(std::is_same<
		CapturedTaskSample::ResourceType<ReadResourceSample1>,
		ptr<const ReadResourceSample1>> ::value);
	static_assert(std::is_same<
		CapturedTaskSample::ResourceType<WriteResourceSample0>,
		ptr<WriteResourceSample0>> ::value);
}

TEST(CapturedTask, Compilation_0)
{
	class CapturedTaskSample : public CapturedTask<
		TaskReadClosure<ReadResourceSample0, ReadResourceSample1>,
		TaskWriteClosure<WriteResourceSample0>>
	{
	public:
		CapturedTaskSample()
			: CapturedTaskType("CapturedTaskSample")
		{}

		~CapturedTaskSample() override {}

		bool Run() override
		{
			ptr<const ReadResourceSample0> read0 = GetRead<ReadResourceSample0>();
			ptr<const ReadResourceSample1> read1 = GetRead<ReadResourceSample1>();
			ptr<WriteResourceSample0> write0 = GetWrite<WriteResourceSample0>();

			RED_LILIUM_ASSERT(read0 != nullptr);
			RED_LILIUM_ASSERT(read1 != nullptr);
			RED_LILIUM_ASSERT(write0 != nullptr);

			return true;
		}
	};

	if (false)
	{
		uptr<CapturedTaskSample> task = umake<CapturedTaskSample>();

		uptr<ReadResourceSample0> read0 = umake<ReadResourceSample0>();
		uptr<ReadResourceSample1> read1 = umake<ReadResourceSample1>();
		uptr<WriteResourceSample0> write0 = umake<WriteResourceSample0>();

		task->SetRead(read0.get());
		task->SetRead(read1.get());
		task->SetWrite(write0.get());
	}
}

TEST(CapturedTask, Compilation_1)
{
	class CapturedTaskSample : public CapturedTask<
		TaskReadClosure<ReadResourceSample0, ReadResourceSample1>>
	{
	public:
		CapturedTaskSample()
			: CapturedTaskType("CapturedTaskSample")
		{}

		~CapturedTaskSample() override {}

		bool Run() override
		{
			ptr<const ReadResourceSample0> read0 = GetRead<ReadResourceSample0>();
			ptr<const ReadResourceSample1> read1 = GetRead<ReadResourceSample1>();

			RED_LILIUM_ASSERT(read0 != nullptr);
			RED_LILIUM_ASSERT(read1 != nullptr);

			return true;
		}
	};

	if (false)
	{
		uptr<CapturedTaskSample> task = umake<CapturedTaskSample>();

		uptr<ReadResourceSample0> read0 = umake<ReadResourceSample0>();
		uptr<ReadResourceSample1> read1 = umake<ReadResourceSample1>();

		task->SetRead(read0.get());
		task->SetRead(read1.get());
	}
}

TEST(CapturedTask, Compilation_2)
{
	class CapturedTaskSample : public CapturedTask<
		TaskReadClosure<ReadResourceSample0, ReadResourceSample1>,
		TaskWriteClosure<WriteResourceSample0>>
	{
	public:
		CapturedTaskSample()
			: CapturedTaskType("CapturedTaskSample")
		{}

		~CapturedTaskSample() override {}

		bool Run() override
		{
			ptr<const ReadResourceSample0> read0 = Get<ReadResourceSample0>();
			ptr<const ReadResourceSample1> read1 = Get<ReadResourceSample1>();
			ptr<WriteResourceSample0> write0 = Get<WriteResourceSample0>();

			RED_LILIUM_ASSERT(read0 != nullptr);
			RED_LILIUM_ASSERT(read1 != nullptr);
			RED_LILIUM_ASSERT(write0 != nullptr);

			return true;
		}
	};

	if (false)
	{
		uptr<CapturedTaskSample> task = umake<CapturedTaskSample>();

		uptr<ReadResourceSample0> read0 = umake<ReadResourceSample0>();
		uptr<ReadResourceSample1> read1 = umake<ReadResourceSample1>();
		uptr<WriteResourceSample0> write0 = umake<WriteResourceSample0>();

		task->Set(read0.get());
		task->Set(read1.get());
		task->Set(write0.get());
	}
}

template<class ReadClosure = TaskReadClosure<>, class WriteClosure = TaskWriteClosure<>>
static void ResourcesClosureTestFunc(ReadClosure readClosure, WriteClosure writeClosure)
{
}

TEST(CapturedTask, Compilation_3)
{
    ptr<ReadResourceSample0> readPtr0(nullptr);
    ptr<ReadResourceSample1> readPtr1(nullptr);
    ptr<WriteResourceSample0> writePtr(nullptr);
    auto readClosure = TaskReadClosure(readPtr0, readPtr1);
    auto writeClosure = TaskWriteClosure(writePtr);
    ResourcesClosureTestFunc(readClosure, writeClosure);

    auto emptyReadClosure = TaskReadClosure<>();
    auto emptyWriteClosure = TaskWriteClosure<>();
    ResourcesClosureTestFunc(emptyReadClosure, emptyWriteClosure);
}
