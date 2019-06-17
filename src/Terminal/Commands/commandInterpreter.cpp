#include "commandInterpreter.hpp"

#include "Utilities/debug.hpp"
#include "Base/gameData.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Logs/logger.hpp"

void ph::CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;

	const std::string commandWithoutArguments = getCommandWithoutArguments();

	if		(commandWithoutArguments == "echo")            executeEcho();
	else if (commandWithoutArguments == "exit")            executeExit();
	else if (commandWithoutArguments == "teleport")        executeTeleport();
	else if (commandWithoutArguments == "currentpos")      executeCurrentPos();
	else if (commandWithoutArguments == "collisiondebug")  executeCollisionDebug();
	else if (commandWithoutArguments == "mute")            executeMute();
	else if (commandWithoutArguments == "unmute")          executeUnmute();
	else if (commandWithoutArguments == "setvolume")       executeSetVolume();
	else if (commandWithoutArguments == "log")             executeLog();
	//else PH_LOG(LogType::Error, "Entered command is not recognised. Make sure if it's correct.");
	//weird loop occures when the program starts. Problem disappears when we run the terminal.
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
	PH_LOG(LogType::Info, "Teleported! Player's position set to " + std::to_string(newPosition.x) + 
		" X and " + std::to_string(newPosition.y) + " Y.");
}

sf::Vector2f ph::CommandInterpreter::getPositionFromCommand() const
{
	const std::string numbers("1234567890");

	size_t xArgumentPositionInCommand = mCommand.find_first_of(numbers);
	size_t xArgumentEndPositionInCommand = mCommand.find(' ', xArgumentPositionInCommand);
	size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = mCommand.substr(xArgumentPositionInCommand, xArgumentLength);
	float positionX = std::stof(xArgument);

	size_t yArgumentPositionInCommand = mCommand.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
	size_t yArgumentEndPositionInCommand = mCommand.find_first_not_of(numbers, yArgumentPositionInCommand);
	if(yArgumentEndPositionInCommand == std::string::npos)
		yArgumentEndPositionInCommand = mCommand.size();
	size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	std::string yArgument = mCommand.substr(yArgumentPositionInCommand, yArgumentLength);
	float positionY = std::stof(yArgument);
	return sf::Vector2f(positionX, positionY);
}

void ph::CommandInterpreter::executeCurrentPos()
{
	auto& player = getPlayer();
	std::string x = std::to_string(player.getPosition().x);
	std::string y = std::to_string(player.getPosition().y);
	PH_LOG(LogType::Info, "player position:  x: " + x + "  y:" + y);
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
	if(commandContains("turn")) 			turnOnOrTurnOffCollisionDebug();
	else if(commandContains("color"))		changeCollisionDebugColor();
	else if(commandContains("display"))		changeCollisionDebugDisplayMode();
}

void ph::CommandInterpreter::turnOnOrTurnOffCollisionDebug()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	//if(commandContains("off")) must be first! Do not change the order!
	if (commandContains("off"))
	{
		collisionDebugSettings.turnOff();
		PH_LOG(LogType::Info, "Collision debug mode is turned off.");
	}
	else if (commandContains("on"))
	{
		collisionDebugSettings.turnOn();
		PH_LOG(LogType::Info, "Collision debug mode is turned on.");
	}
}

void ph::CommandInterpreter::changeCollisionDebugColor()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if (commandContains('1'))
	{ 
		collisionDebugSettings.setColors(1);
		PH_LOG(LogType::Info, "Collision debug color mode is set to 1.");
	}
	else if (commandContains('2'))
	{
		collisionDebugSettings.setColors(2);
		PH_LOG(LogType::Info, "Collision debug color mode is set to 2.");
	}
	else if(commandContains('3'))	
	{
		collisionDebugSettings.setColors(3);
		PH_LOG(LogType::Info, "Collision debug color mode is set to 3.");
	}
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You can set collision debug color only from 1 to 3.");
}

void ph::CommandInterpreter::changeCollisionDebugDisplayMode()
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if (commandContains("kinematic"))
	{
		collisionDebugSettings.displayOnlyKinematicBodies();
		PH_LOG(LogType::Info, "Collition debug display mode is set to kinematic bodies.");
	}
	else if (commandContains("static"))
	{
		collisionDebugSettings.displayOnlyStaticBodies();
		PH_LOG(LogType::Info, "Collition debug display mode is set to static bodies.");
	}
	else if (commandContains("all"))
	{
		collisionDebugSettings.displayAllBodies();
		PH_LOG(LogType::Info, "Collition debug display mode is set to all bodies.");
	}
	else	PH_LOG(LogType::Error, "Incorrect second argument! Display mode not found.");
}

