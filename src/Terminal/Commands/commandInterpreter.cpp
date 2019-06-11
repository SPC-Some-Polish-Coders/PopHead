#include "commandInterpreter.hpp"

#include <array>
#include "Utilities/debug.hpp"
#include "Base/gameData.hpp"

using ph::Terminal::CommandInterpreter;

void CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;

	if(commandHasAnArgument())
		handleCommandWithOneArgument();
	else
		handleCommandWithoutArguments();
}

bool CommandInterpreter::commandHasAnArgument()
{
	return commandContains(' ') || commandContains('-') || commandContains('=');
}

bool CommandInterpreter::commandContains(const char c)
{
	return mCommand.find(c) != std::string::npos;
}

void CommandInterpreter::handleCommandWithOneArgument()
{
	const std::string commandWithoutArguments = getCommandWithoutArguments();
	if(commandWithoutArguments == "log")
		executeLog();
}

std::string CommandInterpreter::getCommandWithoutArguments()
{
	return mCommand.substr(0, getArgumentPositionInCommand());
}

int CommandInterpreter::getArgumentPositionInCommand()
{
	size_t argumentPosition;
	std::array<char, 3> argumentCharacters{' ', '-', '='};
	for(int i = 0; i < 3; ++i) {
		argumentPosition = mCommand.find(argumentCharacters[i]);
		if(argumentPosition != std::string::npos)
			return argumentPosition;
	}
}

void CommandInterpreter::executeLog()
{
	PH_LOG(LogType::Info, "execution of log command");
}

void CommandInterpreter::handleCommandWithoutArguments()
{
	if(mCommand == "test")      executeTest();
	else if(mCommand == "exit") executeExit();
}

void CommandInterpreter::executeTest()
{
	PH_LOG(LogType::Info, "terminal test");
}

void CommandInterpreter::executeExit()
{
	mGameData->getRenderer().getWindow().close();
}
