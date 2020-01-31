#include "commandInterpreter.hpp"
#include "terminalRenderer.hpp"
#include "Audio/Sound/soundData.hpp"
#include "Terminal/terminal.hpp"
#include "Logs/logs.hpp"
#include "Scenes/sceneManager.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/xml.hpp"
#include "game.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Systems/areasDebug.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Renderer/MinorRenderers/lightRenderer.hpp"
#include "Renderer/API/font.hpp"
#include "GUI/xmlGuiParser.hpp"
#include <entt/entt.hpp>

namespace ph {

void CommandInterpreter::init(SceneManager* sceneManager, TerminalSharedData terminalSharedData, TerminalRenderer* terminalRenderer)
{
	mTerminalSharedData = terminalSharedData;
	mTerminalRenderer = terminalRenderer;
	mSceneManager = sceneManager;

	mCommandsMap["teleport"] =					&CommandInterpreter::executeTeleport;
	mCommandsMap["tp"] =						&CommandInterpreter::executeTeleport;
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
	mCommandsMap["gts"] =						&CommandInterpreter::executeGotoScene;
	mCommandsMap["r"] =							&CommandInterpreter::executeReset;
	mCommandsMap["pause"] =						&CommandInterpreter::executePause;
	mCommandsMap["rgui"] =						&CommandInterpreter::executeResetGui;
	mCommandsMap["light"] =						&CommandInterpreter::executeLight;
	mCommandsMap["m"] =							&CommandInterpreter::executeMove;
	mCommandsMap["fontd"] =						&CommandInterpreter::executeFontDebug;
	mCommandsMap["nofocusupdate"] =				&CommandInterpreter::executeNoFocusUpdate;
	mCommandsMap["dc"] =						&CommandInterpreter::executeDebugCamera;
	mCommandsMap[""] =							&CommandInterpreter::executeInfoMessage;

#ifndef PH_DISTRIBUTION

	mCommandsMap["rguilive"] =					&CommandInterpreter::executeResetGuiLive;
	mCommandsMap["rguilivefreq"] =				&CommandInterpreter::executeResetGuiLiveFrequency;

#endif

}

void CommandInterpreter::update(float dt)
{
#ifndef PH_DISTRIBUTION
	if(mResetGuiLive.isActive) {
		mResetGuiLive.timeFromReset += dt;
		if(mResetGuiLive.timeFromReset > mResetGuiLive.resetFrequency) {
			executeResetGui();
			mResetGuiLive.timeFromReset = 0.f;
		}
	}
#endif
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

void CommandInterpreter::executeInfoMessage()
{
	executeMessage("This is terminal. Enter 'help' to see available commands.", MessageType::INFO);
}

void CommandInterpreter::executeHistory()
{
	auto& commandsHistory = mTerminalSharedData->lastCommands;
	std::deque<std::string>::reverse_iterator it = commandsHistory.rbegin();

	for (; it != commandsHistory.rend(); ++it)
		executeMessage("- " + *it, MessageType::INFO);
	executeMessage("Ten last used commands: ", MessageType::INFO);
}

void CommandInterpreter::executeHelp()
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

void CommandInterpreter::executeGotoScene()
{
	const int spacePosition = mCommand.find_first_of(' ') + 1;
	const std::string scenePath = "scenes/" + mCommand.substr(spacePosition, mCommand.size()) + ".xml";
	mSceneManager->replaceScene(scenePath);
}

void CommandInterpreter::executeReset()
{
	mSceneManager->replaceScene(mSceneManager->getCurrentSceneFilePath());	
}

void CommandInterpreter::executePause()
{
	system::System::setPause(!commandContains("off"));
}

void CommandInterpreter::executeResetGui()
{
	const int spacePosition = mCommand.find_first_of(' ') + 1;
	Xml sceneFile;
	sceneFile.loadFromFile(mSceneManager->getCurrentSceneFilePath());
	const auto sceneLinksNode = *sceneFile.getChild("scenelinks");
	if(const auto guiNode = sceneLinksNode.getChild("gui")) {
		const std::string filepath = "scenes/gui/" + guiNode->getAttribute("filename")->toString();
		XmlGuiParser guiParser;
		guiParser.parseGuiXml(filepath);
	}
}

void CommandInterpreter::executeClear()
{
	for (int i = 0; i < 20; ++i)
		executeMessage("", MessageType::BLANK);
}

void CommandInterpreter::executeTeleport()
{
	const sf::Vector2f newPosition = getVector2Argument();
	if(newPosition == mVector2ArgumentError)
		return;

	auto& registry = mSceneManager->getScene().getRegistry(); 
	auto view = registry.view<component::Player, component::BodyRect>();
	view.each([newPosition](const component::Player player, component::BodyRect& body) {
		body.rect.left = newPosition.x;
		body.rect.top = newPosition.y;
	});
}

void CommandInterpreter::executeMove()
{
	sf::Vector2f moveOffset = getVector2Argument();
	if (commandContains('x'))
		moveOffset = sf::Vector2f(moveOffset.x, 0.f);
	else if (commandContains('y'))
		moveOffset = sf::Vector2f(0.f , moveOffset.y);

	auto& registry = mSceneManager->getScene().getRegistry(); 
	auto view = registry.view<component::Player, component::BodyRect>();
	view.each([moveOffset](const component::Player, component::BodyRect& body) {
		body.rect.left += moveOffset.x;
		body.rect.top += moveOffset.y;
	});
}

void CommandInterpreter::executeGive()
{
	if (commandContains("bullet"))
	{
		int numberOfItems = static_cast<int>(getSingleFloatArgument());
		auto& registry = mSceneManager->getScene().getRegistry(); 
		auto view = registry.view<component::Player, component::Bullets>();
		view.each([numberOfItems](const component::Player, component::Bullets& bullets) {
			bullets.numOfPistolBullets += numberOfItems;
			bullets.numOfShotgunBullets += numberOfItems;
		});
	}
	else
		executeMessage("Type of item is unknown!", MessageType::ERROR);
}

void CommandInterpreter::executeCurrentPos()
{
	auto& registry = mSceneManager->getScene().getRegistry(); 
	auto view = registry.view<component::Player, component::BodyRect>();
	view.each([this](const component::Player, const component::BodyRect& body) {
		const sf::Vector2f playerPos = body.rect.getCenter();
		executeMessage("player position: " + Cast::toString(playerPos), MessageType::INFO);
	});
}

void CommandInterpreter::executeCollisionDebug()
{
	if(commandContains("on"))
		system::AreasDebug::setIsCollisionDebugActive(true);
	if(commandContains("off"))
		system::AreasDebug::setIsCollisionDebugActive(false);
	else
		executeMessage("Incorrect argument! Argument has to be 'on' or 'off'", MessageType::ERROR);
}

void CommandInterpreter::executeVelocityChangingAreaDebug()
{
	if(commandContains("on"))
		system::AreasDebug::setIsVelocityChangingAreaDebugActive(true);
	if(commandContains("off"))
		system::AreasDebug::setIsVelocityChangingAreaDebugActive(false);
	else
		executeMessage("Incorrect argument! Argument has to be 'on' or 'off'", MessageType::ERROR);
}

void CommandInterpreter::executePushingAreaDebug()
{
	if(commandContains("on"))
		system::AreasDebug::setIsPushingAreaDebugActive(true);
	if(commandContains("off"))
		system::AreasDebug::setIsPushingAreaDebugActive(false);
	else
		executeMessage("Incorrect argument! Argument has to be 'on' or 'off'", MessageType::ERROR);
}

void CommandInterpreter::executeMute()
{
	setAudioMuted(true);
}

void CommandInterpreter::executeUnmute()
{
	setAudioMuted(false);
}

void CommandInterpreter::setAudioMuted(bool mute) const
{
	if(commandContains("music"))
		MusicPlayer::setMuted(mute);
	else if(commandContains("sound"))
		SoundPlayer::setMuted(mute);
	else if(commandContains("all")) {
		MusicPlayer::setMuted(mute);
		SoundPlayer::setMuted(mute);
	}
	else
		executeMessage("Incorrect second argument! You have to enter 'music', 'sound' or 'all'.", MessageType::ERROR);
}

void CommandInterpreter::executeSetVolume()
{
	const float newVolume = getSingleFloatArgument();
	if(!(commandContains('0')) && newVolume == 0 || newVolume > 100){
		executeMessage("Incorrect volume value! Enter value from 0 to 100", MessageType::ERROR);
		return;
	}

	if (commandContains("music"))
		MusicPlayer::setVolume(newVolume);
	else if (commandContains("sound"))
		SoundPlayer::setVolume(newVolume);
	else{
		MusicPlayer::setVolume(newVolume);
		SoundPlayer::setVolume(newVolume);
	}
}

void CommandInterpreter::executeView()
{
	auto& registry = mSceneManager->getScene().getRegistry(); 
	auto view = registry.view<component::Player, component::Camera>();
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

void CommandInterpreter::executeLight()
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

void CommandInterpreter::executeFontDebug()
{
	// TODO: Add font filename command argument and font size command argument
	if(commandContains("on") && !FontDebugRenderer::isActive()) {
		FontDebugRenderer::init("joystixMonospace.ttf", 50);
	}
	if(commandContains("off") && FontDebugRenderer::isActive()) {
		FontDebugRenderer::shutDown();
	}
}

void CommandInterpreter::executeNoFocusUpdate()
{
	Game::setNoFocusUpdate(!commandContains("off"));
}

void CommandInterpreter::executeDebugCamera()
{
	auto& registry = mSceneManager->getScene().getRegistry(); 

	auto destroyExistingDebugCameras = [&registry]() {
		auto debugCameras = registry.view<component::DebugCamera, component::Camera, component::BodyRect>();
		registry.destroy(debugCameras.begin(), debugCameras.end());
	};

	component::Camera::currentCameraName = "default";
	destroyExistingDebugCameras();
	mTerminalSharedData->isVisible = true;

	if(!commandContains("off"))	
	{
		// get player position
		sf::Vector2f playerPos;
		auto players = registry.view<component::Player, component::BodyRect>();
		players.each([&playerPos](const component::Player, const component::BodyRect body) {
			playerPos = body.rect.getCenter();
		});

		// create debug camera
		auto entity = registry.create();
		registry.assign<component::Camera>(entity, Camera(playerPos, {640, 360}), "debug");
		registry.assign<component::DebugCamera>(entity);
		registry.assign<component::BodyRect>(entity, FloatRect(playerPos, {0.f, 0.f}));
		component::Camera::currentCameraName = "debug";

		mTerminalSharedData->isVisible = false;
	}
}

#ifndef PH_DISTRIBUTION

void CommandInterpreter::executeResetGuiLive()
{
	mResetGuiLive.isActive = !commandContains("off");
	Game::setNoFocusUpdate(mResetGuiLive.isActive);
}

void CommandInterpreter::executeResetGuiLiveFrequency()
{
	mResetGuiLive.resetFrequency = getSingleFloatArgument();
}

#endif 


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

float CommandInterpreter::getSingleFloatArgument() const
{
	const size_t spacePosition = mCommand.find_last_of(' ');
	const size_t valueStartPos = spacePosition + 1;
	const size_t valueLength = mCommand.size() - valueStartPos;
	const std::string value = mCommand.substr(valueStartPos, valueLength);
	return std::strtof(value.c_str(), nullptr);
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

	mTerminalRenderer->pushOutputLine(OutputLine{ message, color });
}

}
