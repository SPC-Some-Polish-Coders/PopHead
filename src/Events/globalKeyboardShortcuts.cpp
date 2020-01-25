#include "globalKeyboardShortcuts.hpp"
#include "Renderer/renderer.hpp"
#include "game.hpp"
#include "GUI/widget.hpp"

namespace ph {

void handleGlobalKeyboardShortcuts(sf::Window& window, const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F11)
		{
			auto windowSize = window.getSize();
			if(windowSize == sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height)) {
				window.create(sf::VideoMode(640, 360), "PopHead", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3));
				Renderer::restart(640, 360);
				Widget::setScreenSize({640, 360});
			}
			else {
				window.create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen, sf::ContextSettings(24, 8, 0, 3, 3));
				sf::Vector2u fullScreenSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
				Renderer::restart(fullScreenSize.x, fullScreenSize.y);
				Widget::setScreenSize(static_cast<sf::Vector2f>(fullScreenSize));
			}
			window.setVerticalSyncEnabled(true);
			window.setKeyRepeatEnabled(false);
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem)
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) 
	{
		Game::close();
	}
}

}
