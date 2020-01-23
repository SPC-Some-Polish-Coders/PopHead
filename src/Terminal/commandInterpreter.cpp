#include "commandInterpreter.hpp"
#include "Audio/Sound/soundData.hpp"
#include "Terminal/terminal.hpp"
#include "Logs/logs.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/xml.hpp"
#include "gameData.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Systems/areasDebug.hpp"
#include "Renderer/MinorRenderers/lightRenderer.hpp"
#include "Renderer/API/font.hpp"
#include "GUI/xmlGuiParser.hpp"
#include <entt/entt.hpp>

namespace ph {

void CommandInterpreter::init()
{
	mCommandsMap["echo"] =						&CommandInterpreter::executeEcho;
	mCommandsMap["exit"] =						&CommandInterpreter::executeExit;
	mCommandsMap["teleport"] =					&CommandInterpreter::executeTeleport;
	mCommandsMap["give"] =	    				&CommandInterpreter::executeGive;
	mCommandsMap["currentpos"] =				&CommandInterpreter::executeCurrentPos;
	mCommandsMap["collisiondebug"] =			&CommandInterpreter::executeCollisionDebug;
	mCommandsMap["velocitychangingareadebug"] = &CommandInterpreter::executeVelocityChangingAreaDebug;
	mCommandsMap["pushingareadebug"] =			&CommandInterpreter::executePushingAreaDebug;
	mCommandsMap["mute"] =						&CommandInterpreter::executeMute;
	mCommandsMap["unmute"] =					&CommandInterpreter::executeUnmute;
	mCommandsMap["setvolume"] =					&CommandInterpreter::executeSetVolume;
	mCommandsMap["history"] =					&CommandInterpreter::executeHistory;
	mCommandsMap["help"] =						&CommandInterpreter::executeHelp;
	mCommandsMap["clear"] =						&CommandInterpreter::executeClear;
	mCommandsMap["view"] =						&CommandInterpreter::executeView;
	mCommandsMap["gotoscene"] =					&CommandInterpreter::executeGotoScene;
	mCommandsMap["rgui"] =						&CommandInterpreter::executeResetGui;
	mCommandsMap["light"] =						&CommandInterpreter::executeLight;
	mCommandsMap["m"] =							&CommandInterpreter::executeMove;
	mCommandsMap["fontd"] =						&CommandInterpreter::executeFontDebug;
	mCommandsMap[""] =							&CommandInterpreter::executeInfoMessage;
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
	auto& commandsHistory = mGameData->getTerminal().getSharedData()->lastCommands;
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

void CommandInterpreter::executeResetGui() const
{
	const int spacePosition = mCommand.find_first_of(' ') + 1;
	Xml sceneFile;
	sceneFile.loadFromFile(mGameData->getSceneManager().getCurrentSceneFilePath());
	const auto sceneLinksNode = *sceneFile.getChild("scenelinks");
	if(const auto guiNode = sceneLinksNode.getChild("gui")) {
		const std::string filepath = "scenes/gui/" + guiNode->getAttribute("filename")->toString();
		XmlGuiParser guiParser;
		guiParser.parseGuiXml(filepath);
	}
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
	const sf::Vector2f newPosition = getVector2Argument();
	if(newPosition == mVector2ArgumentError)
		return;

	auto view = mSceneRegistry->view<component::Player, component::BodyRect>();
	view.each([newPosition](const component::Player player, component::BodyRect& body) {
		body.rect.left = newPosition.x;
		body.rect.top = newPosition.y;
	});
}

void CommandInterpreter::executeMove() const
{
	sf::Vector2f moveOffset = getVector2Argument();
	if (commandContains('x'))
		moveOffset = sf::Vector2f(moveOffset.x, 0.f);
	else if (commandContains('y'))
		moveOffset = sf::Vector2f(0.f , moveOffset.y);

	auto view = mSceneRegistry->view<component::Player, component::BodyRect>();
	view.each([moveOffset](const component::Player, component::BodyRect& body) {
		body.rect.left += moveOffset.x;
		body.rect.top += moveOffset.y;
	});
}

void CommandInterpreter::executeGive() const
{
	if (commandContains("bullet"))
	{
		int numberOfItems = static_cast<int>(getVolumeFromCommand());
		auto view = mSceneRegistry->view<component::Player, component::Bullets>();
		view.each([numberOfItems](const component::Player, component::Bullets& bullets) {
			bullets.numOfPistolBullets += numberOfItems;
			bullets.numOfShotgunBullets += numberOfItems;
		});
	}
	else
		executeMessage("Type of item is unknown!", MessageType::ERROR);
}

void CommandInterpreter::executeCurrentPos() const
{
	auto view = mSceneRegistry->view<component::Player, component::BodyRect>();
	view.each([this](const component::Player, const component::BodyRect& body) {
		const sf::Vector2f playerPos = body.rect.getCenter();
		executeMessage("player position: " + Cast::toString(playerPos), MessageType::INFO);
	});
}

void CommandInterpreter::executeCollisionDebug() const
{
	if(commandContains("on"))
		system::AreasDebug::setIsCollisionDebugActive(true);
	if(commandContains("off"))
		system::AreasDebug::setIsCollisionDebugActive(false);
	else
		executeMessage("Incorrect argument! Argument has to be 'on' or 'off'", MessageType::ERROR);
}

void CommandInterpreter::executeVelocityChangingAreaDebug() const
{
	if(commandContains("on"))
		system::AreasDebug::setIsVelocityChangingAreaDebugActive(true);
	if(commandContains("off"))
		system::AreasDebug::setIsVelocityChangingAreaDebugActive(false);
	else
		executeMessage("Incorrect argument! Argument has to be 'on' or 'off'", MessageType::ERROR);
}

void CommandInterpreter::executePushingAreaDebug() const
{
	if(commandContains("on"))
		system::AreasDebug::setIsPushingAreaDebugActive(true);
	if(commandContains("off"))
		system::AreasDebug::setIsPushingAreaDebugActive(false);
	else
		executeMessage("Incorrect argument! Argument has to be 'on' or 'off'", MessageType::ERROR);
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
	auto view = mSceneRegistry->view<component::Player, component::Camera>();
	view.each([this](const component::Player, component::Camera& playerCamera){
		if(commandContains("normal"))
			playerCamera.camera.setSize({640, 360});
		else {
			sf::Vector2f viewSize = getVector2Argument();
			if(viewSize == mVector2ArgumentError)
				return;
			else
				playerCamera.camera.setSize(viewSize);
		}
	});

}

void CommandInterpreter::executeLight() const
{
	bool on;
	if(commandContains("on"))
		on = true;
	else if(commandContains("off"))
		on = false;
	else
		return; // TODO: Display error message

	auto& lightDebug = LightRenderer::getDebug();

	if(commandContains("walls"))
		lightDebug.drawWalls = on;
	else if(commandContains("rays"))
		lightDebug.drawRays = on;
	else
		lightDebug.drawLight = on;
}

void CommandInterpreter::executeFontDebug() const
{
	// TODO: Add font filename command argument and font size command argument
	if(commandContains("on") && !FontDebugRenderer::isActive()) {
		FontDebugRenderer::init("joystixMonospace.ttf", 50);
	}
	if(commandContains("off") && FontDebugRenderer::isActive()) {
		FontDebugRenderer::shutDown();
	}
}

auto CommandInterpreter::getVector2Argument() const -> sf::Vector2f
{
	const std::string numbers("1234567890-");

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
