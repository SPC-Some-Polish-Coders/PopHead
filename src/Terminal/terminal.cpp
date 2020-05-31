#include "pch.hpp"
#include "terminal.hpp"
#include "game.hpp"
#include "Scenes/sceneManager.hpp"
#include "ECS/System.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Components/debugComponents.hpp"
#include "ECS/Systems/zombieSystem.hpp"
#include "ECS/Systems/hostileCollisions.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Sound/soundData.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/font.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "Utilities/cast.hpp"

namespace ph::Terminal {

static std::string content;
static std::deque<std::string> lastCommands;
static bool isVisible = false;

static sf::Window* window;
static ph::SceneManager* sceneManager;

static i32 indexOfCurrentLastCommand = -1;
static std::deque<ph::OutputLine> outputLines;
static const Vec2 vector2ArgumentError = {-1, -1};

typedef void (*ExecuteCommandFunction)(void);
static std::unordered_map<std::string, ExecuteCommandFunction> commandsMap;

static sf::Color errorRedColor = {255, 25, 33};
static sf::Color infoLimeColor = {127, 244, 44};

#ifndef PH_DISTRIBUTION
struct ResetGuiLive
{
	float timeFromReset = 0.f;
	float resetFrequency = 0.2f;
	bool isActive = false;
};
static ResetGuiLive resetGuiLive;
#endif 

static Vec2 getPlayerPosition()
{
	Vec2 playerPos = vector2ArgumentError;
	auto& registry = sceneManager->getScene().getRegistry();
	registry.view<component::Player, component::BodyRect>().each([&playerPos]
	(auto, const auto& body) 
	{
		playerPos = body.center();
	});
	return playerPos;
}

static Vec2 handleGetVector2ArgumentError()
{
	pushOutputLine({"Incorrect argument! Argument has to be a number.", errorRedColor});
	return vector2ArgumentError;
}

static Vec2 getVector2Argument()
{
	const std::string numbers("1234567890-");

	if(content.find_first_of(numbers) == std::string::npos)
		return handleGetVector2ArgumentError();

	const size_t xArgumentPositionInCommand = content.find_first_of(numbers);
	const size_t xArgumentEndPositionInCommand = content.find(' ', xArgumentPositionInCommand);
	const size_t xArgumentLength = xArgumentEndPositionInCommand - xArgumentPositionInCommand;
	std::string xArgument = content.substr(xArgumentPositionInCommand, xArgumentLength);
	const float positionX = std::strtof(xArgument.c_str(), Null);

	const size_t yArgumentPositionInCommand = content.find_first_of(numbers, xArgumentEndPositionInCommand + 1);
	if(yArgumentPositionInCommand == std::string::npos)
		return handleGetVector2ArgumentError();
	const size_t yArgumentEndPositionInCommand = content.find_first_not_of(numbers, yArgumentPositionInCommand);
	const size_t yArgumentLength = yArgumentEndPositionInCommand - yArgumentPositionInCommand;
	const std::string yArgument = content.substr(yArgumentPositionInCommand, yArgumentLength);
	const float positionY = std::strtof(yArgument.c_str(), Null);

	return Vec2(positionX, positionY);
}

static float getSingleFloatArgument()
{
	const size_t spacePosition = content.find_last_of(' ');
	const size_t valueStartPos = spacePosition + 1;
	const size_t valueLength = content.size() - valueStartPos;
	const std::string value = content.substr(valueStartPos, valueLength);
	return std::strtof(value.c_str(), Null);
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
		pushOutputLine({"Entered command wasn't recognised. Enter 'help' to see available commands.", sf::Color::Red});
}

static void executeInfoMessage()
{
	pushOutputLine({"This is terminal. Enter 'help' to see available commands.", sf::Color(50, 50, 255)});
}

static void executeHistory()
{
	auto& commandsHistory = lastCommands;
	std::deque<std::string>::reverse_iterator it = commandsHistory.rbegin();

	for(; it != commandsHistory.rend(); ++it)
		pushOutputLine({"- " + *it, infoLimeColor});
	pushOutputLine({"Ten last used commands: ", sf::Color::White});
}

static void executeHelp()
{
	pushOutputLine({});
	pushOutputLine({"fz @C9999 freeze zombies @CO @S31", infoLimeColor});
	pushOutputLine({"history @C9999 show last commands @CO @S31 currentpos @C9999 output player's position @CO @S32 view @C9999 change player's camera size", infoLimeColor});
	pushOutputLine({"give @C9999 player gets an item @CO @S31 tp @C9999 teleport @CO @S32 m @C9999 move player", infoLimeColor});
	pushOutputLine({"setvolume @S31 mute @C9999 mute audio @CO @S32 unmute @C9999 unmute audio", infoLimeColor});
	pushOutputLine({"gts @C9999 go to scene @CO @S31 r @C9999 reset scene @CO @S32 clear @C9999 clear terminal output", infoLimeColor});
	pushOutputLine({"pause @C9999 pause game @CO @S31 rgui @C9999 reset gui @CO @S32 rguilive @C9999 reset gui all the time", infoLimeColor});
	pushOutputLine({"rguilivefreq @C9999 set gui reset frequency", infoLimeColor});
	pushOutputLine({"@CO @S31 nofocusupdate @CO @S32 gm @C9999 god mode", infoLimeColor});
	pushOutputLine({"@C9509 TO LEARN MORE DETAILS ABOUT THE COMMAND USE @CO? @C9509 For example: @COgts ?", infoLimeColor});
}

static void executeGotoScene()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C2919 Example: @C9609 gts sewage@CO goes to sewage scene of file sewage.xml"});
		pushOutputLine({"@C9609 gts@CO takes one argument which is name of the scene file without extension."});
	}
	else
	{
		size_t spacePosition = content.find_first_of(' ') + 1;
		const std::string scenePath = "scenes/" + content.substr(spacePosition, content.size()) + ".xml";
		sceneManager->replaceScene(scenePath);
	}
}

