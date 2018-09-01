#include "pch.h"

#include "Commands.h"
#include <TaskManager/Task.h>
#include <TaskManager/TaskManager.h>

REGISTER_COMMAND("WindowsManagerDemo", WindowsManagerDemo)

using namespace RED_LILIUM_NAMESPACE;

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

int WindowsManagerDemo(ptr<ArgsParser> parser)
{
	sptr<InitialTask> initialTask = std::make_shared<InitialTask>();
	uptr<TaskManager> taskManager = umake<TaskManager>();
	taskManager->Start(initialTask, 2);
	taskManager->Wait();
	return 0;
}
