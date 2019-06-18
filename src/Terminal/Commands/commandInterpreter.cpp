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

	if (commandWithoutArguments == "echo")				   executeEcho();
	else if (commandWithoutArguments == "exit")            executeExit();
	else if (commandWithoutArguments == "teleport")        executeTeleport();
	else if (commandWithoutArguments == "currentpos")      executeCurrentPos();
	else if (commandWithoutArguments == "collisiondebug")  executeCollisionDebug();
	else if (commandWithoutArguments == "mute")            executeMute();
	else if (commandWithoutArguments == "unmute")          executeUnmute();
	else if (commandWithoutArguments == "setvolume")       executeSetVolume();
	else if (commandWithoutArguments == "log")             executeLog();
	else PH_LOG(LogType::Error, "Entered command is not recognised. Use 'help' in order to get availible commands.");
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

void ph::CommandInterpreter::executeExit()
{
	mGameData->getRenderer().getWindow().close();
}

void ph::CommandInterpreter::executeTeleport()
{
	auto& player = getPlayer();
	sf::Vector2f newPosition = getPositionFromCommand();
	player.setPosition(newPosition);
}

sf::Vector2f ph::CommandInterpreter::getPositionFromCommand() const
{
	const std::string numbers("1234567890");

	if (mCommand.find_first_of(numbers) == std::string::npos)
	{
		PH_LOG(LogType::Error, "Incorrect argument! Use only digits from 0 to 9");
		return getPlayer().getPosition();
	}

	size_t xArgumentPositionInCommand = mCommand.find_first_of(numbers);
		if (xArgumentPositionInCommand != std::string::npos && xArgumentPositionInCommand > 9)
		{
			PH_LOG(LogType::Error, "Incorrect argument! Argument of X position is not a digit.");
			return sf::Vector2f(getPlayer().getPosition());
		}
	size_t xArgumentEndPositionInCommand = mCommand.find(' ', xArgumentPositionInCommand);
	size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = mCommand.substr(xArgumentPositionInCommand, xArgumentLength);
	float positionX = std::strtof(xArgument.c_str(), nullptr);

	size_t yArgumentPositionInCommand = mCommand.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
		if (yArgumentPositionInCommand == std::string::npos)
		{
			PH_LOG(LogType::Error, "Incorrect argument! Argument of Y position is not a digit.");
			return sf::Vector2f(getPlayer().getPosition());
		}
	size_t yArgumentEndPositionInCommand = mCommand.find_first_not_of(numbers, yArgumentPositionInCommand);
	size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	std::string yArgument = mCommand.substr(yArgumentPositionInCommand, yArgumentLength);
	float positionY = std::strtof(yArgument.c_str(), nullptr);
	return sf::Vector2f(positionX, positionY);
}

void ph::CommandInterpreter::executeCurrentPos()
{
	auto& player = getPlayer().getPosition();
	PH_LOG(LogType::Info, "player position: " + Cast::toString(player));
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
	if (commandContains("turn")) 			turnOnOrTurnOffCollisionDebug();
	else if (commandContains("color"))		changeCollisionDebugColor();
	else if (commandContains("display"))	changeCollisionDebugDisplayMode();
	else
		PH_LOG(LogType::Error, "Incorrect argument! Use 'turn', 'color' or 'display' to set collision debugging.");
}

void ph::CommandInterpreter::turnOnOrTurnOffCollisionDebug()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();
	size_t endOfCommand = getCommandWithoutArguments().size();

	if (mCommand.rfind("on") > endOfCommand)	collisionDebugSettings.turnOn();
	else if (mCommand.find("off"))			collisionDebugSettings.turnOff();
	else
		PH_LOG(LogType::Error, "Incorrect argument! Use 'on' or 'off' to switch between modes.");
}

void ph::CommandInterpreter::changeCollisionDebugColor()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if (commandContains('1'))		collisionDebugSettings.setColors(1);
	else if (commandContains('2'))	collisionDebugSettings.setColors(2);
	else if(commandContains('3'))	collisionDebugSettings.setColors(3);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You can set collision debug color only from 1 to 3.");
}

