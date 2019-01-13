#include "pch.h"

#include "Commands.h"

using namespace RED_LILIUM_NAMESPACE;

int main(int argc, char** argv)
{
	if (argc < 2)
		return -1;

	uptr<ApplicationSettings> applicationSettings = umake<ApplicationSettings>(argc, argv);
	auto& commands = UtilCommands::GetCommands();

	const std::string command = argv[1];
	if (commands.find(command) != commands.end())
	{
		return commands.at(command)(applicationSettings.get());
	}

	return 0;
}
