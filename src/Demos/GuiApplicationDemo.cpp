#include "pch.h"

#include "Commands.h"
#include <TaskManager/Task.h>
#include <TaskManager/TaskManager.h>
#include <WindowsManager/WindowsManager.h>

REGISTER_COMMAND("GuiApplicationDemo", GuiApplicationDemo)

using namespace RED_LILIUM_NAMESPACE;

namespace
{
}

int GuiApplicationDemo(ptr<ArgsParser> parser)
{
	return 0;
}
