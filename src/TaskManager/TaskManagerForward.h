#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Task;
class TaskManager;
class TaskScheduler;

#if !RED_LILIUM_SINGLE_THREAD_MODE
class TaskManagerThread;
#endif // RED_LILIUM_SINGLE_THREAD_MODE

} // namespace RED_LILIUM_NAMESPACE
