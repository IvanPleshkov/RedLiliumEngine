#pragma once

#include "WindowsManagerForward.h"
#include <TaskManager/AsyncReturn.h>

namespace RED_LILIUM_NAMESPACE
{

class WindowsManager : public RedLiliumObject
{
public:
	WindowsManager();
	~WindowsManager() override;

	void Start();
	AsyncResult<ptr<IWindow>> CreateWindowAsync() const;
	ptr<IWindow> CreateWindowSync();
	void DeleteWindowAsync(ptr<IWindow> window) const;

protected:

private:
	class CreateWindowTask;
	friend class CreateWindowTask;

	class WindowsManagerTickTask;
	friend class WindowsManagerTickTask;

	void DeleteWindowImpl(ptr<IWindow> window);
	void Tick();
	void SwapBuffers();

	void CreateTickTask();

	std::set<uptr<IWindow>> m_windows;
};

} // namespace RED_LILIUM_NAMESPACE
