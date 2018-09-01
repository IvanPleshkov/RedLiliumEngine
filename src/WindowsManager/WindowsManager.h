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
	void DeleteWindow(ptr<IWindow> window) const;

private:
	void SwapBuffers();

};

} // namespace RED_LILIUM_NAMESPACE
