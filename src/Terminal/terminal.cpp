#include "terminal.hpp"
#include "game.hpp"
#include "Events/actionEventManager.hpp"
#include "Logs/logs.hpp"
#include "Scenes/sceneManager.hpp"
#include "ECS/System.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Systems/areasDebug.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Sound/soundData.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/MinorRenderers/lightRenderer.hpp"
#include "Renderer/API/font.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/xml.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <entt/entt.hpp>
#include <array>
#include <deque>
#include <unordered_map>
#include <string>

namespace ph {

namespace {

std::string content;
std::deque<std::string> lastCommands;
bool isVisible = false;

sf::Window* window;
ph::SceneManager* sceneManager;

int indexOfCurrentLastCommand = -1;
std::deque<ph::OutputLine> outputLines;
const sf::Vector2f vector2ArgumentError = {-1, -1};

typedef void (*ExecuteCommandFunction)(void);
std::unordered_map<std::string, ExecuteCommandFunction> commandsMap;

enum class MessageType
{
	ERROR, INFO, USER, BLANK
};

#ifndef PH_DISTRIBUTION
struct ResetGuiLive
{
	float timeFromReset = 0.f;
	float resetFrequency = 0.2f;
	bool isActive = false;
};
ResetGuiLive resetGuiLive;
#endif 

}

static void executeMessage(const std::string& message, const MessageType colorType)
{
	sf::Color color;
	switch(colorType)
	{
	case MessageType::ERROR:
		color = {255, 25, 33};
		break;
	case MessageType::INFO:
		color = {127, 244, 44};
		break;
	case MessageType::USER:
		color = sf::Color(79, 202, 255);
		break;
	case MessageType::BLANK:
		color = sf::Color::Transparent;
		break;
	}

	Terminal::pushOutputLine(OutputLine{message, color});
}

static sf::Vector2f handleGetVector2ArgumentError()
{
	executeMessage("Incorrect argument! Argument has to be a number.", MessageType::ERROR);
	return vector2ArgumentError;
}

static sf::Vector2f getVector2Argument()
{
	const std::string numbers("1234567890-");

	if(content.find_first_of(numbers) == std::string::npos)
		return handleGetVector2ArgumentError();

	const size_t xArgumentPositionInCommand = content.find_first_of(numbers);
	const size_t xArgumentEndPositionInCommand = content.find(' ', xArgumentPositionInCommand);
	const size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = content.substr(xArgumentPositionInCommand, xArgumentLength);
	const float positionX = std::strtof(xArgument.c_str(), nullptr);

	const size_t yArgumentPositionInCommand = content.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
	if(yArgumentPositionInCommand == std::string::npos)
		return handleGetVector2ArgumentError();
	const size_t yArgumentEndPositionInCommand = content.find_first_not_of(numbers, yArgumentPositionInCommand);
	const size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	const std::string yArgument = content.substr(yArgumentPositionInCommand, yArgumentLength);
	const float positionY = std::strtof(yArgument.c_str(), nullptr);

	return sf::Vector2f(positionX, positionY);
}

static float getSingleFloatArgument()
{
	const size_t spacePosition = content.find_last_of(' ');
	const size_t valueStartPos = spacePosition + 1;
	const size_t valueLength = content.size() - valueStartPos;
	const std::string value = content.substr(valueStartPos, valueLength);
	return std::strtof(value.c_str(), nullptr);
}

static bool commandContains(const char c)
{
	return content.find(c) != std::string::npos;
}

static bool commandContains(const char* c)
{
	return content.find(c) != std::string::npos;
}

static void executeCommand()
{
	// find command without arguments
	size_t argumentPosition = content.find(' ');
	if(argumentPosition == std::string::npos)
		argumentPosition = content.size();
	const std::string commandWithoutArguments = content.substr(0, argumentPosition);

	// execute command
	auto found = commandsMap.find(commandWithoutArguments);
	if(found != commandsMap.end())
		(*found->second)();
	else
		executeMessage("Entered command wasn't recognised. Enter 'help' to see available commands.", MessageType::ERROR);
}

static void executeInfoMessage()
{
	executeMessage("This is terminal. Enter 'help' to see available commands.", MessageType::INFO);
}

static void executeHistory()
{
	auto& commandsHistory = lastCommands;
	std::deque<std::string>::reverse_iterator it = commandsHistory.rbegin();

	for(; it != commandsHistory.rend(); ++it)
		executeMessage("- " + *it, MessageType::INFO);
	executeMessage("Ten last used commands: ", MessageType::INFO);
}

static void executeHelp()
{
	const std::array<std::string, 11> commandsList1{
		"VELD - enables(with no args) or disables(with 'off' arg) velocity changing areas debug",
		"PUSHD - enables(with no args) or disables(with 'off' arg) pushing areas debug"
		"COLLISION DEBUG / COLD - enables(with no args) or disables(with 'off' arg) collision debug",
		"GIVE - adds certain amount of certain item to inventory, takes args (amount, item) items: bullet",
		"TELEPORT / TP - teleports player to given position, takes args (x, y)",
		"M - moves player for certain vector, takes args (x, y)",
		"SETVOLUME - sets volume, you have to pass number from 0 to 100 as an arg",
		"UNMUTE - unmutes audio, if you don't pass any args it will mute everything; But you can pass args: 'music' 'sound'",
		"MUTE - mutes audio, if you don't pass any args it will mute everything; But you can pass args: 'music' 'sound'",
		"HISTORY - shows last commands",
		"HELP - shows avaiable commends",
	};
	const std::array<std::string, 3> commandsList2{
		"CURRENTPOS", "SPAWN", "VIEW"
	};

	if(commandContains('2')) {
		for(auto& command : commandsList2)
			executeMessage("- " + command, MessageType::INFO);
		executeMessage("Available commands, PAGE 2 of 2.", MessageType::INFO);
	}
	else {
		for(auto& command : commandsList1)
			executeMessage("- " + command, MessageType::INFO);
		executeMessage("Available commands, PAGE 1 of 2.", MessageType::INFO);
	}
}

static void executeGotoScene()
{
	const int spacePosition = content.find_first_of(' ') + 1;
	const std::string scenePath = "scenes/" + content.substr(spacePosition, content.size()) + ".xml";
	sceneManager->replaceScene(scenePath);
}

static void executeReset()
{
	sceneManager->replaceScene(sceneManager->getCurrentSceneFilePath());
}

static void executePause()
{
	system::System::setPause(!commandContains("off"));
}

static void executeResetGui()
{
	const int spacePosition = content.find_first_of(' ') + 1;
	Xml sceneFile;
	sceneFile.loadFromFile(sceneManager->getCurrentSceneFilePath());
	const auto sceneLinksNode = *sceneFile.getChild("scenelinks");
	if(const auto guiNode = sceneLinksNode.getChild("gui")) {
		const std::string filepath = "scenes/gui/" + guiNode->getAttribute("filename")->toString();
		XmlGuiParser guiParser;
		guiParser.parseGuiXml(filepath);
	}
}

static void executeClear()
{
	for(int i = 0; i < 20; ++i)
		executeMessage("", MessageType::BLANK);
}

static void executeTeleport()
{
	const sf::Vector2f newPosition = getVector2Argument();
	if(newPosition == vector2ArgumentError)
		return;

	auto& registry = sceneManager->getScene().getRegistry();
	auto view = registry.view<component::Player, component::BodyRect>();
	view.each([newPosition](const component::Player player, component::BodyRect& body) {
		body.rect.left = newPosition.x;
		body.rect.top = newPosition.y;
		});
}

static void executeMove()
{
	sf::Vector2f moveOffset = getVector2Argument();

	auto& registry = sceneManager->getScene().getRegistry();
	auto view = registry.view<component::Player, component::BodyRect>();
	view.each([moveOffset](const component::Player, component::BodyRect& body) {
		body.rect.left += moveOffset.x;
		body.rect.top += moveOffset.y;
		});
}

static void executeGive()
{
	if(commandContains("bullet"))
	{
		int numberOfItems = static_cast<int>(getSingleFloatArgument());
		auto& registry = sceneManager->getScene().getRegistry();
		auto view = registry.view<component::Player, component::Bullets>();
		view.each([numberOfItems](const component::Player, component::Bullets& bullets) {
			bullets.numOfPistolBullets += numberOfItems;
			bullets.numOfShotgunBullets += numberOfItems;
			});
	}
	else
		executeMessage("Type of item is unknown!", MessageType::ERROR);
}

static void executeCurrentPos()
{
	auto& registry = sceneManager->getScene().getRegistry();
	auto view = registry.view<component::Player, component::BodyRect>();
	view.each([](const component::Player, const component::BodyRect& body) {
		const sf::Vector2f playerPos = body.rect.getCenter();
		executeMessage("player position: " + Cast::toString(playerPos), MessageType::INFO);
		});
}

static void executeCollisionDebug()
{
	system::AreasDebug::setIsCollisionDebugActive(!commandContains("off"));
}

static void executeVelocityChangingAreaDebug()
{
	system::AreasDebug::setIsVelocityChangingAreaDebugActive(!commandContains("off"));
}

static void executePushingAreaDebug()
{
	system::AreasDebug::setIsPushingAreaDebugActive(!commandContains("off"));
}

static void executeLightWallsAreaDebug()
{
	system::AreasDebug::setIsLightWallsAreaDebugActive(!commandContains("off"));
}

static void setAudioMuted(bool mute)
{
	if(commandContains("music"))
		MusicPlayer::setMuted(mute);
	else if(commandContains("sound"))
		SoundPlayer::setMuted(mute);
	else {
		MusicPlayer::setMuted(mute);
		SoundPlayer::setMuted(mute);
	}
}

static void executeMute()
{
	setAudioMuted(true);
}

static void executeUnmute()
{
	setAudioMuted(false);
}

static void executeSetVolume()
{
	const float newVolume = getSingleFloatArgument();
	if(!(commandContains('0')) && newVolume == 0 || newVolume > 100) {
		executeMessage("Incorrect volume value! Enter value from 0 to 100", MessageType::ERROR);
		return;
	}

	if(commandContains("music"))
		MusicPlayer::setVolume(newVolume);
	else if(commandContains("sound"))
		SoundPlayer::setVolume(newVolume);
	else {
		MusicPlayer::setVolume(newVolume);
		SoundPlayer::setVolume(newVolume);
	}
}

static void executeView()
{
	auto& registry = sceneManager->getScene().getRegistry();
	auto view = registry.view<component::Player, component::Camera>();
	view.each([](const component::Player, component::Camera& playerCamera) {
		if(commandContains("normal"))
			playerCamera.camera.setSize({640, 360});
		else {
			sf::Vector2f viewSize = getVector2Argument();
			if(viewSize == vector2ArgumentError)
				return;
			else
				playerCamera.camera.setSize(viewSize);
		}
		});

}

static void executeLight()
{
	bool on;
	if(commandContains("on"))
		on = true;
	else if(commandContains("off"))
		on = false;
	else
		return; // TODO: Display error message

	auto& lightDebug = LightRenderer::getDebug();

	if(commandContains("rays"))
		lightDebug.drawRays = on;
	else
		lightDebug.drawLight = on;
}

static void executeFontDebug()
{
	// TODO: Add font filename command argument and font size command argument
	if(commandContains("on") && !FontDebugRenderer::isActive()) {
		FontDebugRenderer::init("joystixMonospace.ttf", 50);
	}
	if(commandContains("off") && FontDebugRenderer::isActive()) {
		FontDebugRenderer::shutDown();
	}
}

static void executeNoFocusUpdate()
{
	Game::setNoFocusUpdate(!commandContains("off"));
}

static void executeDebugCamera()
{
	auto& registry = sceneManager->getScene().getRegistry();

	auto destroyExistingDebugCameras = [&registry]() {
		auto debugCameras = registry.view<component::DebugCamera, component::Camera, component::BodyRect>();
		registry.destroy(debugCameras.begin(), debugCameras.end());
	};

	component::Camera::currentCameraName = "default";
	destroyExistingDebugCameras();
	isVisible = true;

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

		isVisible = false;
	}
}