void ph::CommandInterpreter::changeCollisionDebugDisplayMode()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if (commandContains("kinematic"))		collisionDebugSettings.displayOnlyKinematicBodies();
	else if (commandContains("static"))		collisionDebugSettings.displayOnlyStaticBodies();
	else if (commandContains("all"))		collisionDebugSettings.displayAllBodies();
	else	
		PH_LOG(LogType::Error, "Incorrect second argument! Display mode not found.");
}

void ph::CommandInterpreter::executeMute()
{
	if (commandContains("music"))		mGameData->getMusicPlayer().setMuted(true);
	else if (commandContains("sound"))		mGameData->getSoundPlayer().setMuted(true);
	else if (commandContains("all"))
	{
		mGameData->getSoundPlayer().setMuted(true);
		mGameData->getMusicPlayer().setMuted(true);
	}
	else 
		PH_LOG(LogType::Error, "Incorrect second argument! Specified module not found.");
}

void ph::CommandInterpreter::executeUnmute()
{
	if (commandContains("music"))		mGameData->getMusicPlayer().setMuted(false);
	else if(commandContains("sound"))	mGameData->getSoundPlayer().setMuted(false);
	else if(commandContains("all"))
	{
		mGameData->getMusicPlayer().setMuted(false);
		mGameData->getSoundPlayer().setMuted(false);
	}
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Specified module not found.");
}

void ph::CommandInterpreter::executeSetVolume()
{
	float newVolume = getVolumeFromCommand();
	if (!(commandContains("0")) && newVolume == 0 || newVolume > 100)
	{
		PH_LOG(LogType::Error, "Incorrect volume value! Use values from 0 to 100");
		return;
	}

	if (commandContains("music"))			mGameData->getMusicPlayer().setVolume(newVolume);
	else if (commandContains("sound"))		mGameData->getSoundPlayer().setVolume(newVolume);
	else
	{
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
	if (commandContains("into"))			logInto();
	else if (commandContains("types"))		setLogTypesToLog();
	else if (commandContains("modules"))	setModulesToLog();
	else
		PH_LOG(LogType::Error, "Incorrect argument! Use 'into' 'types' or 'modules' to set logging.");
}

void ph::CommandInterpreter::logInto()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	int newValue = commandContains("not") ? false : true;
	if (commandContains("console") || commandContains("both"))
	{
		logSettings.setWritingLogsIntoConsole(newValue);
	}
	else if (commandContains("file") || commandContains("both"))
	{
		logSettings.setWritingLogsIntoFile(newValue);
	}
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Specified module not found!");
}

void ph::CommandInterpreter::setLogTypesToLog()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	if (commandContains("info"))     logSettings.addToVector(LogType::Info);
	if (commandContains("warning"))  logSettings.addToVector(LogType::Warning);
	if (commandContains("error"))    logSettings.addToVector(LogType::Error);
	if (commandContains("user"))     logSettings.addToVector(LogType::FromUser);

	if (commandContains("all"))			logSettings.turnOnWritingLogsFromEachLogTypes();
	else if (commandContains("clear")) 	logSettings.setLogTypesToWrite({});
	if (areArgumentsToLogTypesToLogInvalid())
		PH_LOG(LogType::Error, "Incorrect second argument! Use one of log types or 'all'/'clear'.");
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

	if (commandContains("audio"))       logSettings.addToVector("Audio");
	if (commandContains("base"))        logSettings.addToVector("Base");
	if (commandContains("input"))       logSettings.addToVector("Input");
	if (commandContains("logs"))        logSettings.addToVector("Logs");
	if (commandContains("physics"))     logSettings.addToVector("Physics");
	if (commandContains("renderer"))    logSettings.addToVector("Renderer");
	if (commandContains("resources"))   logSettings.addToVector("Resources");
	if (commandContains("states"))      logSettings.addToVector("States");
	if (commandContains("utilities"))   logSettings.addToVector("Utilities");
	if (commandContains("world"))       logSettings.addToVector("World");
	if (commandContains("terminal"))    logSettings.addToVector("Terminal");
	if (commandContains("none"))        logSettings.addToVector("None");
	
	if (commandContains("all"))			logSettings.turnOnWritingLogsFromEachModule();
	else if (commandContains("clear"))	logSettings.setModuleNamesToWrite({});
	if (areArgumentsToModulesToLogInvalid())
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
