#include "globalKeyboardShortcuts.hpp"
#include "gameData.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

void handleGlobalKeyboardShortcuts(sf::Window& renderWindow, GameCloser& gameCloser, const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F11)
		{
			auto windowSize = renderWindow.getSize();

			if(windowSize == sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height)) {
				renderWindow.create(sf::VideoMode(640, 360), "PopHead", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3));
				Renderer::reset(640, 360);
			}
			else {
				renderWindow.create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen, sf::ContextSettings(24, 8, 0, 3, 3));
				Renderer::reset(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
			}
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem) 
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
		gameCloser.closeGame();
}

}
