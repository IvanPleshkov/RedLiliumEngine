#include "pch.h"
#include "WindowsManager.h"
#include "IWindow.h"
#include <TaskManager/Task.h>
#include <TaskManager/TaskManager.h>
#include <TaskManager/CapturedTask.h>

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

class WindowsManager::CreateWindowTask : public CapturedTask<
	TaskReadClosure<>,
	TaskWriteClosure<WindowsManager>>
{
public:
	CreateWindowTask(AsyncReturn<ptr<IWindow>>&& returnWindow)
		: CapturedTaskType("CreateWindowTask")
		, m_return(std::move(returnWindow))
	{}

	~CreateWindowTask() override {}

	bool Run() override
	{
		Get<WindowsManager>()->CreateWindowImpl();
		m_return.SetReturn(GetWrite<WindowsManager>()->CreateWindowImpl());
		return true;
	}

private:
	AsyncReturn<ptr<IWindow>> m_return;
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
	AsyncResult<ptr<IWindow>> asyncResult = asyncReturn.GetResult();

	sptr<CreateWindowTask> task = smake<CreateWindowTask>(std::move(asyncReturn));
	task->Set(this);
	TaskManager::AddTask(task);

	return asyncResult;
}

void WindowsManager::DeleteWindowAsync(ptr<IWindow> window) const
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

ptr<IWindow> WindowsManager::CreateWindowImpl()
{
	return nullptr;
}

void WindowsManager::DeleteWindowImpl(ptr<IWindow> window)
{
}