static void executeReset()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C9609 r stay@CO reloads the current scene and spawns player in his current position."});
		pushOutputLine({"@C9609 r@CO reloads the current scene."});
	}
	else
	{
		if(commandContains("stay"))
			sceneManager->replaceScene(sceneManager->getCurrentSceneFilePath(), getPlayerPosition());
		else
			sceneManager->replaceScene(sceneManager->getCurrentSceneFilePath());
	}
}

static void executePause()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"More precisly it calls system::System::setPause(!commandContains(\"off\"))"});
		pushOutputLine({"@C9609 pause off@CO Unpauses the game"});
		pushOutputLine({"@C9609 pause@CO Pauses the game"});
	}
	else
	{
		system::System::setPause(!commandContains("off"));
	}
}

static void executeResetGui()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C9609 rgui@CO reloads current scene gui from file, doesn't take arguments"});
	}
	else
	{
		size_t spacePosition = content.find_first_of(' ') + 1;
		Xml sceneFile;
		sceneFile.loadFromFile(sceneManager->getCurrentSceneFilePath());
		const auto sceneLinksNode = *sceneFile.getChild("scenelinks");
		if(const auto guiNode = sceneLinksNode.getChild("gui")) 
		{
			const std::string filepath = "scenes/gui/" + guiNode->getAttribute("filename")->toString();
			XmlGuiParser guiParser;
			guiParser.parseGuiXml(filepath);
		}
	}
}

static void executeClear()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C9609clear@CO clears terminal output area, doesn't take arguments"});
	}
	else
	{
		for(i32 i = 0; i < 20; ++i)
			pushOutputLine({""});
	}
}

static void executeTeleportPoint()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C2919 Example: @C9609tp cave1"});
		pushOutputLine({"@C2919 Example: @C9609tp start"});
		pushOutputLine({"@C9609tp@CO teleports player to specified teleport point"});
	}
	else
	{
		size_t spacePosition = content.find_first_of(' ') + 1;
		const std::string teleportPointName = content.substr(spacePosition, content.size());

		auto& registry = sceneManager->getScene().getRegistry();

		Vec2 newPos; 
		bool tpExists = false;
		registry.view<component::TeleportPoint, component::BodyRect>().each([&]
		(auto tp, auto body)
		{
			if(tp.name == teleportPointName)
			{
				newPos = body.pos;
				tpExists = true;
			}
		});

		if(tpExists)
		{
			registry.view<component::Player, component::BodyRect>().each([newPos]
			(auto player, auto& body) 
			{
				body.pos = newPos;
			});
		}
	}
}

static void executeTeleport()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C2919 Example: @C9609tp 100"});
		pushOutputLine({"@C2919 Example: @C9609tp -100 2000"});
		pushOutputLine({"It takes 1 parameter(a, a) or 2 parameters (x, y)"});
		pushOutputLine({"If player is not on the scene it doesn't do anything"});
		pushOutputLine({"@C9609tp@CO teleports player to absolute coordinate"});
	}
	else
	{
		Vec2 newPosition = getVector2Argument();
		if(newPosition == vector2ArgumentError)
			return;

		auto& registry = sceneManager->getScene().getRegistry();
		registry.view<component::Player, component::BodyRect>().each([newPosition]
		(auto, auto& body) 
		{
			body.pos = newPosition;
		});
	}
}

