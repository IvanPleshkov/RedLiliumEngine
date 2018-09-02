#include "pch.h"

#include <TaskManager/TaskScheduler.h>
#include <TaskManager/Task.h>
#include <TaskManager/CapturedTask.h>
#include "Tests.h"

using namespace RED_LILIUM_NAMESPACE;

class ReadResourceSample0 : public RedLiliumObject
{
public:
    ReadResourceSample0() {}
    ~ReadResourceSample0() override {}
};

class ReadResourceSample1 : public RedLiliumObject
{
public:
    ReadResourceSample1() {}
    ~ReadResourceSample1() override {}
};

class ReadResourceSample2 : public RedLiliumObject
{
public:
    ReadResourceSample2() {}
    ~ReadResourceSample2() override {}
};

class WriteResourceSample0 : public RedLiliumObject
{
public:
    WriteResourceSample0() {}
    ~WriteResourceSample0() override {}
};

class CapturedTaskSample0 : public CapturedTask<
        TaskReadClosure<ReadResourceSample0, ReadResourceSample1, ReadResourceSample2>,
        TaskWriteClosure<WriteResourceSample0>>
{
public:
    CapturedTaskSample0() {}
    ~CapturedTaskSample0() override {}
};
