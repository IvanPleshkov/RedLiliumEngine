#include "pch.h"

#include "Commands.h"
#include <TaskManager/TaskManager.h>
#include <TaskManager/Task.h>

REGISTER_COMMAND("TaskManagerDemo", TaskManagerDemo)

using namespace RED_LILIUM_NAMESPACE;

namespace
{

class InitialTask : public Task
{

public:
	InitialTask()
		: Task("Initial Task")
	{}

	~InitialTask() override {}

	bool Run() override
	{
		return true;
	}
};

}

int TaskManagerDemo(ptr<ArgsParser> parser)
{
	sptr<InitialTask> initialTask = std::make_shared<InitialTask>();
	uptr<TaskManager> taskManager = umake<TaskManager>();
	taskManager->Start(initialTask, 2);
	taskManager->Wait();
	return 0;
}
