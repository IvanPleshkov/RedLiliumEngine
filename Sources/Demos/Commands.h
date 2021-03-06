#pragma once

#include <Core/Common.h>
#include <Core/ApplicationSettings.h>

namespace RED_LILIUM_NAMESPACE
{
	typedef std::function<int(ptr<ApplicationSettings>)> RegisteredCommand;

	class UtilCommands
	{
	public:
		UtilCommands(const std::string& name, RegisteredCommand command)
		{
			AddCommand(name, command);
		}

		static std::map<std::string, RegisteredCommand>& GetCommands()
		{
			return AddCommand(std::string(), std::nullopt);
		}

	private:
		static std::map<std::string, RegisteredCommand>& AddCommand(
			const std::string& name,
			std::optional<RegisteredCommand> command)
		{
			static std::map<std::string, RegisteredCommand> commands;

			if (command && !name.empty())
			{
				commands.insert({ name, command.value() });
			}

			return commands;
		}
	};
}

#define REGISTER_COMMAND(name, command)\
\
int command(RED_LILIUM_NAMESPACE::ptr<RED_LILIUM_NAMESPACE::ApplicationSettings> parser);\
\
namespace \
{\
RED_LILIUM_NAMESPACE::UtilCommands register_##command(name, RED_LILIUM_NAMESPACE::RegisteredCommand(command));\
}
