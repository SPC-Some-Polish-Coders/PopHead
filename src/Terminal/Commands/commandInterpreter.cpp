#include "commandInterpreter.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Terminal/terminal.hpp"
#include "Logs/logs.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/spawn.hpp"
#include "gameData.hpp"
#include "Map/map.hpp"

namespace ph {

void CommandInterpreter::init()
{
	mCommandsMap["echo"] =		&CommandInterpreter::executeEcho;
	mCommandsMap["exit"] =		&CommandInterpreter::executeExit;
	mCommandsMap["teleport"] =	&CommandInterpreter::executeTeleport;
	mCommandsMap["currentpos"] =	&CommandInterpreter::executeCurrentPos;
	mCommandsMap["collisiondebug"] =&CommandInterpreter::executeCollisionDebug;
	mCommandsMap["mute"] =		&CommandInterpreter::executeMute;
	mCommandsMap["unmute"] =	&CommandInterpreter::executeUnmute;
	mCommandsMap["setvolume"] =	&CommandInterpreter::executeSetVolume;
	mCommandsMap["history"] =	&CommandInterpreter::executeHistory;
	mCommandsMap["help"] =		&CommandInterpreter::executeHelp;
	mCommandsMap["clear"] =		&CommandInterpreter::executeClear;
	mCommandsMap["view"] =		&CommandInterpreter::executeView;
	mCommandsMap["spawn"] =		&CommandInterpreter::executeSpawn;
	mCommandsMap["gotoscene"] =	&CommandInterpreter::executeGotoScene;
	mCommandsMap[""] =			&CommandInterpreter::executeInfoMessage;
}

void CommandInterpreter::handleCommand(const std::string& command)
{
	mCommand = command;
	const std::string commandWithoutArguments = getCommandWithoutArguments();

	if (mCommandsMap.count(commandWithoutArguments))
		(this->*mCommandsMap.at(commandWithoutArguments))(); 
	else executeMessage("Entered command wasn't recognised. Enter 'help' to see available commands.", MessageType::ERROR);
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

void CommandInterpreter::executeInfoMessage() const
{
	executeMessage("This is terminal. Enter 'help' to see available commands.", MessageType::INFO);
}

void CommandInterpreter::executeEcho() const
{
	const size_t spacePosition = mCommand.find(' ');
	const size_t messageStartPos = spacePosition + 1;
	const size_t messageLength = mCommand.size() - messageStartPos;
	const std::string message = mCommand.substr(messageStartPos, messageLength);
	executeMessage(message, MessageType::USER);
}

void CommandInterpreter::executeHistory() const
{
	auto& terminalData = mGameData->getTerminal();
	auto& commandsHistory = mGameData->getTerminal().getSharedData()->mLastCommands;
	std::deque<std::string>::reverse_iterator it = commandsHistory.rbegin();

	for (; it != commandsHistory.rend(); ++it)
		executeMessage("- " + *it, MessageType::INFO);
	executeMessage("Ten last used commands: ", MessageType::INFO);
}

void CommandInterpreter::executeHelp() const
{
	const std::vector<std::string> commandsList1 {
		"EXIT", "ECHO", "HISTORY", "HELP", "MUTE", "UNMUTE", 
		"SETVOLUME", "TELEPORT"
	};
	const std::vector<std::string> commandsList2{
		"CURRENTPOS", "COLLISIONDEBUG", "SPAWN", "VIEW"
	};

	if (commandContains('2')){
		for (const auto& command : commandsList2)
			executeMessage("- " + command, MessageType::INFO);
		executeMessage("Available commands, PAGE 2 of 2.", MessageType::INFO);
	}
	else{
		for (const auto& command : commandsList1)
			executeMessage("- " + command, MessageType::INFO);
		executeMessage("Available commands, PAGE 1 of 2.", MessageType::INFO);
	}
}

void CommandInterpreter::executeGotoScene() const
{
	const int spacePosition = mCommand.find_first_of(' ') + 1;
	const std::string scenePath = "scenes/" + mCommand.substr(spacePosition, mCommand.size()) + ".xml";
	mGameData->getSceneManager().replaceScene(scenePath);
}

void CommandInterpreter::executeSpawn() const
{
	//TODO: Handle possible improper ObjectType
	Spawn(mGameData, Cast::toObjectType(mCommand), getVector2Argument());
}

void CommandInterpreter::executeClear() const
{
	for (int i = 0; i < 20; ++i)
		executeMessage("", MessageType::BLANK);
}

void CommandInterpreter::executeExit() const
{
	mGameData->getGameCloser().closeGame();
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
	executeMessage("player position: " + Cast::toString(playerPosition), MessageType::INFO);
}

auto CommandInterpreter::getPlayer() const -> GameObject&
{
	auto& gameScene = mGameData->getSceneManager().getScene();
	auto& root = gameScene.getRoot();
	GameObject& player = *root.getChild("player");
	return player;
}

void CommandInterpreter::executeCollisionDebug() const
{
	if(commandContains("turn"))          turnOnOrTurnOffCollisionDebug();
	else if(commandContains("color"))    changeCollisionDebugColor();
	else if(commandContains("display"))  changeCollisionDebugDisplayMode();
	else
		executeMessage("Incorrect argument! First argument has to be 'turn', 'color' or 'display'.", MessageType::ERROR);
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
		executeMessage("Incorrect second argument! Enter 'on' or 'off' to turn on/off collision debug.", MessageType::ERROR);
}

void CommandInterpreter::changeCollisionDebugColor() const
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains('1'))       collisionDebugSettings.setColors(1);
	else if(commandContains('2'))  collisionDebugSettings.setColors(2);
	else if(commandContains('3'))  collisionDebugSettings.setColors(3);
	else
		executeMessage("Incorrect second argument! You can set collision debug color only from 1 to 3.", MessageType::ERROR);
}

