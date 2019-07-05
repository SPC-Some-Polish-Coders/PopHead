#include "commandInterpreter.hpp"

#include "Utilities/debug.hpp"
#include "gameData.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Logs/logger.hpp"
#include "Utilities/cast.hpp"
#include "EntityComponentSystem/Objects/Map/map.hpp"

namespace ph {

void CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;

	const std::string commandWithoutArguments = getCommandWithoutArguments();

	if(commandWithoutArguments == "echo")                executeEcho();
	else if(commandWithoutArguments == "exit")           executeExit();
	else if(commandWithoutArguments == "teleport")       executeTeleport();
	else if(commandWithoutArguments == "currentpos")     executeCurrentPos();
	else if(commandWithoutArguments == "collisiondebug") executeCollisionDebug();
	else if(commandWithoutArguments == "mute")           executeMute();
	else if(commandWithoutArguments == "unmute")         executeUnmute();
	else if(commandWithoutArguments == "setvolume")      executeSetVolume();
	else if(commandWithoutArguments == "log")            executeLog();
	else if(commandWithoutArguments == "history")        executeHistory();
	else if(commandWithoutArguments == "help")           executeHelp();
	else if(commandWithoutArguments == "clear")          executeClear();
	else if(commandWithoutArguments == "view")           executeView();
	else if(commandWithoutArguments == "") PH_LOG(LogType::Info, "This is terminal. Enter 'help' to see availible commands.");
	else PH_LOG(LogType::Error, "Entered command wasn't recognised. Enter 'help' to see availible commands.");
}

std::string CommandInterpreter::getCommandWithoutArguments() const
{
	return mCommand.substr(0, getArgumentPositionInCommand());
}

int CommandInterpreter::getArgumentPositionInCommand() const
{
	const size_t argumentPosition = mCommand.find(' ');
	return argumentPosition == std::string::npos ? mCommand.size() : argumentPosition;
}

void CommandInterpreter::executeEcho() const
{
	const size_t spacePosition = mCommand.find(' ');
	const size_t messageStartPos = spacePosition + 1;
	const size_t messageLength = mCommand.size() - messageStartPos;
	const std::string message = mCommand.substr(messageStartPos, messageLength);
	PH_LOG(LogType::FromUser, message);
}

void CommandInterpreter::executeHistory() const
{
	auto& terminalData = mGameData->getTerminal();
	auto& commandsHistory = mGameData->getTerminal().getSharedData()->mLastCommands;
	std::deque<std::string>::reverse_iterator it = commandsHistory.rbegin();

	for (; it != commandsHistory.rend(); ++it)
		terminalData.pushOutputLine({ "- " + *it, sf::Color(127, 244, 44) });
	terminalData.pushOutputLine({ "Ten last used commands: ",sf::Color(127, 244, 44) });
}

void CommandInterpreter::executeHelp() const
{
	const std::vector<std::string> commandsList1 {
		"EXIT", "ECHO", "HISTORY", "HELP", "MUTE", "UNMUTE", "SETVOLUME",
		"TELEPORT","CURRENTPOS", "LOG", "COLLISIONDEBUG"
	};

	const sf::Color infoColor = {127, 244, 44};
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

void CommandInterpreter::executeClear() const
{
	for (int i = 0; i < 20; ++i)
		mGameData->getTerminal().pushOutputLine({"", sf::Color::Transparent});
}

void CommandInterpreter::executeExit() const
{
	mGameData->getGameCloser().closeTheGame();
}

void CommandInterpreter::executeTeleport() const
{
	auto& player = getPlayer();
	const sf::Vector2f newPosition = getVector2Argument();
	if(newPosition == mVector2ArgumentError)
		return;
	player.setPosition(newPosition);
}

void CommandInterpreter::executeCurrentPos() const
{
	const sf::Vector2f playerPosition = getPlayer().getPosition();
	PH_LOG(LogType::Info, "player position: " + Cast::toString(playerPosition));
}

auto CommandInterpreter::getPlayer() const -> Object&
{
	auto& gameState = mGameData->getStateMachine().getTopState();
	auto& root = gameState.getRoot();
	Object& player = dynamic_cast<Object&>(root.getChild("player"));
	return player;
}

void CommandInterpreter::executeCollisionDebug() const
{
	if(commandContains("turn"))          turnOnOrTurnOffCollisionDebug();
	else if(commandContains("color"))    changeCollisionDebugColor();
	else if(commandContains("display"))  changeCollisionDebugDisplayMode();
	else
		PH_LOG(LogType::Error, "Incorrect argument! First argument has to be 'turn', 'color' or 'display'.");
}

void CommandInterpreter::turnOnOrTurnOffCollisionDebug() const
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	const size_t endOfCommand = getCommandWithoutArguments().size();
	if (mCommand.rfind("on") > endOfCommand)
		collisionDebugSettings.turnOn();
	else if (commandContains("off"))
		collisionDebugSettings.turnOff();
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Enter 'on' or 'off' to turn on/off collision debug.");
}

void CommandInterpreter::changeCollisionDebugColor() const
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains('1'))       collisionDebugSettings.setColors(1);
	else if(commandContains('2'))  collisionDebugSettings.setColors(2);
	else if(commandContains('3'))  collisionDebugSettings.setColors(3);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You can set collision debug color only from 1 to 3.");
}