static void executeMove()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C2919 Example: @C9609m 100"});
		pushOutputLine({"@C2919 Example: @C9609m -100 2000"});
		pushOutputLine({"It takes 1 parameter(a, a) or 2 parameters (x, y)"});
		pushOutputLine({"If player is not on the scene it doesn't do anything"});
		pushOutputLine({"@C9609m@CO teleports player to relative coordinate"});
	}
	else
	{
		Vec2 moveOffset = getVector2Argument();
		auto& registry = sceneManager->getScene().getRegistry();
		registry.view<component::Player, component::BodyRect>().each([moveOffset]
		(auto, auto& body) 
		{
			body.pos += moveOffset;
		});
	}
}

static void executeGive()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C2919 Example: @C9609give bullet 100"});
		pushOutputLine({"It takes 2 parameters (number of items, item name)"});
		pushOutputLine({"@C9609give@CO puts given number of given item in player's inventory"});
	}
	else
	{
		if(commandContains("bullet"))
		{
			i32 numberOfItems = Cast<i32>(getSingleFloatArgument());
			auto& registry = sceneManager->getScene().getRegistry();
			registry.view<component::Player, component::Bullets>().each([numberOfItems]
			(auto, auto& bullets) 
			{
				bullets.numOfPistolBullets += numberOfItems;
				bullets.numOfShotgunBullets += numberOfItems;
			});
		}
		else
		{
			pushOutputLine({"Type of item is unknown!", errorRedColor});
		}
	}
}

static void executeCurrentPos()
{
	if(commandContains('?'))
	{
		pushOutputLine({""});
		pushOutputLine({"@C9609currentpos@CO Outputs player's position to terminal"});
	}
	else
	{
		pushOutputLine({"player position: " + ph::toString(getPlayerPosition()), infoLimeColor});
	}
}

static void setAudioMuted(bool mute)
{
	if(commandContains("music")) 
	{
		MusicPlayer::setMuted(mute);
	}
	else if(commandContains("sound")) 
	{
		SoundPlayer::setMuted(mute);
	}
	else 
	{
		MusicPlayer::setMuted(mute);
		SoundPlayer::setMuted(mute);
	}
}

static void executeMute()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"@C9609mute sound @CO mutes only sound"});
		pushOutputLine({"@C9609mute music @CO mutes only music"});
		pushOutputLine({"@C9609mute @CO mutes audio"});
	}
	else
	{
		setAudioMuted(true);
	}
}

static void executeUnmute()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"@C9609unmute sound @CO unmutes only sound"});
		pushOutputLine({"@C9609unmute music @CO unmutes only music"});
		pushOutputLine({"@C9609unmute @CO unmutes audio"});
	}
	else
	{
		setAudioMuted(false);
	}
}

static void executeSetVolume()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"@C9609 setvolume sound @CO sets sound volume"});
		pushOutputLine({"@C9609 setvolume music @CO sets music volume"});
		pushOutputLine({"@C9609 setvolume @CO sets audio volume"});
	}
	else
	{
		const float newVolume = getSingleFloatArgument();
		if(!(commandContains('0')) && newVolume == 0 || newVolume > 100) 
		{
			pushOutputLine({"Incorrect volume value! Enter value from 0 to 100", sf::Color::Red});
			return;
		}

		if(commandContains("music")) 
		{
			MusicPlayer::setVolume(newVolume);
		}
		else if(commandContains("sound")) 
		{
			SoundPlayer::setVolume(newVolume);
		}
		else 
		{
			MusicPlayer::setVolume(newVolume);
			SoundPlayer::setVolume(newVolume);
		}
	}
}

static void executeFontDebug()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"@C9609 fontd off @CO disables font debug"});
		pushOutputLine({"@C9609 fontd @CO enables font debug"});
	}
	else
	{
		if(commandContains("off") && FontDebugRenderer::isActive())
			FontDebugRenderer::shutDown();
		else if(!FontDebugRenderer::isActive())
			FontDebugRenderer::init("joystixMonospace.ttf", 50);
	}
}

static void executeNoFocusUpdate()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"@C9609 nofocusupdate off@CO disables updating game if game's window doesn't have focus"});
		pushOutputLine({"@C9609 nofocusupdate@CO enables updating game if game's window doesn't have focus"});
	}
	else
	{
		Game::setNoFocusUpdate(!commandContains("off"));
	}
}

static void executeFreezeZombies()
{
	system::ZombieSystem::freezeZombies = !commandContains("off");
}

static void executeGodMode()
{
	system::HostileCollisions::godMode = !commandContains("off");
}

#ifndef PH_DISTRIBUTION

static void executeResetGuiLive()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"you can change rguilivefreq with @C9609 rguilivefreq @CO command"});
		pushOutputLine({"@C9609 rguilive off@CO disables loading gui from file once for rguilivefreq seconds"});
		pushOutputLine({"@C9609 rguilive@CO enables loading gui from file once for rguilivefreq seconds"});
	}
	else
	{
		resetGuiLive.isActive = !commandContains("off");
		Game::setNoFocusUpdate(resetGuiLive.isActive);
	}
}

