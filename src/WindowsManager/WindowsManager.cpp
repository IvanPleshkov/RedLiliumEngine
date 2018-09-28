#include "pch.h"
#include "WindowsManager.h"
#include "IWindow.h"
#include <TaskManager/Task.h>
#include <TaskManager/TaskManager.h>
#include <TaskManager/CapturedTask.h>
#include <TaskManager/ScopedTask.h>


using namespace RED_LILIUM_NAMESPACE;

class WindowsManager::WindowsManagerTickTask : public CapturedTask<
	TaskReadClosure<>,
	TaskWriteClosure<WindowsManager>>
{
public:
	WindowsManagerTickTask()
		: CapturedTaskType("WindowsManagerTick")
	{}

	~WindowsManagerTickTask() override {}

	bool Run() override
	{
		ptr<WindowsManager> windowsManager = Get<WindowsManager>();
		windowsManager->Tick();
		windowsManager->CreateTickTask();
		return true;
	}
};

WindowsManager::WindowsManager()
{
}

WindowsManager::~WindowsManager()
{
}

AsyncResult<ptr<IWindow>> WindowsManager::CreateWindowAsync() const
{
	AsyncReturn<ptr<IWindow>> asyncReturn(nullptr);

	ScopedTask::Add("CreateWindowTask", {}, { this }, {}, 
		[asyncReturn](const sptr<ScopedTask>& task)
	{
		auto windowsManager = task->GetWrite<WindowsManager>();
		ptr<IWindow> window = windowsManager->CreateWindowSync();
		// asyncReturn.SetReturn(window);
		return true;
	});

	return asyncReturn.GetResult();
}

void WindowsManager::DestroyWindowAsync(ptr<IWindow> window) const
{
}

void WindowsManager::SwapBuffers()
{
}

void WindowsManager::Start()
{
	CreateTickTask();
}

void WindowsManager::Tick()
{

}

void WindowsManager::CreateTickTask()
{
	sptr<WindowsManagerTickTask> nextTickTask = smake<WindowsManagerTickTask>();
	nextTickTask->Set(this);
	TaskManager::AddTask(nextTickTask);
}

ptr<IWindow> WindowsManager::CreateWindowSync()
{
	return nullptr;
}

void WindowsManager::DeleteWindowImpl(ptr<IWindow> window)
{
}