void CommandInterpreter::changeCollisionDebugDisplayMode() const
{
	auto& collisionDebugSettings = CollisionDebugSettings::getInstance();

	if(commandContains("kinematic"))    collisionDebugSettings.displayOnlyKinematicBodies();
	else if(commandContains("static"))  collisionDebugSettings.displayOnlyStaticBodies();
	else if(commandContains("all"))     collisionDebugSettings.displayAllBodies();
	else
		executeMessage("Incorrect second argument! You have to enter 'kinematic', 'static' or 'all'.", MessageType::ERROR);
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
		executeMessage("Incorrect second argument! You have to enter 'music', 'sound' or 'all'.", MessageType::ERROR);
}

void CommandInterpreter::executeSetVolume() const
{
	const float newVolume = getVolumeFromCommand();
	if(!(commandContains('0')) && newVolume == 0 || newVolume > 100){
		executeMessage("Incorrect volume value! Enter value from 0 to 100", MessageType::ERROR);
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
	auto& renderer = mGameData->getRenderer();
	renderer.setDebugRenderingMode(commandContains("debugmode"));
}

auto CommandInterpreter::getVector2Argument() const -> sf::Vector2f
{
	const std::string numbers("1234567890");

	if(mCommand.find_first_of(numbers) == std::string::npos)
		return handleGetVector2ArgumentError();

	const size_t xArgumentPositionInCommand = mCommand.find_first_of(numbers);
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
	executeMessage("Incorrect argument! Argument has to be a number.", MessageType::ERROR);
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

void CommandInterpreter::executeMessage(const std::string& message, const MessageType colorType) const
{
	sf::Color color;
	switch (colorType)
	{
	case MessageType::ERROR:
		color = { 255, 25, 33 };
		break;
	case MessageType::INFO:
		color = { 127, 244, 44 };
		break;
	case MessageType::USER:
		color = sf::Color(79, 202, 255);
		break;
	case MessageType::BLANK:
		color = sf::Color::Transparent;
		break;
	}

	mGameData->getTerminal().pushOutputLine(OutputLine{ message, color });
}

}
