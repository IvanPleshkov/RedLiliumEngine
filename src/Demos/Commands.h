#pragma once

#include <Core/Common.h>
#include "ArgsParser.h"

namespace RED_LILIUM_NAMESPACE
{
	typedef std::function<int(ptr<ArgsParser>)> RegisteredCommand;

	class UtilCommands
	{
	public:
		UtilCommands(std::string name, RegisteredCommand command, RegisteredCommand help_command = RegisteredCommand())
		{
			AddCommand(name, command);
			AddHelpCommand(name, help_command);
		}

		static std::map<std::string, RegisteredCommand>& GetCommands()
		{
			return AddCommand();
		}

		static std::map<std::string, RegisteredCommand>& GetHelpCommands()
		{
			return AddHelpCommand();
		}

	private:
		static std::map<std::string, RegisteredCommand>& AddCommand(
			std::string name = std::string(),
			std::optional<RegisteredCommand> command = std::nullopt)
		{
			static std::map<std::string, RegisteredCommand> commands;

			if (command)
			{
				commands.insert({ name, command.value() });
			}

			return commands;
		}

		static std::map<std::string, RegisteredCommand>& AddHelpCommand(
			std::string name = std::string(),
			std::optional<RegisteredCommand> command = std::nullopt)
		{
			static std::map<std::string, RegisteredCommand> help_commands;

			if (command)
			{
				help_commands.insert({ name, command.value() });
			}

			return help_commands;
		}
	};

}

#define REGISTER_COMMAND(name, command)\
\
int command(RED_LILIUM_NAMESPACE::ptr<RED_LILIUM_NAMESPACE::ArgsParser> parser);\
\
namespace \
{\
RED_LILIUM_NAMESPACE::UtilCommands r(name, RED_LILIUM_NAMESPACE::RegisteredCommand(command));\
}

#define REGISTER_COMMAND_WITH_HELP(name, command, help_command)\
\
int command(RED_LILIUM_NAMESPACE::ptr<RED_LILIUM_NAMESPACE::ArgsParser> parser);\
int help_command(RED_LILIUM_NAMESPACE::ptr<RED_LILIUM_NAMESPACE::ArgsParser> parser);\
\
namespace \
{\
RED_LILIUM_NAMESPACE::UtilCommands r(name, RED_LILIUM_NAMESPACE::RegisteredCommand(command), RED_LILIUM_NAMESPACE::RegisteredCommand(help_command));\
}
