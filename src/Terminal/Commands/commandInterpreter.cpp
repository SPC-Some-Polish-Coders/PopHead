#include "commandInterpreter.hpp"

#include "Utilities/debug.hpp"
#include "gameData.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Logs/logger.hpp"
#include "Utilities/cast.hpp"

namespace ph {

void CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;

	const std::string commandWithoutArguments = getCommandWithoutArguments();

	if (commandWithoutArguments == "echo")                 executeEcho();
	else if (commandWithoutArguments == "exit")            executeExit();
	else if (commandWithoutArguments == "teleport")        executeTeleport();
	else if (commandWithoutArguments == "currentpos")      executeCurrentPos();
	else if (commandWithoutArguments == "collisiondebug")  executeCollisionDebug();
	else if (commandWithoutArguments == "mute")            executeMute();
	else if (commandWithoutArguments == "unmute")          executeUnmute();
	else if (commandWithoutArguments == "setvolume")       executeSetVolume();
	else if (commandWithoutArguments == "log")             executeLog();
	else if (commandWithoutArguments == "history")		   executeHistory();
	else if (commandWithoutArguments == "help")			   executeHelp();
	else if (commandWithoutArguments == "clear")		   executeClear();
	else if(commandWithoutArguments == "") PH_LOG(LogType::Info, "This is terminal. Enter 'help' to see availible commands.");
	else PH_LOG(LogType::Error, "Entered command wasn't recognised. Enter 'help' to see availible commands.");
}

std::string CommandInterpreter::getCommandWithoutArguments()
{
	return mCommand.substr(0, getArgumentPositionInCommand());
}

int CommandInterpreter::getArgumentPositionInCommand()
{
	size_t argumentPosition = mCommand.find(' ');
	return argumentPosition == std::string::npos ? mCommand.size() : argumentPosition;
}

void CommandInterpreter::executeEcho()
{
	size_t spacePosition = mCommand.find(' ');
	size_t messageStartPos = spacePosition + 1;
	size_t messageLength = mCommand.size() - messageStartPos;
	std::string message = mCommand.substr(messageStartPos, messageLength);
	PH_LOG(LogType::FromUser, message);
}

void CommandInterpreter::executeHistory()
{
	auto& terminalData = mGameData->getTerminal();
	auto& commandsHistory = mGameData->getTerminal().getSharedData()->mLastCommands;
	std::deque<std::string>::reverse_iterator it = commandsHistory.rbegin();

	for (; it != commandsHistory.rend(); ++it)
		terminalData.pushOutputLine({ "- " + *it, sf::Color(127, 244, 44) });
	terminalData.pushOutputLine({ "Ten last used commands: ",sf::Color(127, 244, 44) });
}

void CommandInterpreter::executeHelp()
{
	std::vector<std::string> commandsList1 {
		"EXIT", "ECHO", "HISTORY", "HELP", "MUTE", "UNMUTE", "SETVOLUME",
		"TELEPORT","CURRENTPOS", "LOG", "COLLISIONDEBUG"
	};

	sf::Color infoColor = {127, 244, 44};
	auto& terminal = mGameData->getTerminal();

	if (commandContains('2')){
		terminal.pushOutputLine({ "Will be fulfilled if necessary", infoColor });
		terminal.pushOutputLine({ "Avalible commands, PAGE 2 of 2.", infoColor });
	}
	else{
		for (size_t i = 0; i < commandsList1.size(); ++i)
			terminal.pushOutputLine({ "- " + commandsList1[i], infoColor });
		terminal.pushOutputLine({ "Avalible commands, PAGE 1 of 2.", infoColor });
	}
}

void CommandInterpreter::executeClear()
{
	for (int i = 0; i < 20; ++i)
		mGameData->getTerminal().pushOutputLine({"", sf::Color::Transparent});
}

void CommandInterpreter::executeExit()
{
	mGameData->getRenderer().getWindow().close();
}

void CommandInterpreter::executeTeleport()
{
	auto& player = getPlayer();
	sf::Vector2f newPosition = getTeleportPositionFromCommand();
	player.setPosition(newPosition);
}

