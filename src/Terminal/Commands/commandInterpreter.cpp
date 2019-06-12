#include "commandInterpreter.hpp"

#include <array>
#include "Utilities/debug.hpp"
#include "Base/gameData.hpp"
#include "NonArgumentCommands/exitCommand.hpp"

void ph::CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;

	if(commandHasAnArgument())
		handleCommandWithOneArgument();
	else
		handleCommandWithoutArguments();
}

bool ph::CommandInterpreter::commandHasAnArgument()
{
	return commandContains(' ') || commandContains('-') || commandContains('=');
}

bool ph::CommandInterpreter::commandContains(const char c)
{
	return mCommand.find(c) != std::string::npos;
}

void ph::CommandInterpreter::handleCommandWithOneArgument()
{
	const std::string commandWithoutArguments = getCommandWithoutArguments();
	if(commandWithoutArguments == "log")
		executeLog();
}

std::string ph::CommandInterpreter::getCommandWithoutArguments()
{
	return mCommand.substr(0, getArgumentPositionInCommand());
}

int ph::CommandInterpreter::getArgumentPositionInCommand()
{
	size_t argumentPosition;
	std::array<char, 3> argumentCharacters{' ', '-', '='};
	for(int i = 0; i < 3; ++i) {
		argumentPosition = mCommand.find(argumentCharacters[i]);
		if(argumentPosition != std::string::npos)
			return argumentPosition;
	}
}

void ph::CommandInterpreter::executeLog()
{
	PH_LOG(LogType::Info, "execution of log command");
}

void ph::CommandInterpreter::handleCommandWithoutArguments()
{
	if(mCommand == "exit") ExitCommand::execute(mGameData);
}