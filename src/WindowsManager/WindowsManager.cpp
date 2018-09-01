#include "pch.h"
#include "WindowsManager.h"
#include <TaskManager/Task.h>

using namespace RED_LILIUM_NAMESPACE;



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

	return asyncResult;
}

void WindowsManager::DeleteWindow(ptr<IWindow> window) const
{
}

void WindowsManager::SwapBuffers()
{
}