sf::Vector2f CommandInterpreter::getTeleportPositionFromCommand() const
{
	const std::string numbers("1234567890");

	if(mCommand.find_first_of(numbers) == std::string::npos)
		return handleTeleportArgumentError();

	size_t xArgumentPositionInCommand = mCommand.find_first_of(numbers);
	if(xArgumentPositionInCommand != std::string::npos && xArgumentPositionInCommand > 9)
		return handleTeleportArgumentError();
	size_t xArgumentEndPositionInCommand = mCommand.find(' ', xArgumentPositionInCommand);
	size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = mCommand.substr(xArgumentPositionInCommand, xArgumentLength);
	float positionX = std::strtof(xArgument.c_str(), nullptr);

	size_t yArgumentPositionInCommand = mCommand.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
	if(yArgumentPositionInCommand == std::string::npos)
		return handleTeleportArgumentError();
	size_t yArgumentEndPositionInCommand = mCommand.find_first_not_of(numbers, yArgumentPositionInCommand);
	size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	std::string yArgument = mCommand.substr(yArgumentPositionInCommand, yArgumentLength);
	float positionY = std::strtof(yArgument.c_str(), nullptr);

	return sf::Vector2f(positionX, positionY);
}

sf::Vector2f CommandInterpreter::handleTeleportArgumentError() const
{
	PH_LOG(LogType::Error, "Incorrect argument! Argument has to be a number.");
	return getPlayer().getPosition();
}

void CommandInterpreter::executeCurrentPos()
{
	auto& playerPosition = getPlayer().getPosition();
	PH_LOG(LogType::Info, "player position: " + Cast::toString(playerPosition));
}

auto CommandInterpreter::getPlayer() const -> Object&
{
	auto& gameState = mGameData->getStateMachine().getTopState();
	auto& root = gameState.getRoot();
	Object& player = dynamic_cast<Object&>(root.getChild("player"));
	return player;
}

void CommandInterpreter::executeCollisionDebug()
{
	if(commandContains("turn"))          turnOnOrTurnOffCollisionDebug();
	else if(commandContains("color"))    changeCollisionDebugColor();
	else if(commandContains("display"))  changeCollisionDebugDisplayMode();
	else
		PH_LOG(LogType::Error, "Incorrect argument! First argument has to be 'turn', 'color' or 'display'.");
}

void CommandInterpreter::turnOnOrTurnOffCollisionDebug()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	size_t endOfCommand = getCommandWithoutArguments().size();
	if (mCommand.rfind("on") > endOfCommand)
		collisionDebugSettings.turnOn();
	else if (commandContains("off"))
		collisionDebugSettings.turnOff();
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Enter 'on' or 'off' to turn on/off collision debug.");
}

void CommandInterpreter::changeCollisionDebugColor()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains('1'))       collisionDebugSettings.setColors(1);
	else if(commandContains('2'))  collisionDebugSettings.setColors(2);
	else if(commandContains('3'))  collisionDebugSettings.setColors(3);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You can set collision debug color only from 1 to 3.");
}

void CommandInterpreter::changeCollisionDebugDisplayMode()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains("kinematic"))    collisionDebugSettings.displayOnlyKinematicBodies();
	else if(commandContains("static"))  collisionDebugSettings.displayOnlyStaticBodies();
	else if(commandContains("all"))     collisionDebugSettings.displayAllBodies();
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You have to enter 'kinematic', 'static' or 'all'.");
}

void CommandInterpreter::executeMute()
{
	setAudioMuted(true);
}

void CommandInterpreter::executeUnmute()
{
	setAudioMuted(false);
}

void CommandInterpreter::setAudioMuted(bool mute)
{
	if(commandContains("music"))
		mGameData->getMusicPlayer().setMuted(mute);
	else if(commandContains("sound"))
		mGameData->getSoundPlayer().setMuted(mute);
	else if(commandContains("all")) {
		mGameData->getMusicPlayer().setMuted(mute);
		mGameData->getSoundPlayer().setMuted(mute);
	}
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You have to enter 'music', 'sound' or 'all'.");
}

void CommandInterpreter::executeSetVolume()
{
	float newVolume = getVolumeFromCommand();
	if(!(commandContains('0')) && newVolume == 0 || newVolume > 100){
		PH_LOG(LogType::Error, "Incorrect volume value! Enter value from 0 to 100");
		return;
	}

	if (commandContains("music"))
		mGameData->getMusicPlayer().setVolume(newVolume);
	else if (commandContains("sound"))
		mGameData->getSoundPlayer().setVolume(newVolume);
	else{
		mGameData->getMusicPlayer().setVolume(newVolume);
		mGameData->getSoundPlayer().setVolume(newVolume);
	}
}

