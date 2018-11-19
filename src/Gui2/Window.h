#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class Window : public RedLiliumObject
{
public:
	void HandleEvents();
	uptr<Context> CreateContext();
	
private:
};

} // namespace RED_LILIUM_NAMESPACE
