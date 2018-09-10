#include "pch.h"

#include "Commands.h"
#include <TaskManager/Task.h>
#include <TaskManager/TaskManager.h>
#include <WindowsManager/WindowsManager.h>

REGISTER_COMMAND("WindowsManagerDemo", WindowsManagerDemo)

using namespace RED_LILIUM_NAMESPACE;

namespace
{

class InitialTask : public Task
{
public:
	InitialTask(ptr<WindowsManager> windowsManager)
		: Task("Initial Task")
		, m_windowsManager(windowsManager)
	{}

	~InitialTask() override {}

	bool Run() override
	{
		m_windowsManager->Start();

		return true;
	}

private:
	ptr<WindowsManager> m_windowsManager;
};

}

int WindowsManagerDemo(ptr<ArgsParser> parser)
{
	uptr<WindowsManager> windowsManager;

	sptr<InitialTask> initialTask = std::make_shared<InitialTask>(windowsManager.get());
	initialTask->AddWriteResource(windowsManager.get());

	uptr<TaskManager> taskManager = umake<TaskManager>();
	taskManager->Start(initialTask, 2);
	taskManager->Wait();
	return 0;
}
