#include "commandInterpreter.hpp"

#include "Utilities/debug.hpp"
#include "gameData.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Logs/logger.hpp"
#include "Utilities/cast.hpp"

void ph::CommandInterpreter::handleCommand(const std::string& command)
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
	else if(commandWithoutArguments == "") PH_LOG(LogType::Info, "This is terminal. Enter 'help' to see availible commands.");
	else PH_LOG(LogType::Error, "Entered command wasn't recognised. Enter 'help' to see availible commands.");
}

std::string ph::CommandInterpreter::getCommandWithoutArguments()
{
	return mCommand.substr(0, getArgumentPositionInCommand());
}

int ph::CommandInterpreter::getArgumentPositionInCommand()
{
	size_t argumentPosition = mCommand.find(' ');
	return argumentPosition == std::string::npos ? mCommand.size() : argumentPosition;
}

void ph::CommandInterpreter::executeEcho()
{
	size_t spacePosition = mCommand.find(' ');
	size_t messageStartPos = spacePosition + 1;
	size_t messageLength = mCommand.size() - messageStartPos;
	std::string message = mCommand.substr(messageStartPos, messageLength);
	PH_LOG(LogType::FromUser, message);
}

void ph::CommandInterpreter::executeHistory()
{
	auto historyData = mGameData->getTerminal().getSharedData()->mLastCommands;
	std::deque<std::string>::reverse_iterator it = historyData.rbegin();

	for (; it != historyData.rend(); ++it)
		mGameData->getTerminal().pushOutputLine({ "- " + *it, sf::Color(127, 244, 44) });
	mGameData->getTerminal().pushOutputLine({ "Ten last used commands: ",sf::Color(127, 244, 44) });
}

void ph::CommandInterpreter::executeHelp()
{
	std::vector<std::string> commandsList = { "EXIT", "ECHO", "HISTORY", 
							"HELP", "MUTE", "UNMUTE", "SETVOLUME",
							"TELEPORT","CURRENTPOS", "LOG", "COLLISIONDEBUG"};
	int i = 0;

	if (commandContains('1'))
	{
		i = 1;
		for (int i = 0; i < commandsList.size(); ++i)
			mGameData->getTerminal().pushOutputLine({ "- " + commandsList[i], sf::Color(127, 244, 44) });
	}
	else if (commandContains('2'))
	{
		i = 2;
		mGameData->getTerminal().pushOutputLine({ "Will be fulfilled if necessary" + commandsList[i], sf::Color(127, 244, 44) });
	}
	else
	{
		i = 1;
		for (int i = 0; i < commandsList.size(); ++i)
			mGameData->getTerminal().pushOutputLine({ "- " + commandsList[i], sf::Color(127, 244, 44) });
	}

	mGameData->getTerminal().pushOutputLine({"Avalible commands, PAGE "+ std::to_string(i) + ": ",sf::Color(127, 244, 44) });
}

void ph::CommandInterpreter::executeExit()
{
	mGameData->getRenderer().getWindow().close();
}

void ph::CommandInterpreter::executeTeleport()
{
	auto& player = getPlayer();
	sf::Vector2f newPosition = getTeleportPositionFromCommand();
	player.setPosition(newPosition);
}

sf::Vector2f ph::CommandInterpreter::getTeleportPositionFromCommand() const
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

sf::Vector2f ph::CommandInterpreter::handleTeleportArgumentError() const
{
	PH_LOG(LogType::Error, "Incorrect argument! Argument has to be a number.");
	return getPlayer().getPosition();
}

void ph::CommandInterpreter::executeCurrentPos()
{
	auto& playerPosition = getPlayer().getPosition();
	PH_LOG(LogType::Info, "player position: " + Cast::toString(playerPosition));
}

auto ph::CommandInterpreter::getPlayer() const -> Object&
{
	auto& gameState = mGameData->getStateMachine().getTopState();
	auto& root = gameState.getRoot();
	Object& player = dynamic_cast<Object&>(root.getChild("player"));
	return player;
}

void ph::CommandInterpreter::executeCollisionDebug()
{
	if(commandContains("turn"))          turnOnOrTurnOffCollisionDebug();
	else if(commandContains("color"))    changeCollisionDebugColor();
	else if(commandContains("display"))  changeCollisionDebugDisplayMode();
	else
		PH_LOG(LogType::Error, "Incorrect argument! First argument has to be 'turn', 'color' or 'display'.");
}

