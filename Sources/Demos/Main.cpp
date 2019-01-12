#include "pch.h"

#include "Commands.h"

using namespace RED_LILIUM_NAMESPACE;

int main(int argc, char** argv)
{
	if (argc < 2)
		return -1;

	argh::parser parser(argv);
	auto& commands = UtilCommands::GetCommands();

	const std::string command = argv[1];
	if (commands.find(command) != commands.end())
	{
		return commands.at(command)(parser);
	}

	return 0;
}
