#include "pch.h"

#include "Commands.h"

using namespace RED_LILIUM_NAMESPACE;

int main(int argc, char** argv)
{
	if (argc < 2)
		return -1;

	uptr<ArgsParser> parser = std::make_unique<ArgsParser>(argc, argv);
	auto& commands = UtilCommands::GetCommands();
	auto& help_commands = UtilCommands::GetHelpCommands();

	std::string command = argv[1];
	if (command == "help")
	{
		if (help_commands.find(command) != help_commands.end())
			return help_commands.at(command)(parser.get());
	}

	if (commands.find(command) != commands.end())
	{
		return commands.at(command)(parser.get());
	}

	return 0;
}
