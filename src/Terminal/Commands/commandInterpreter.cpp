#include "commandInterpreter.hpp"

#include <array>
#include "Utilities/debug.hpp"
#include "Base/gameData.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Logs/logger.hpp"

void ph::CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;

	const std::string commandWithoutArguments = getCommandWithoutArguments();

	if (commandWithoutArguments == "log")                           executeLog();
	else if (commandWithoutArguments == "teleport")                 executeTeleport();
	else if (commandWithoutArguments == "currentpos")               executeCurrentPos();
	else if (commandWithoutArguments == "changecollisiondisplay")   executeChangeCollisionDebugDisplay();
	else if (commandWithoutArguments == "changecolor")              executeChangeCollisionDebugColors();
	else if (commandWithoutArguments == "switchcollisionmode")      executeSwitchCollisionDebugMode();
	else if (commandWithoutArguments == "mute")                     executeMute();
	else if (commandWithoutArguments == "unmute")                   executeUnmute();
	else if (commandWithoutArguments == "setvolume")                executeSetVolume();
	else if (commandWithoutArguments == "loginttofile")             executeSetLoggingIntoFile();
	else if (commandWithoutArguments == "logintoconsole")           executeSetLoggingIntoConsole();
	else if (commandWithoutArguments == "logintoboth")              executeSetLogging();
	else if (commandWithoutArguments == "setlogtype")               executeSetLoggingLogTypes();
	else if (commandWithoutArguments == "setmodulename")            executeSetLoggingModuleNames();
	else if (commandWithoutArguments == "exit")                     executeExit();
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
	return mCommand.size();
}

void ph::CommandInterpreter::executeLog()
{
	size_t spacePosition = mCommand.find(' ');
	size_t messageStartPos = spacePosition + 1;
	size_t messageLength = mCommand.size() - messageStartPos;
	std::string message = mCommand.substr(messageStartPos, messageLength);
	PH_LOG(LogType::FromUser, message);
}

void ph::CommandInterpreter::executeTeleport()
{
	const std::string numbers("1234567890");

	size_t xArgumentPositionInCommand = mCommand.find_first_of(numbers);
	size_t xArgumentEndPositionInCommand = mCommand.find(' ', xArgumentPositionInCommand);
	size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = mCommand.substr(xArgumentPositionInCommand, xArgumentLength);
	float newXPosition = std::stof(xArgument);

	size_t yArgumentPositionInCommand = mCommand.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
	size_t yArgumentEndPositionInCommand = mCommand.find_first_not_of(numbers, yArgumentPositionInCommand);
	if(yArgumentEndPositionInCommand == std::string::npos)
		yArgumentEndPositionInCommand = mCommand.size();
	size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	std::string yArgument = mCommand.substr(yArgumentPositionInCommand, yArgumentLength);
	float newYPosition = std::stof(yArgument);

	auto& gameState = mGameData->getStateMachine().getTopState();
	auto& root = gameState.getRoot();
	auto& player = dynamic_cast<Object&>(root.getChild("player"));
	player.setPosition({newXPosition, newYPosition});
}

void ph::CommandInterpreter::executeCurrentPos()
{
	auto& gameState = mGameData->getStateMachine().getTopState();
	auto& root = gameState.getRoot();
	auto& player = dynamic_cast<Object&>(root.getChild("player"));
	std::string x = std::to_string(player.getPosition().x);
	std::string y = std::to_string(player.getPosition().y);
	PH_LOG(LogType::Info, "player position:  x: " + x + "  y:" + y);
}

void ph::CommandInterpreter::executeChangeCollisionDebugDisplay()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();
	if (commandContains("kinematic"))
		collisionDebugSettings.displayOnlyKinematicBodies();
	else if (commandContains("static"))
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

void ph::CommandInterpreter::executeMute()
{
	if (commandContains("music"))
		mGameData->getMusicPlayer().setMuted(true);
	else if (commandContains("sound"))
		mGameData->getSoundPlayer().setVolume(0.f);
}

void ph::CommandInterpreter::executeUnmute()
{
	if (commandContains("music"))
		mGameData->getMusicPlayer().setMuted(false);
	else if (commandContains("sound"))
		mGameData->getSoundPlayer().setVolume(20.f);
}

