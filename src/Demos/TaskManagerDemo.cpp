#include "pch.h"

#include "Commands.h"
#include <TaskManager/TaskManager.h>
#include <TaskManager/Task.h>

REGISTER_COMMAND("TaskManagerDemo", TaskManagerDemo)

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

int TaskManagerDemo(const ptr<ArgsParser>& parser)
{
	sptr<InitialTask> initialTask = std::make_shared<InitialTask>();
	uptr<TaskManager> taskManager = std::make_unique<TaskManager>(initialTask, 2);
	return 0;
}