void ph::CommandInterpreter::executeMute()
{
	if (commandContains("music"))
	{
		mGameData->getMusicPlayer().setMuted(true);
		PH_LOG(LogType::Info, "Muted music.");
	}
	else if (commandContains("sound"))
	{
		mGameData->getSoundPlayer().setMuted(true);
		PH_LOG(LogType::Info, "Muted sounds.");
	}
	else if (commandContains("all"))
	{
		mGameData->getSoundPlayer().setMuted(true);
		mGameData->getMusicPlayer().setMuted(true);
		PH_LOG(LogType::Info, "Muted sounds and music.");
	}
	else 
		PH_LOG(LogType::Error, "Incorrect second argument! Specified module not found.");
}

void ph::CommandInterpreter::executeUnmute()
{
	if (commandContains("music"))
	{
		mGameData->getMusicPlayer().setMuted(false);
		PH_LOG(LogType::Info, "Unmuted music.");
	}
	else if(commandContains("sound"))
	{
		mGameData->getSoundPlayer().setMuted(false);
		PH_LOG(LogType::Info, "Unmuted sounds.");
	}
	else if(commandContains("all"))
	{
		mGameData->getMusicPlayer().setMuted(false);
		mGameData->getSoundPlayer().setMuted(false);
		PH_LOG(LogType::Info, "Unmuted sounds and music.");
	}
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Specified module not found.");
}

void ph::CommandInterpreter::executeSetVolume()
{
	float newVolume = getVolumeFromCommand();

	if (commandContains("music"))
	{
		mGameData->getMusicPlayer().setVolume(newVolume);
		PH_LOG(LogType::Info, "Music volume is set to " + std::to_string(newVolume) + ".");
	}
	else if (commandContains("sound"))
	{
		mGameData->getSoundPlayer().setVolume(newVolume);
		PH_LOG(LogType::Info, "Sound volume is set to " + std::to_string(newVolume) + ".");
	}
	else{
		mGameData->getMusicPlayer().setVolume(newVolume);
		mGameData->getSoundPlayer().setVolume(newVolume);
		PH_LOG(LogType::Info, "Sound and music volume is set to " + std::to_string(newVolume) + ".");
	}
}

float ph::CommandInterpreter::getVolumeFromCommand()
{
	size_t spacePosition = mCommand.find_last_of(' ');
	size_t valueStartPos = spacePosition + 1;
	size_t valueLength = mCommand.size() - valueStartPos;
	std::string textToFloat = mCommand.substr(valueStartPos, valueLength);
	return std::strtof(textToFloat.c_str(), nullptr);
}

void ph::CommandInterpreter::executeLog()
{
	if (commandContains("into"))
		logInto();
	else if(commandContains("types"))
		setLogTypesToLog();
	else if(commandContains("modules"))
		setModulesToLog();
}

void ph::CommandInterpreter::logInto()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	int newValue = commandContains("not") ? false : true;
	if (commandContains("console") || commandContains("both"))
	{
		logSettings.setWritingLogsIntoConsole(newValue);
	}
	if (commandContains("file") || commandContains("both"))
	{
		logSettings.setWritingLogsIntoFile(newValue);
	}
}

void ph::CommandInterpreter::setLogTypesToLog()
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	if (commandContains("info"))     logSettings.addToVector(LogType::Info);
	if (commandContains("warning"))  logSettings.addToVector(LogType::Warning);
	if (commandContains("error"))    logSettings.addToVector(LogType::Error);
	if (commandContains("user"))     logSettings.addToVector(LogType::FromUser);

	if (commandContains("all"))		
		logSettings.turnOnWritingLogsFromEachLogTypes();
	else if (commandContains("clear"))	
		logSettings.setLogTypesToWrite({});
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

	if (commandContains("all"))
		logSettings.turnOnWritingLogsFromEachModule();
	else if (commandContains("clear"))
		logSettings.setModuleNamesToWrite({});
}

bool ph::CommandInterpreter::commandContains(const char c)
{
	return mCommand.find(c) != std::string::npos;
}

bool ph::CommandInterpreter::commandContains(const char* c)
{
	return mCommand.find(c) != std::string::npos;
}