void CommandInterpreter::changeCollisionDebugDisplayMode() const
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains("kinematic"))    collisionDebugSettings.displayOnlyKinematicBodies();
	else if(commandContains("static"))  collisionDebugSettings.displayOnlyStaticBodies();
	else if(commandContains("all"))     collisionDebugSettings.displayAllBodies();
	else
		PH_LOG(LogType::Error, "Incorrect second argument! You have to enter 'kinematic', 'static' or 'all'.");
}

void CommandInterpreter::executeMute() const
{
	setAudioMuted(true);
}

void CommandInterpreter::executeUnmute() const
{
	setAudioMuted(false);
}

void CommandInterpreter::setAudioMuted(bool mute) const
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

void CommandInterpreter::executeSetVolume() const
{
	const float newVolume = getVolumeFromCommand();
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

float CommandInterpreter::getVolumeFromCommand() const
{
	const size_t spacePosition = mCommand.find_last_of(' ');
	const size_t valueStartPos = spacePosition + 1;
	const size_t valueLength = mCommand.size() - valueStartPos;
	const std::string volumeValue = mCommand.substr(valueStartPos, valueLength);
	return std::strtof(volumeValue.c_str(), nullptr);
}

void CommandInterpreter::executeLog() const
{
	if (commandContains("into"))          logInto();
	else if (commandContains("types"))    setLogTypesToLog();
	else if (commandContains("modules"))  setModulesToLog();
	else
		PH_LOG(LogType::Error, "Incorrect first argument! Enter 'into' 'types' or 'modules'.");
}

void CommandInterpreter::logInto() const
{
	auto& logSettings = Logger::getInstance().getLogSettings();

	const int newValue = commandContains("not") ? false : true;
	if (commandContains("console") || commandContains("all"))
		logSettings.setWritingLogsIntoConsole(newValue);
	else if (commandContains("file") || commandContains("all"))
		logSettings.setWritingLogsIntoFile(newValue);
	else if (commandContains("terminal") || commandContains("all"))
		logSettings.setWritingLogsIntoTerminal(newValue);
	else
		PH_LOG(LogType::Error, "Incorrect second argument! Enter 'console', 'file', 'terminal' or 'all'.");
}

void CommandInterpreter::setLogTypesToLog() const
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

bool CommandInterpreter::areArgumentsToLogTypesToLogInvalid() const
{
	return(!(
		commandContains("info") || commandContains("warning") ||
		commandContains("error") || commandContains("user") ||
		commandContains("all") || commandContains("clear")
	));
}

void CommandInterpreter::setModulesToLog() const
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

bool CommandInterpreter::areArgumentsToModulesToLogInvalid() const
{
	return(!(commandContains("audio") || commandContains("base") || commandContains("input") ||
		commandContains("logs") || commandContains("physics") || commandContains("renderer") ||
		commandContains("resources") || commandContains("states") || commandContains("utilities") ||
		commandContains("world") || commandContains("terminal") || commandContains("none") ||
		commandContains("all") || commandContains("clear")
	));
}

void CommandInterpreter::executeView() const
{
	auto& camera = mGameData->getRenderer().getCamera();
	if(commandContains("normal")) {
		camera.setSize({640, 480});
		return;
	}
	const sf::Vector2f newViewSize = getVector2Argument();
	if(newViewSize == mVector2ArgumentError)
		return;
	camera.setSize(newViewSize);
	auto& map = getMap();
	if(commandContains("chunkdebug"))
		map.setRenderChunksMode(RenderChunksMode::for640x480CameraView);
	else
		map.setRenderChunksMode(RenderChunksMode::forCurrentCameraView);
}

auto CommandInterpreter::getMap() const -> Map&
{
	auto& topState = mGameData->getStateMachine().getTopState();
	auto& root = topState.getRoot();
	auto& map = dynamic_cast<Map&>(root.getChild("map"));
	return map;
}

auto CommandInterpreter::getVector2Argument() const -> sf::Vector2f
{
	const std::string numbers("1234567890");

	if(mCommand.find_first_of(numbers) == std::string::npos)
		return handleGetVector2ArgumentError();

	const size_t xArgumentPositionInCommand = mCommand.find_first_of(numbers);
	if(xArgumentPositionInCommand != std::string::npos && xArgumentPositionInCommand > 9)
		return handleGetVector2ArgumentError();
	const size_t xArgumentEndPositionInCommand = mCommand.find(' ', xArgumentPositionInCommand);
	const size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = mCommand.substr(xArgumentPositionInCommand, xArgumentLength);
	const float positionX = std::strtof(xArgument.c_str(), nullptr);

	const size_t yArgumentPositionInCommand = mCommand.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
	if(yArgumentPositionInCommand == std::string::npos)
		return handleGetVector2ArgumentError();
	const size_t yArgumentEndPositionInCommand = mCommand.find_first_not_of(numbers, yArgumentPositionInCommand);
	const size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	const std::string yArgument = mCommand.substr(yArgumentPositionInCommand, yArgumentLength);
	const float positionY = std::strtof(yArgument.c_str(), nullptr);

	return sf::Vector2f(positionX, positionY);
}

sf::Vector2f CommandInterpreter::handleGetVector2ArgumentError() const
{
	PH_LOG(LogType::Error, "Incorrect argument! Argument has to be a number.");
	return mVector2ArgumentError;
}

bool CommandInterpreter::commandContains(const char c) const
{
	return mCommand.find(c) != std::string::npos;
}

bool CommandInterpreter::commandContains(const char* c) const
{
	return mCommand.find(c) != std::string::npos;
}

}