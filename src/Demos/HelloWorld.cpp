#include "pch.h"

#include "Commands.h"

REGISTER_COMMAND("HelloWorld", HelloWorld)

using namespace RED_LILIUM_NAMESPACE;

int HelloWorld(ptr<ArgsParser> parser)
{
	return 0;
}
