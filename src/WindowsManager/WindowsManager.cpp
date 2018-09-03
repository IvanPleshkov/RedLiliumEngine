#include "pch.h"
#include "WindowsManager.h"
#include "IWindow.h"
#include <TaskManager/Task.h>
#include <TaskManager/TaskManager.h>
#include <TaskManager/CapturedTask.h>

using namespace RED_LILIUM_NAMESPACE;

class WindowsManager::CreateWindowTask : public CapturedTask<
	TaskReadClosure<>,
	TaskWriteClosure<WindowsManager>>
{
public:
	CreateWindowTask(AsyncReturn<ptr<IWindow>>&& returnWindow)
		: CapturedTaskType("CreateWindowTask")
		//, m_return(std::move(returnWindow))
	{}

	~CreateWindowTask() override {}

	bool Run() override
	{
		Get<WindowsManager>()->CreateWindowImpl();
		//m_return.SetReturn(GetWrite<WindowsManager>()->CreateWindowImpl());
		return true;
	}

private:
	//AsyncReturn<ptr<IWindow>> m_return;
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

	WindowsManager* t = const_cast<WindowsManager*>(this);
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

ptr<IWindow> WindowsManager::CreateWindowImpl()
{
	return nullptr;
}

void WindowsManager::DeleteWindowImpl(ptr<IWindow> window)
{
}
