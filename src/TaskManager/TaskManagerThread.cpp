#include "pch.h"
#include "TaskManager.h"
#include "Task.h"
#include "TaskManagerThread.h"
#include "TaskSheduler.h"

using namespace RED_LILIUM_NAMESPACE;

TaskManagerThread::TaskManagerThread()
{
}

TaskManagerThread::~TaskManagerThread()
{
}

void TaskManagerThread::RunThread(const sptr<Task>& startTask)
{
}
