#include "commandInterpreter.hpp"

#include "Utilities/debug.hpp"
#include "Base/gameData.hpp"

using PopHead::Terminal::CommandInterpreter;

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
	PH_LOG(LogType::Info, "handling command with one argument");
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
