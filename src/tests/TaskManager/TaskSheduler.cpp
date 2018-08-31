#include "pch.h"

#include <TaskManager/TaskSheduler.h>
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

TEST(TaskSheduler, Construct)
{
	uptr<TaskSheduler> taskSheduler = umake<TaskSheduler>();
	std::vector<sptr<TestTask>> testTasks;

	for (auto i : { 0, 1, 2, 3, 4 })
	{
		sptr<TestTask> task = smake<TestTask>();
		testTasks.push_back(task);
	}
}
