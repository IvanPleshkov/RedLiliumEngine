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

	AsyncResult<ptr<IWindow>> CreateWindowAsync() const;
	void DeleteWindowAsync(ptr<IWindow> window) const;

protected:

private:
	class CreateWindowTask;
	friend class CreateWindowTask;

	class WindowsManagerTickTask;
	friend class WindowsManagerTickTask;

	ptr<IWindow> CreateWindowImpl();
	void DeleteWindowImpl(ptr<IWindow> window);
	void SwapBuffers();

	std::set<uptr<IWindow>> m_windows;
};

} // namespace RED_LILIUM_NAMESPACE