void ph::CommandInterpreter::turnOnOrTurnOffCollisionDebug()
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

void ph::CommandInterpreter::changeCollisionDebugColor()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains('1'))       collisionDebugSettings.setColors(1);
	else if(commandContains('2'))  collisionDebugSettings.setColors(2);
	else if(commandContains('3'))  collisionDebugSettings.setColors(3);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You can set collision debug color only from 1 to 3.");
}

void ph::CommandInterpreter::changeCollisionDebugDisplayMode()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains("kinematic"))    collisionDebugSettings.displayOnlyKinematicBodies();
	else if(commandContains("static"))  collisionDebugSettings.displayOnlyStaticBodies();
	else if(commandContains("all"))     collisionDebugSettings.displayAllBodies();
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You have to enter 'kinematic', 'static' or 'all'.");
}

void ph::CommandInterpreter::executeMute()
{
	setAudioMuted(true);
}

void ph::CommandInterpreter::executeUnmute()
{
	setAudioMuted(false);
}

void ph::CommandInterpreter::setAudioMuted(bool mute)
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

void ph::CommandInterpreter::executeSetVolume()
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

float ph::CommandInterpreter::getVolumeFromCommand()
{
	size_t spacePosition = mCommand.find_last_of(' ');
	size_t valueStartPos = spacePosition + 1;
	size_t valueLength = mCommand.size() - valueStartPos;
	std::string volumeValue = mCommand.substr(valueStartPos, valueLength);
	return std::strtof(volumeValue.c_str(), nullptr);
}

void ph::CommandInterpreter::executeLog()
{
	if (commandContains("into"))          logInto();
	else if (commandContains("types"))    setLogTypesToLog();
	else if (commandContains("modules"))  setModulesToLog();
	else
		PH_LOG(LogType::Error, "Incorrect first argument! Enter 'into' 'types' or 'modules'.");
}

void ph::CommandInterpreter::logInto()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	int newValue = commandContains("not") ? false : true;
	if (commandContains("console") || commandContains("both"))
		logSettings.setWritingLogsIntoConsole(newValue);
	else if (commandContains("file") || commandContains("both"))
		logSettings.setWritingLogsIntoFile(newValue);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Enter 'console', 'file' or 'both'.");
}

void ph::CommandInterpreter::setLogTypesToLog()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	if(commandContains("info"))     logSettings.addToVector(LogType::Info);
	if(commandContains("warning"))  logSettings.addToVector(LogType::Warning);
	if(commandContains("error"))    logSettings.addToVector(LogType::Error);
	if(commandContains("user"))     logSettings.addToVector(LogType::FromUser);

	if(commandContains("all"))			logSettings.turnOnWritingLogsFromEachLogTypes();
	else if(commandContains("clear")) 	logSettings.setLogTypesToWrite({});

	if(areArgumentsToLogTypesToLogInvalid())
		PH_LOG(LogType::Error, "Incorrect 2nd argument! Enter: 'info','warning','error','user','all' or 'clear'.");
}

bool ph::CommandInterpreter::areArgumentsToLogTypesToLogInvalid()
{
	return(!(
		commandContains("info") || commandContains("warning") ||
		commandContains("error") || commandContains("user") ||
		commandContains("all") || commandContains("clear")
	));
}

void ph::CommandInterpreter::setModulesToLog()
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

bool ph::CommandInterpreter::areArgumentsToModulesToLogInvalid()
{
	return(!(commandContains("audio") || commandContains("base") || commandContains("input") ||
		commandContains("logs") || commandContains("physics") || commandContains("renderer") ||
		commandContains("resources") || commandContains("states") || commandContains("utilities") ||
		commandContains("world") || commandContains("terminal") || commandContains("none") ||
		commandContains("all") || commandContains("clear")
	));
}

bool ph::CommandInterpreter::commandContains(const char c)
{
	return mCommand.find(c) != std::string::npos;
}

bool ph::CommandInterpreter::commandContains(const char* c)
{
	return mCommand.find(c) != std::string::npos;
}
