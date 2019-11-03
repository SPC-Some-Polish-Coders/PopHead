#include "game.hpp"

#include "Logs/logs.hpp"
#include "Gui/messageBox.hpp"

#include <stdexcept>
#include <string>

#include "Logs/logger.hpp"
#include "Logs/logsInitializing.hpp"

#include "Gui/xmlGuiParser.hpp"
#include "Gui/guiActionsParserImpl.hpp"

#include "Events/actionEventManager.hpp"

//#include "ECS/System.hpp"
#include "ECS/Systems/movement.hpp"
#include "ECS/Systems/playerInput.hpp"

#include "ECS/systemsQueue.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

#include <iostream>

int main()
{
	try {
		PH_LOG_INFO("start initializing PopHead");
		ph::Game game;

		ph::XmlGuiParser::setActionsParser(std::make_unique<ph::GuiActionsParserImpl>());

		// TODO: change place of initializing logs to start of main(), because now it needs Terminal from Game
		ph::initializeLogsModule("config/logsConfig.ini", game.getTerminal());
		
		PH_LOG_INFO("start executing PopHead");
		game.run();
	}
	catch (const ph::CriticalError& criticalError) {
		ph::showErrorMessageBox("Critical Error: ", criticalError.what());
		throw;
	}
	catch (const std::exception& e) {
		PH_LOG_WARNING("Standard exceptions should be handled in code.");
		ph::showErrorMessageBox("Error", e.what());
		throw;
	}
	catch (...) {
		ph::showErrorMessageBox("Error", "Unknown error occurred!");
		throw;
	}

	/*ph::ActionEventManager::init();

	sf::RenderWindow window(sf::VideoMode(500, 500), "Test");
	sf::Texture texture;
	texture.loadFromFile("resources/textures/vehicles/car.png");

	using namespace ph;

	entt::registry reg;

	ph::SystemsQueue queue(reg);
	queue.appendSystem<system::PlayerInput>();
	queue.appendSystem<system::Movement>();
	queue.appendSystem<system::SpritesSync>();
	queue.appendSystem<system::Renderer>(std::ref(window));

	component::Sprite spr;
	spr.sprite.setTexture(texture);

	auto entity = reg.create();
	reg.assign<component::Position>(entity, 0.f, 0.f);
	reg.assign<component::Velocity>(entity, 0.f, 0.f);
	reg.assign<component::Sprite>(entity, std::move(spr));
	reg.assign<component::CharacterSpeed>(entity, 55.f);
	reg.assign<component::Player>(entity);
	
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time delta = clock.restart();
		queue.update(delta.asSeconds());
	}

	std::cin.get();*/

	return 0;
}