#ifndef PH_DISTRIBUTION

static void executeResetGuiLive()
{
	resetGuiLive.isActive = !commandContains("off");
	Game::setNoFocusUpdate(resetGuiLive.isActive);
}

static void executeResetGuiLiveFrequency()
{
	resetGuiLive.resetFrequency = getSingleFloatArgument();
}

#endif 

static void updateCommands(float dt)
{
#ifndef PH_DISTRIBUTION
	if(resetGuiLive.isActive) {
		resetGuiLive.timeFromReset += dt;
		if(resetGuiLive.timeFromReset > resetGuiLive.resetFrequency) {
			executeResetGui();
			resetGuiLive.timeFromReset = 0.f;
		}
	}
#endif
}

namespace Terminal {

void handleEvent(Event& phEvent)
{
	sf::Event* e = std::get_if<sf::Event>(&phEvent);
	if(!e)
		return;

	if(isVisible && e->type == sf::Event::TextEntered)
	{
		char key = static_cast<char>(e->text.unicode);
		if(!iscntrl(key))
			content += key;
	}

	if(e->type == sf::Event::KeyPressed)
	{
		switch(e->key.code)
		{
		case sf::Keyboard::Tab: {
			if(e->key.control) {
				// show or hide command prompt
				ActionEventManager::setEnabled(isVisible);
				isVisible = !isVisible;
				window->setKeyRepeatEnabled(isVisible);
				system::System::setPause(isVisible);
			}
		} break;

		case sf::Keyboard::BackSpace: {
			if(content.size() > 0)
				content.pop_back();
		} break;

		case sf::Keyboard::Enter: {
			// execute command
			executeCommand();

			// clear input area content
			content.clear();

			// update last commands
			indexOfCurrentLastCommand = -1;
			if(content.size() != 0) {
				lastCommands.emplace_front(content);
				if(lastCommands.size() > 10)
					lastCommands.pop_back();
			}
		} break;

		case sf::Keyboard::Up: {
			if(indexOfCurrentLastCommand + 1 < static_cast<int>(lastCommands.size()))
			{
				++indexOfCurrentLastCommand;
				if(indexOfCurrentLastCommand >= 0)
					content = lastCommands[indexOfCurrentLastCommand];
			}
		} break;

		case sf::Keyboard::Down: {
			if(indexOfCurrentLastCommand > -1)
			{
				--indexOfCurrentLastCommand;
				if(indexOfCurrentLastCommand == -1)
					content.clear();
				else
					content = lastCommands[indexOfCurrentLastCommand];
			}
		} break;
		}
	}
}

void pushOutputLine(const OutputLine& line)
{
	if(outputLines.size() >= 14)
		outputLines.pop_back();
	outputLines.emplace_front(line);
}

void init(sf::Window* w, SceneManager* sm)
{
	window = w;
	sceneManager = sm;

	commandsMap["teleport"] = &executeTeleport;
	commandsMap["tp"] = &executeTeleport;
	commandsMap["give"] = &executeGive;
	commandsMap["currentpos"] = &executeCurrentPos;
	commandsMap["collisiondebug"] = &executeCollisionDebug;
	commandsMap["cold"] = &executeCollisionDebug;
	commandsMap["veld"] = &executeVelocityChangingAreaDebug;
	commandsMap["pushd"] = &executePushingAreaDebug;
	commandsMap["lwd"] = &executeLightWallsAreaDebug;
	commandsMap["mute"] = &executeMute;
	commandsMap["unmute"] = &executeUnmute;
	commandsMap["setvolume"] = &executeSetVolume;
	commandsMap["history"] = &executeHistory;
	commandsMap["help"] = &executeHelp;
	commandsMap["clear"] = &executeClear;
	commandsMap["view"] = &executeView;
	commandsMap["gotoscene"] = &executeGotoScene;
	commandsMap["gts"] = &executeGotoScene;
	commandsMap["r"] = &executeReset;
	commandsMap["pause"] = &executePause;
	commandsMap["rgui"] = &executeResetGui;
	commandsMap["light"] = &executeLight;
	commandsMap["m"] = &executeMove;
	commandsMap["fontd"] = &executeFontDebug;
	commandsMap["nofocusupdate"] = &executeNoFocusUpdate;
	commandsMap["dc"] = &executeDebugCamera;
	commandsMap[""] = &executeInfoMessage;

#ifndef PH_DISTRIBUTION
	commandsMap["rguilive"] = &executeResetGuiLive;
	commandsMap["rguilivefreq"] = &executeResetGuiLiveFrequency;
#endif
}

void update(float dt)
{
	updateCommands(dt);

	if(isVisible)
	{
		Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 0, 0, 230), nullptr, {0.f, 660.f}, {1920.f, 420.f}, 5, 0.f, {},
			ProjectionType::gui, false);

		Renderer::submitQuad(nullptr, nullptr, &sf::Color::Black, nullptr, {0.f, 720.f}, {1920.f, 5.f},
			4, 0.f, {}, ProjectionType::gui, false);

		Renderer::submitText(content.c_str(), "LiberationMono-Bold.ttf", {5.f, 660.f},
			50.f, sf::Color::White, 0, ProjectionType::gui, false);

		float posY = 723.f;
		for(size_t i = 0; i < outputLines.size(); ++i, posY += 25.f)
			Renderer::submitText(outputLines[i].text.c_str(), "LiberationMono.ttf", {5.f, posY}, 25.f, outputLines[i].color, 0, ProjectionType::gui, false);
	}
}

}}