void ph::CommandInterpreter::executeSetVolume()
{
	if (commandContains("music"))
		mGameData->getMusicPlayer().setVolume(getVolumeFromCommand());
	else if (commandContains("sound"))
		mGameData->getSoundPlayer().setVolume(getVolumeFromCommand());
	else{
		mGameData->getMusicPlayer().setVolume(getVolumeFromCommand());
		mGameData->getSoundPlayer().setVolume(getVolumeFromCommand());
	}
}

float ph::CommandInterpreter::getVolumeFromCommand()
{
	size_t spacePosition = mCommand.find_last_of(' ');
	size_t valueStartPos = spacePosition + 1;
	size_t valueLength = mCommand.size() - valueStartPos;
	std::string textToFloat = mCommand.substr(valueStartPos, valueLength);
	float volumeValue = std::strtof(textToFloat.c_str(), nullptr);
	return !(volumeValue) ? 50.f : volumeValue;	
}

void ph::CommandInterpreter::executeSetLoggingIntoFile()
{
	auto& logSettings = Logger::getLogger();
	if (commandContains('1'))
		logSettings.getLogSettings().setWritingLogsIntoFile(true);
	else if (commandContains('0'))
		logSettings.getLogSettings().setWritingLogsIntoFile(false);
}
void ph::CommandInterpreter::executeSetLoggingIntoConsole()
{
	auto& logSettings = Logger::getLogger();
	if (commandContains('1'))
		logSettings.getLogSettings().setWritingLogsIntoConsole(true);
	else if (commandContains('0'))
		logSettings.getLogSettings().setWritingLogsIntoConsole(false);
}

void ph::CommandInterpreter::executeSetLogging()
{
	executeSetLoggingIntoConsole();
	executeSetLoggingIntoFile();
}

void ph::CommandInterpreter::executeSetLoggingLogTypes()
{
	auto& logSettings = Logger::getLogger();
	if (commandContains("info")) 			logSettings.getLogSettings().addLogType(LogType::Info);
	else if (commandContains("warning")) 	logSettings.getLogSettings().addLogType(LogType::Warning);
	else if (commandContains("error"))		logSettings.getLogSettings().addLogType(LogType::Error);
	else if (commandContains("user"))		logSettings.getLogSettings().addLogType(LogType::FromUser);

	else if (commandContains("all"))		
		logSettings.getLogSettings().turnOnWritingLogsFromEachLogTypes();
	else if (commandContains("clear"))	
		logSettings.getLogSettings().setLogTypesToWrite({});
}

void ph::CommandInterpreter::executeSetLoggingModuleNames()
{
	auto& logSettings = Logger::getLogger();
	if (commandContains("audio")) 			logSettings.getLogSettings().addModuleName("Audio");
	else if (commandContains("base"))		logSettings.getLogSettings().addModuleName("Base");
	else if (commandContains("input"))		logSettings.getLogSettings().addModuleName("Input");
	else if (commandContains("logs"))		logSettings.getLogSettings().addModuleName("Logs");
	else if (commandContains("physics"))	logSettings.getLogSettings().addModuleName("Physics");
	else if (commandContains("renderer"))	logSettings.getLogSettings().addModuleName("Renderer");
	else if (commandContains("resources"))	logSettings.getLogSettings().addModuleName("Resources");
	else if (commandContains("states"))		logSettings.getLogSettings().addModuleName("States");
	else if (commandContains("utilities"))	logSettings.getLogSettings().addModuleName("Utilities");
	else if (commandContains("world"))		logSettings.getLogSettings().addModuleName("World");
	else if (commandContains("terminal"))	logSettings.getLogSettings().addModuleName("Terminal");
	else if (commandContains("none"))		logSettings.getLogSettings().addModuleName("None");

	else if (commandContains("all"))
		logSettings.getLogSettings().turnOnWritingLogsFromEachModule();
	else if (commandContains("clear"))
		logSettings.getLogSettings().setModuleNamesToWrite({});
}

void ph::CommandInterpreter::executeExit()
{
	mGameData->getRenderer().getWindow().close();
}

bool ph::CommandInterpreter::commandContains(const char c)
{
	return mCommand.find(c) != std::string::npos;
}

bool ph::CommandInterpreter::commandContains(const char* c)
{
	return mCommand.find(c) != std::string::npos;
}