static void executeResetGuiLiveFrequency()
{
	if(commandContains('?'))
	{
		pushOutputLine({});
		pushOutputLine({"@C2919 Example: @C9609 rguilivefreq 0.5@CO sets rguilive freq to 0.5 seconds"});
		pushOutputLine({"@C9609 rguilivefreq@CO takes one floating point argument and sets rguilivefreq"});
	}
	else
	{
		resetGuiLive.resetFrequency = getSingleFloatArgument();
	}
}

#endif 

static void updateCommands(float dt)
{
#ifndef PH_DISTRIBUTION
	if(resetGuiLive.isActive) 
	{
		resetGuiLive.timeFromReset += dt;
		if(resetGuiLive.timeFromReset > resetGuiLive.resetFrequency) 
		{
			executeResetGui();
			resetGuiLive.timeFromReset = 0.f;
		}
	}
#endif
}

void handleEvent(sf::Event e)
{
	if(isVisible && e.type == sf::Event::TextEntered)
	{
		char key = Cast<char>(e.text.unicode);
		if(!iscntrl(key))
			content += key;
	}

	if(e.type == sf::Event::KeyPressed)
	{
		if(e.key.code == sf::Keyboard::Tab && e.key.control)
		{	
			isVisible = !isVisible;
			window->setKeyRepeatEnabled(isVisible);
			system::System::setPause(isVisible);
		}

		if(!isVisible)
			return;

		switch(e.key.code)
		{
			case sf::Keyboard::BackSpace: {
				if(content.size() > 0)
					content.pop_back();
			} break;

			case sf::Keyboard::Enter: {
				executeCommand();

				indexOfCurrentLastCommand = -1;
				if(content.size() != 0) 
				{
					lastCommands.emplace_front(content);
					if(lastCommands.size() > 10)
						lastCommands.pop_back();
				}

				content.clear();
			} break;

			case sf::Keyboard::Up: {
				if(indexOfCurrentLastCommand + 1 < Cast<i32>(lastCommands.size()))
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

	commandsMap["tp"] = &executeTeleport;
	commandsMap["tpp"] = &executeTeleportPoint;
	commandsMap["give"] = &executeGive;
	commandsMap["currentpos"] = &executeCurrentPos;
	commandsMap["mute"] = &executeMute;
	commandsMap["unmute"] = &executeUnmute;
	commandsMap["setvolume"] = &executeSetVolume;
	commandsMap["history"] = &executeHistory;
	commandsMap["help"] = &executeHelp;
	commandsMap["clear"] = &executeClear;
	commandsMap["gts"] = &executeGotoScene;
	commandsMap["r"] = &executeReset;
	commandsMap["pause"] = &executePause;
	commandsMap["rgui"] = &executeResetGui;
	commandsMap["m"] = &executeMove;
	commandsMap["fontd"] = &executeFontDebug;
	commandsMap["nofocusupdate"] = &executeNoFocusUpdate;
	commandsMap["fz"] = &executeFreezeZombies;
	commandsMap["gm"] = &executeGodMode;
	commandsMap[""] = &executeInfoMessage;

#ifndef PH_DISTRIBUTION
	commandsMap["rguilive"] = &executeResetGuiLive;
	commandsMap["rguilivefreq"] = &executeResetGuiLiveFrequency;
#endif

	// read terminalInit.txt file
	std::ifstream file;
	file.open("terminalInit.txt");
	if(file.is_open())
	{
		while(!file.eof())
		{
			getline(file, content);
			executeCommand();
		}
		file.close();
	}
}

void update(float dt)
{
	updateCommands(dt);

	if(isVisible)
	{
		Renderer::submitQuad(Null, Null, &sf::Color(0, 0, 0, 230), Null, {0.f, 660.f}, {1920.f, 420.f}, 5, 0.f, {},
			ProjectionType::gui, false);

		Renderer::submitQuad(Null, Null, &sf::Color::Black, Null, {0.f, 720.f}, {1920.f, 5.f},
			4, 0.f, {}, ProjectionType::gui, false);

		Renderer::submitText(content.c_str(), "LiberationMono-Bold.ttf", {5.f, 660.f},
			50.f, sf::Color::White, 0, ProjectionType::gui, false);

		float posY = 723.f;
		for(size_t i = 0; i < outputLines.size(); ++i, posY += 25.f)
			Renderer::submitTextArea(outputLines[i].text.c_str(), "LiberationMono.ttf", {5.f, posY}, 1920.f, TextAligment::left,
			                         25.f, outputLines[i].color, 0, ProjectionType::gui, false);
	}
}

}
