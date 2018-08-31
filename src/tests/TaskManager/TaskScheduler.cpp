#include "pch.h"

#include <TaskManager/TaskScheduler.h>
#include <TaskManager/Task.h>
#include "Tests.h"

using namespace RED_LILIUM_NAMESPACE;

class TestTask : public Task
{
public:
	TestTask() : Task("TestTask") {}
	~TestTask() override {}

	bool Run() override
	{
		return true;
	}
};

static std::vector<sptr<Task>> GetTasksOrder(ptr<TaskScheduler> scheduler, u8 threadsCount = 1)
{
	std::vector<sptr<Task>> result;

	sptr<Task> currentTask;
	do
	{
		currentTask = scheduler->PopTask();

		if (currentTask)
		{
			scheduler->OnStartTask(currentTask);
			scheduler->OnFinishTask(currentTask);
			result.push_back(currentTask);
		}
	} while (currentTask != nullptr);

	return std::move(result);
}

TEST(TaskScheduler, Construct_0)
{
	uptr<TaskScheduler> taskSheduler = umake<TaskScheduler>();

	sptr<Task> task0 = smake<TestTask>();
	sptr<Task> task1 = smake<TestTask>();
	sptr<Task> task2 = smake<TestTask>();

	task2->AddDependTask(task1);
	task1->AddDependTask(task0);

    taskSheduler->PushTasks({ task0, task1, task2 });

	auto order = GetTasksOrder(taskSheduler.get());

    ASSERT_TRUE(order[0] == task0);
    ASSERT_TRUE(order[1] == task1);
    ASSERT_TRUE(order[2] == task2);
}

TEST(TaskScheduler, Construct_1)
{
    uptr<TaskScheduler> taskSheduler = umake<TaskScheduler>();

    sptr<Task> task0 = smake<TestTask>();
    sptr<Task> task1 = smake<TestTask>();
    sptr<Task> task2 = smake<TestTask>();

    task2->AddDependTask(task1);
    task1->AddDependTask(task0);

    taskSheduler->PushTasks({ task2, task1, task0 });

    auto order = GetTasksOrder(taskSheduler.get());

    ASSERT_TRUE(order[0] == task0);
    ASSERT_TRUE(order[1] == task1);
    ASSERT_TRUE(order[2] == task2);
}