float CommandInterpreter::getVolumeFromCommand()
{
	size_t spacePosition = mCommand.find_last_of(' ');
	size_t valueStartPos = spacePosition + 1;
	size_t valueLength = mCommand.size() - valueStartPos;
	std::string volumeValue = mCommand.substr(valueStartPos, valueLength);
	return std::strtof(volumeValue.c_str(), nullptr);
}

void CommandInterpreter::executeLog()
{
	if (commandContains("into"))          logInto();
	else if (commandContains("types"))    setLogTypesToLog();
	else if (commandContains("modules"))  setModulesToLog();
	else
		PH_LOG(LogType::Error, "Incorrect first argument! Enter 'into' 'types' or 'modules'.");
}

void CommandInterpreter::logInto()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	int newValue = commandContains("not") ? false : true;
	if (commandContains("console") || commandContains("all"))
		logSettings.setWritingLogsIntoConsole(newValue);
	else if (commandContains("file") || commandContains("all"))
		logSettings.setWritingLogsIntoFile(newValue);
	else if (commandContains("terminal") || commandContains("all"))
		logSettings.setWritingLogsIntoTerminal(newValue);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Enter 'console', 'file', 'terminal' or 'all'.");
}

void CommandInterpreter::setLogTypesToLog()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	if(commandContains("info"))     logSettings.addToVector(LogType::Info);
	if(commandContains("warning"))  logSettings.addToVector(LogType::Warning);
	if(commandContains("error"))    logSettings.addToVector(LogType::Error);
	if(commandContains("user"))     logSettings.addToVector(LogType::FromUser);

	if(commandContains("all"))			logSettings.turnOnWritingLogsFromEachLogTypes();
	else if(commandContains("clear")) 	logSettings.setLogTypesToWrite({LogType::Exception, LogType::UnhandledException});

	if(areArgumentsToLogTypesToLogInvalid())
		PH_LOG(LogType::Error, "Incorrect 2nd argument! Use one of log types or 'all'/'clear'.");
}

bool CommandInterpreter::areArgumentsToLogTypesToLogInvalid()
{
	return(!(
		commandContains("info") || commandContains("warning") ||
		commandContains("error") || commandContains("user") ||
		commandContains("all") || commandContains("clear")
	));
}

void CommandInterpreter::setModulesToLog()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	if(commandContains("audio"))      logSettings.addToVector("Audio");
	if(commandContains("base"))       logSettings.addToVector("Base");
	if(commandContains("input"))      logSettings.addToVector("Input");
	if(commandContains("logs"))       logSettings.addToVector("Logs");
	if(commandContains("physics"))    logSettings.addToVector("Physics");
	if(commandContains("renderer"))   logSettings.addToVector("Renderer");
	if(commandContains("resources"))  logSettings.addToVector("Resources");
	if(commandContains("states"))     logSettings.addToVector("States");
	if(commandContains("utilities"))  logSettings.addToVector("Utilities");
	if(commandContains("world"))      logSettings.addToVector("World");
	if(commandContains("terminal"))   logSettings.addToVector("Terminal");
	if(commandContains("none"))       logSettings.addToVector("None");
	
	if(commandContains("all"))			logSettings.turnOnWritingLogsFromEachModule();
	else if (commandContains("clear"))	logSettings.setModuleNamesToWrite({});
	if(areArgumentsToModulesToLogInvalid())
		PH_LOG(LogType::Error, "Incorrect second argument! Use one of modules or 'all'/'clear'.");
}

bool CommandInterpreter::areArgumentsToModulesToLogInvalid()
{
	return(!(commandContains("audio") || commandContains("base") || commandContains("input") ||
		commandContains("logs") || commandContains("physics") || commandContains("renderer") ||
		commandContains("resources") || commandContains("states") || commandContains("utilities") ||
		commandContains("world") || commandContains("terminal") || commandContains("none") ||
		commandContains("all") || commandContains("clear")
	));
}

bool CommandInterpreter::commandContains(const char c)
{
	return mCommand.find(c) != std::string::npos;
}

bool CommandInterpreter::commandContains(const char* c)
{
	return mCommand.find(c) != std::string::npos;
}

}