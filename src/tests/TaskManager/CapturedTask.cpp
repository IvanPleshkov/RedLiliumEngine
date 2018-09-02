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

	uptr<CapturedTaskSample> task = umake<CapturedTaskSample>();

	uptr<ReadResourceSample0> read0 = umake<ReadResourceSample0>();
	uptr<ReadResourceSample1> read1 = umake<ReadResourceSample1>();
	uptr<WriteResourceSample0> write0 = umake<WriteResourceSample0>();

	task->SetRead(read0);
	task->SetRead(read1);
	task->SetWrite(write0);

	task->Run();
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

	uptr<CapturedTaskSample> task = umake<CapturedTaskSample>();

	uptr<ReadResourceSample0> read0 = umake<ReadResourceSample0>();
	uptr<ReadResourceSample1> read1 = umake<ReadResourceSample1>();

	task->SetRead(read0);
	task->SetRead(read1);

	task->Run();
}
