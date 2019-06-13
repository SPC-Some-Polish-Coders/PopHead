#include "commandInterpreter.hpp"

#include <array>
#include "Utilities/debug.hpp"
#include "Base/gameData.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"


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

bool ph::CommandInterpreter::commandContains(const char* c)
{
	return mCommand.find(c) != std::string::npos;
}

void ph::CommandInterpreter::handleCommandWithOneArgument()
{
	const std::string commandWithoutArguments = getCommandWithoutArguments();
	if (commandWithoutArguments == "log")
		executeLog();
	else if (commandWithoutArguments == "changecolldisplay")
		executeChangeCollisionDebugDisplay();
	else if (commandWithoutArguments == "changecolor")
		executeChangeCollisionDebugColors();
	else if (commandWithoutArguments == "switchcollmode")
		executeSwitchCollisionDebugMode();
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
	size_t spacePosition = mCommand.find(' ');
	size_t messageStartPos = spacePosition + 1;
	size_t messageLength = mCommand.size() - messageStartPos;
	std::string message = mCommand.substr(messageStartPos, messageLength);
	PH_LOG(LogType::FromUser, message);
}

void ph::CommandInterpreter::handleCommandWithoutArguments()
{
	if(mCommand == "exit") executeExit();
}

void ph::CommandInterpreter::executeExit()
{
	mGameData->getRenderer().getWindow().close();
}

void ph::CommandInterpreter::executeChangeCollisionDebugDisplay()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();
	if (commandContains("kin"))
		collisionDebugSettings.displayOnlyKinematicBodies();
	else if (commandContains("sta"))
		collisionDebugSettings.displayOnlyStaticBodies();
	else if (commandContains("all"))
		collisionDebugSettings.displayAllBodies();
}

void ph::CommandInterpreter::executeChangeCollisionDebugColors() 
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();
	if (commandContains('1'))
		collisionDebugSettings.setColors(1);
	else if (commandContains('2'))
		collisionDebugSettings.setColors(2);
	else if (commandContains('3'))
		collisionDebugSettings.setColors(3);
}

void ph::CommandInterpreter::executeSwitchCollisionDebugMode()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();
	if (commandContains('1'))
		collisionDebugSettings.turnOn();
	else if (commandContains('0'))
		collisionDebugSettings.turnOff();
}

